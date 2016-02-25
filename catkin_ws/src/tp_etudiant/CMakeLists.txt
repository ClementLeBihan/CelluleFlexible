
cmake_minimum_required(VERSION 2.8.3)
project(tp_etudiant)

find_package(catkin REQUIRED)
find_package(catkin REQUIRED COMPONENTS 
	roscpp 
	std_msgs 
	genmsg 
	message_generation)
find_package(OpenCV)

include_directories(${catkin_INCLUDE_DIRS})
include_directories(include ${OpenCV_INCLUDE_DIRS})
set(EXECUTABLE_OUTPUT_PATH ~/Projet_Long)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_executable(tp_etudiant src/tp_etudiant.cpp src/capteurState.cpp src/switchState.cpp src/stopState.cpp) 
target_link_libraries(tp_etudiant ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(tp_etudiant vrep_common_generate_messages_cpp)
add_message_files(
	FILES
	Msg_SensorState.msg
	Msg_StopControl.msg
	Msg_SwitchControl.msg
)
generate_messages( DEPENDENCIES std_msgs)
catkin_package( CATKIN_DEPENDS message_runtime) 


