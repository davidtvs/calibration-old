<?xml version="1.0"?>
<package>
  <name>calibration</name>
  <version>0.0.0</version>
  <description>The calibration package</description>

  <!-- One maintainer tag required, multiple allowed, one person per tag -->
  <!-- Example:  -->
  <!-- <maintainer email="jane.doe@example.com">Jane Doe</maintainer> -->
  <maintainer email="almeida.j@ua.pt">Jorge Almeida</maintainer>
  <author>Marcelo Pereira</author>
  <author>David Silva</author>


  <!-- One license tag required, multiple allowed, one license per tag -->
  <!-- Commonly used license strings: -->
  <!--   BSD, MIT, Boost Software License, GPLv2, GPLv3, LGPLv2.1, LGPLv3 -->
  <license>BSD</license>


  <!-- Url tags are optional, but mutiple are allowed, one per tag -->
  <!-- Optional attribute type can be: website, bugtracker, or repository -->
  <!-- Example: -->
  <!-- <url type="website">http://wiki.ros.org/calibration</url> -->


  <!-- Author tags are optional, mutiple are allowed, one per tag -->
  <!-- Authors do not have to be maintianers, but could be -->
  <!-- Example: -->
  <!-- <author email="jane.doe@example.com">Jane Doe</author> -->


  <!-- The *_depend tags are used to specify dependencies -->
  <!-- Dependencies can be catkin packages or system dependencies -->
  <!-- Examples: -->
  <!-- Use build_depend for packages you need at compile time: -->
  <!--   <build_depend>message_generation</build_depend> -->
  <!-- Use buildtool_depend for build tool packages: -->
  <!--   <buildtool_depend>catkin</buildtool_depend> -->
  <!-- Use run_depend for packages you need at runtime: -->
  <!--   <run_depend>message_runtime</run_depend> -->
  <!-- Use test_depend for packages you need only for testing: -->
  <!--   <test_depend>gtest</test_depend> -->
  <buildtool_depend>catkin</buildtool_depend>
  <build_depend>roscpp</build_depend>
  <build_depend>std_msgs</build_depend>
  <build_depend>colormap</build_depend>
  <build_depend>roslib</build_depend>
  <build_depend>lidar_segmentation</build_depend>

  <run_depend>roscpp</run_depend>
  <run_depend>std_msgs</run_depend>
  <run_depend>colormap</run_depend>
  <run_depend>roslib</run_depend>
  <run_depend>lidar_segmentation</run_depend>



<build_depend version_gte="1.7.0">pcl</build_depend>
+  <build_depend>libpcl-all-dev</build_depend>
   <build_depend>pcl_msgs</build_depend>
   <build_depend>sensor_msgs</build_depend>
   <build_depend>image_transport</build_depend>

-  <run_depend version_gte="1.7.0">pcl</run_depend>
+  <run_depend>libpcl-all</run_depend>
   <run_depend>pcl_msgs</run_depend>
   <run_depend>sensor_msgs</run_depend>
   <run_depend>image_transport</run_depend>


  <!-- The export tag contains other, unspecified, tags -->
  <export>
    <!-- Other tools can request additional information be placed here -->

  </export>
</package>
