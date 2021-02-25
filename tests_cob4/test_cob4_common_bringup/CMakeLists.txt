cmake_minimum_required(VERSION 2.8.3)
project(test_cob4_common_bringup)

add_compile_options(-std=c++11)

find_package(gazebo_ros_link_attacher QUIET)

if(gazebo_ros_link_attacher_FOUND)
find_package(catkin REQUIRED
  COMPONENTS 
    roscpp
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
#    tf2
#    tf2_ros
#    tf2_eigen
#    pluginlib
#    geometric_shapes
#    geometry_msgs
#    control_msgs
#    vicon_object_server_msgs
#    vicon_object_server_library
#    fetch_move_base_msgs
#    gazebo_ros_link_attacher
)
else()
find_package(catkin REQUIRED
  COMPONENTS
    roscpp
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
#    tf2
#    tf2_ros
#    tf2_eigen
#    pluginlib
#    geometric_shapes
#    geometry_msgs
#    control_msgs
#    vicon_object_server_msgs
#    vicon_object_server_library
#    fetch_move_base_msgs
)
endif()

find_package(Boost REQUIRED system filesystem date_time thread)

catkin_package(
  CATKIN_DEPENDS
#    moveit_core
#    moveit_ros_planning_interface
#    tf2
#    tf2_ros
#    fetch_move_base_msgs
)

###########
## Build ##
###########

include_directories(SYSTEM ${Boost_INCLUDE_DIR})
include_directories(${catkin_INCLUDE_DIRS})
link_directories(${catkin_LIBRARY_DIRS})

if(gazebo_ros_link_attacher_FOUND)
	add_definitions(-DGAZEBO_LINK_ATTACHER)
endif()

add_executable(cob4_moveit_demo src/cob4_moveit_demo.cpp)
target_link_libraries(cob4_moveit_demo ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(cob4_moveit_demo ${catkin_EXPORTED_TARGETS})

#### script to position cob4 to a home status ####
add_executable(cob4_moveit_home src/cob4_moveit_home.cpp)
target_link_libraries(cob4_moveit_home ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(cob4_moveit_home ${catkin_EXPORTED_TARGETS})


catkin_add_env_hooks(99.test_cob4_common_functions SHELLS bash DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/env-hooks)
