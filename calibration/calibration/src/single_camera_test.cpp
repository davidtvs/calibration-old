
#include "calibration/camera_config_test.h"

//Marker's publisher
ros::Publisher ballCentroidCam_pub;
ros::Publisher ballCentroidCamPnP_pub;
ros::Publisher markers_pub;
image_transport::Publisher image_pub;

StereoBM sbm;
StereoSGBM sgbm;
Mat CameraMatrix1, CameraMatrix2, disCoeffs1, disCoeffs2, R1, R2, P1, P2, Q, T;

Mat dst;

int lowH;
int highH;
int lowS;
int highS;
int lowV;
int highV;
int valC;
int valA;
int maxR;
int minR;

void ImageCapture_bag(const sensor_msgs::ImageConstPtr& msg)
{
	Mat img;
	Mat imgHSV;
	Mat unImg;
	Mat imgBinary;

	img = cv_bridge::toCvShare(msg, "bgr8")->image;

	imshow("Camera", img);         // untouched image

	// =========================================================================
	// Pre-processing
	// =========================================================================

	// Convert the captured image frame BGR to HSV
	undistort(img,unImg,CameraMatrix1,disCoeffs1);
	cvtColor(unImg, imgHSV, COLOR_BGR2HSV);

	// Threshold the image
	inRange(imgHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), imgBinary);

	//morphological closing (fill small holes in the foreground)
	dilate( imgBinary, imgBinary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	erode(imgBinary, imgBinary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

	//morphological opening (remove small objects from the foreground)
	erode(imgBinary, imgBinary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( imgBinary, imgBinary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

	GaussianBlur( imgBinary, imgBinary, Size(3, 3),2, 2 );

	imshow("Binarized Image", imgBinary);

	// =====================================================================
	// Circle detection
	// Hough Circles or Polygonal Curve fitting algorithm, pick one
	// =====================================================================

	//HoughCircles(unImg, imgBinary, valC, valA, minRadius, maxRadius);

	PolygonalCurveDetection(unImg, imgBinary, valC);
	char ch = waitKey(1);
	if (ch == 'p') // saves image if 'p' key is pressed
	{
		imwrite("original.png", img);
		imwrite("ballDetectionCam.png", dst);
	}
}


/**
   @brief Ball detection in the stereo system
   @param[in]
   @return void
 */
//void ballDetection(sensor_msgs::PointCloud cloud)
void PolygonalCurveDetection( Mat &img, Mat &imgBinary, int valCanny )
{
	vector<vector<Point> > contours;

	/// Detect edges using canny
	Mat imgCanny;
	Canny( imgBinary, imgCanny, valCanny, valCanny*2, 3 );

	imshow("Canny", imgCanny); // debug

	findContours(imgCanny.clone(),contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	vector<Point> approx;
	dst = img.clone();
	vector<double> areas;

	pcl::PointXYZ centroid; // Point structure for centroid
	/* The following lines make sure that position (0,0,0) is published when
	   the ball is not detected (avoids publishing previous positions when the
	   ball is no longer detected) */
	centroid.x = 0;
	centroid.y = 0;
	centroid.z = 0;

	pcl::PointXYZ centroidRadius; // Point structure for centroid
	/* The following lines make sure that position (0,0,0) is published when
	   the ball is not detected (avoids publishing previous positions when the
	   ball is no longer detected) */
	centroidRadius.x = 0;
	centroidRadius.y = 0;
	centroidRadius.z = 0;

	for(int i=0; i<contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02,true);

		// Skip small or non-convex objects
		if (std::fabs(cv::contourArea(contours[i])) < 1000 || !cv::isContourConvex(approx))
			continue;


		// Detect and label circles
		if(approx.size()>=6)
		{
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = (r.width/2 + r.width/2)/2;

			if (abs(1 - ((double)r.width / r.height)) <= 0.2 && abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2
			    && area>1000)
			{
				//cout<< "radius - "<< radius <<endl; // DEBUGGING

				// Computing distnace from camera to center of detected circle
				double Dist;

				Dist = (CameraMatrix1.at<double>(0,0)*(BALL_DIAMETER/(radius*2)));
				std::stringstream s;
				s<<Dist;
				string str = s.str();
				setLabel(dst, str, contours[i]);

				centroid.x = r.x + radius;
				centroid.y = r.y + radius;
				centroid.z = radius;

				// Method based on circle radius =======================================
				cv::Mat image_vector = (cv::Mat_<double>(3,1) << centroid.x, centroid.y, 1);
				cv::Mat camera_vector = CameraMatrix1;

				camera_vector = CameraMatrix1.inv() * (Dist * image_vector);
				//cout << camera_vector << endl; // DEBUGGING
				//cout << CameraMatrix1.inv() << endl; // DEBUGGING
				centroidRadius.x = camera_vector.at<double>(0);
				centroidRadius.y = camera_vector.at<double>(1);
				centroidRadius.z = camera_vector.at<double>(2);
				//cout << centroidRadius << endl; // DEBUGGING
			}
		}
	}
	CentroidPub(centroid, centroidRadius);
	imshow("Circle", dst);
}


void CentroidPub( const pcl::PointXYZ centroid, const pcl::PointXYZ centroidRadius)
{
	// Method based on solvePnP ================================================
	geometry_msgs::PointStamped CentroidCam;

	CentroidCam.point.x = centroid.x;
	CentroidCam.point.y = centroid.y;
	CentroidCam.point.z = centroid.z;

	CentroidCam.header.stamp = ros::Time::now();
	ballCentroidCamPnP_pub.publish(CentroidCam);
	//std::cout << CentroidCam << std::endl;

	// Method based on circle radius ===========================================
	CentroidCam.point.x = centroidRadius.x;
	CentroidCam.point.y = centroidRadius.y;
	CentroidCam.point.z = centroidRadius.z;

	CentroidCam.header.stamp = ros::Time::now();
	ballCentroidCam_pub.publish(CentroidCam);
	//std::cout << CentroidCam << std::endl;

	visualization_msgs::MarkerArray targets_markers;
	targets_markers.markers = createTargetMarkers(centroid);
	markers_pub.publish(targets_markers);
}

void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.8;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);
	int radius = (r.width/2 + r.height/2)/2;

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::Point center(r.x + ((r.width) / 2), r.y + ((r.height) / 2));
	//cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	//cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
	//circle( im, center, 4, Scalar(255,0,0), -1, 8, 0 );
	cv::line(im, cv::Point(center.x - 5, center.y), cv::Point(center.x + 5, center.y), cv::Scalar(255,255,255), 2);  //crosshair horizontal
	cv::line(im, cv::Point(center.x, center.y - 5), cv::Point(center.x, center.y + 5), cv::Scalar(255,255,255), 2);  //crosshair vertical
	// circle outline
	circle( im, center, radius, Scalar(255,255,255), 2, 8, 0 );
}

/**
   @brief Main function of the camera node
   @param argc
   @param argv
   @return int
 */
int main(int argc, char **argv)
{
	ros::init(argc, argv, "Point_Grey");
	ros::NodeHandle n;

	//read calibration paraneters
	string a="/src/mystereocalib.yml";
	string path = ros::package::getPath("calibration");
	path=path+a;
	FileStorage fs(path, FileStorage::READ);
	if(!fs.isOpened())
	{
		cout<<"failed to open document"<<endl;
		return -1;
	}

	fs["CM1"] >> CameraMatrix1;
	fs["D1"] >> disCoeffs1;

	// create the main window, and attach the trackbars
	namedWindow( "Camera", CV_WINDOW_NORMAL );
	namedWindow( "Binarized Image", CV_WINDOW_NORMAL );
	namedWindow( "Control", CV_WINDOW_NORMAL );
	namedWindow( "Circle", CV_WINDOW_NORMAL );
	namedWindow( "Canny", CV_WINDOW_NORMAL );

	/* Trackbars for Hue, Saturation and Value (HSV) in "Camera 1" window */
	lowH = 142;
	highH = 179;

	lowS = 45;
	highS = 255;

	lowV = 0;
	highV = 255;

	valC = 200;

	valA = 150;

	maxR = 300;
	minR = 150;

	// Hue 0-179
	cvCreateTrackbar("Upper Hue        ", "Control", &highH, 179);
	cvCreateTrackbar("Lower Hue        ", "Control", &lowH, 179);
	// Saturation 0-255
	cvCreateTrackbar("Upper Saturation", "Control", &highS, 255);
	cvCreateTrackbar("Lower Saturation", "Control", &lowS, 255);
	// Value 0-255
	cvCreateTrackbar("Upper Value     ", "Control", &highV, 255);
	cvCreateTrackbar("Lower Value     ", "Control", &lowV, 255);

	cvCreateTrackbar("Canny Threshold ", "Control", &valC, 200);

	ballCentroidCam_pub = n.advertise<geometry_msgs::PointStamped>( "/SingleCamera/ballCentroid", 1);
	ballCentroidCamPnP_pub = n.advertise<geometry_msgs::PointStamped>( "/SingleCamera/ballCentroidPnP", 1);
	markers_pub = n.advertise<visualization_msgs::MarkerArray>( "/markers4", 1000);
	image_transport::ImageTransport it(n);

	image_transport::Subscriber sub = it.subscribe("/SingleCamera/image", 1, ImageCapture_bag);

	ros::spin();

	destroyWindow("Camera"); //destroy the window
	destroyWindow("Binarized Image");
	destroyWindow("Control");
	destroyWindow("Circle");

	return 0;
}
