cmake_minimum_required(VERSION 2.8.3)
project(simulation)

find_package(catkin REQUIRED)
find_package(catkin REQUIRED COMPONENTS std_msgs sensor_msgs image_transport vrep_common cv_bridge genmsg)
find_package(OpenCV)

include_directories(${catkin_INCLUDE_DIRS})
include_directories(include ${OpenCV_INCLUDE_DIRS})
set(EXECUTABLE_OUTPUT_PATH ~/Projet_Long)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_executable(simulation src/simulation.cpp) 
target_link_libraries(simulation ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(simulation vrep_common_generate_messages_cpp)
