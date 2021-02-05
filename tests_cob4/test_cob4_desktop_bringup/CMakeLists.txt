cmake_minimum_required(VERSION 2.8.3)
project(test_cob4_desktop_bringup)

find_package(catkin REQUIRED COMPONENTS)

catkin_package()

catkin_add_env_hooks(99.test_cob4_desktop_functions SHELLS bash DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/env-hooks)