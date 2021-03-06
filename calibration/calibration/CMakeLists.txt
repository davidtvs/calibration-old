cmake_minimum_required(VERSION 2.8.3)
project(calibration)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  colormap
  roslib
  image_transport
  cv_bridge
)
find_package(PkgConfig REQUIRED)
find_package(PCL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(lidar_segmentation REQUIRED)
if ( NOT lidar_segmentation_FOUND )
	message(FATAL_ERROR "Package lidar_segmentation required but not found!")
endif( NOT lidar_segmentation_FOUND )
#pkg_check_modules(GTK   REQUIRED gtk+-3.0)
#pkg_check_modules(GTKMM gtkmm-2.4 glibmm-2.4 gconfmm-2.6 libglademm-2.4)
#pkg_check_modules(GTKMM REQUIRED gtkmm-2.4 glibmm-2.4 gconfmm-2.6 libglademm-2.4)
## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependencies might have been
##     pulled in transitively but can be declared for certainty nonetheless:
##     * add a build_depend tag for "message_generation"
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
 INCLUDE_DIRS include
 CATKIN_DEPENDS roscpp std_msgs colormap roslib
)

#find_library(POINTGREY_LIB flycapture)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
link_directories(
    ${GTKMM_LIBRARY_DIRS}  )
include_directories(${catkin_INCLUDE_DIRS}
	            ${PCL_INCLUDE_DIRS}
		    ${OpenCV_INCLUDE_DIRS}
		    ${roscpp_INCLUDE_DIRS}
		    ${lidar_segmentation_INCLUDE_DIRS}
		    /usr/include/flycapture
		    include
                   )
find_library(FLYCAPTURE2 flycapture)

add_executable(sick_ldmrs src/visualization_rviz_ldmrs.cpp src/sick_ldmrs.cpp src/common_functions.cpp)

target_link_libraries(sick_ldmrs ${catkin_LIBRARIES}
			         ${PCL_LIBRARIES}
				 ${lidar_segmentation_LIBRARIES}
				 ${roscpp_LIBRARIES})

add_executable(sick_lms151_1 src/visualization_rviz_lms.cpp src/sick_lms151_1.cpp src/common_functions.cpp)

target_link_libraries(sick_lms151_1 ${catkin_LIBRARIES}
				    ${PCL_LIBRARIES}
				    ${lidar_segmentation_LIBRARIES}
				    ${roscpp_LIBRARIES})

add_executable(sick_lms151_2 src/visualization_rviz_lms2.cpp src/sick_lms151_2.cpp src/common_functions.cpp)

target_link_libraries(sick_lms151_2 ${catkin_LIBRARIES}
				    ${PCL_LIBRARIES}
				    ${lidar_segmentation_LIBRARIES}
				    ${roscpp_LIBRARIES})

add_executable(calibration src/calibration.cpp src/calibration_utils.cpp src/visualization_rviz_calibration.cpp)

target_link_libraries(calibration ${catkin_LIBRARIES}
				  ${PCL_LIBRARIES}
				  ${lidar_segmentation_LIBRARIES}
				  ${roscpp_LIBRARIES})

add_executable(calibration_pcd src/calibration_pcd.cpp src/calibration_utils.cpp src/visualization_rviz_calibration.cpp)

target_link_libraries(calibration_pcd ${catkin_LIBRARIES}
				  ${PCL_LIBRARIES}
				  ${lidar_segmentation_LIBRARIES}
				  ${roscpp_LIBRARIES})

add_executable(swissranger src/swissranger.cpp src/visualization_rviz_swissranger.cpp)

target_link_libraries(swissranger ${catkin_LIBRARIES}
				  ${PCL_LIBRARIES}
				  ${lidar_segmentation_LIBRARIES}
				  ${roscpp_LIBRARIES})

add_executable(camera src/camera.cpp src/camera_config.cpp src/visualization_rviz_swissranger.cpp)

target_link_libraries(camera ${catkin_LIBRARIES}
			     ${OpenCV_LIBS}
			     ${PCL_LIBRARIES}
			     flycapture
			     ${lidar_segmentation_LIBRARIES}
			     ${roscpp_LIBRARIES}
                     )

add_executable(single_camera src/single_camera.cpp src/camera_config.cpp src/visualization_rviz_swissranger.cpp)

target_link_libraries(single_camera ${catkin_LIBRARIES}
			     ${OpenCV_LIBS}
			     ${PCL_LIBRARIES}
			     flycapture
			     ${lidar_segmentation_LIBRARIES}
			     ${roscpp_LIBRARIES}
                     )

add_executable(camera_calib src/camera_calib.cpp src/camera_config.cpp)

target_link_libraries(camera_calib ${catkin_LIBRARIES}
			     ${OpenCV_LIBS}
			     flycapture
                     )

add_executable(single_camera_calib src/single_camera_calib.cpp src/camera_config.cpp)

target_link_libraries(single_camera_calib ${catkin_LIBRARIES}
			     ${OpenCV_LIBS}
			     flycapture
                     )

add_executable(single_camera_test src/single_camera_test.cpp src/camera_config.cpp src/visualization_rviz_swissranger.cpp)

target_link_libraries(single_camera_test ${catkin_LIBRARIES}
			     ${OpenCV_LIBS}
			     ${PCL_LIBRARIES}
			     flycapture
			     ${lidar_segmentation_LIBRARIES}
			     ${roscpp_LIBRARIES}
                     )

#add_library(segmentation src/lidar_segmentation.cpp src/clustering.cpp src/groundtruth.cpp src/visualization_rviz.cpp)

#target_link_libraries(segmentation ${catkin_LIBRARIES})
#target_link_libraries(segmentation ${PCL_LIBRARIES})
## Declare a cpp library
#add_library(clustering src/clustering.cpp src/lidar_segmentation.cpp src/groundtruth.cpp src/visualization_rviz.cpp)
#   src/${PROJECT_NAME}/lidar_segmentation.cpp
# )

## Declare a cpp executable
# add_executable(lidar_segmentation_node src/lidar_segmentation_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(lidar_segmentation_node lidar_segmentation_generate_messages_cpp)

## Specify libraries to link a library or executable target against
#target_link_libraries(clustering
#   ${catkin_LIBRARIES}
#   ${roscpp_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
 #install(TARGETS clustering
 #  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
 #  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
 #  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
 #  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
 #  FILES_MATCHING PATTERN "*.h"
  # PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_lidar_segmentation.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
