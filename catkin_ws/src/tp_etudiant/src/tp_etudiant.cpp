#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <signal.h>
#include <termios.h>
#include <sstream>
#include <string>
#include <math.h>
#include <cmath>
#include <stdint.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/String.h>

#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>
#include <ros/ros.h>
int PS01,PS02,PS03,PS04,PS05,PS06,PS07,PS08,PS09,PS10,PS11,PS12,PS13,PS14,PS15,PS16;
int CP01,CP02,CP03,CP04,CP05,CP06,CP07,CP08,CP09,CP10;
int CPI01,CPI02,CPI03,CPI04,CPI05,CPI06,CPI07,CPI08;
std_msgs::Int32 resultStop; 
int stateStop(0) ;
void CapteurCallbackRail(const std_msgs::Int32::ConstPtr& msg)
{
	PS01 = (msg->data & 1) > 0;
	PS02 = (msg->data & 2) > 0;
	PS03 = (msg->data & 4) > 0;
	PS04 = (msg->data & 8) > 0;
	PS05 = (msg->data & 16) > 0;
	PS06 = (msg->data & 32) > 0;
	PS07 = (msg->data & 64) > 0;
	PS08 = (msg->data & 128) > 0;
	PS09 = (msg->data & 256) > 0;
	PS10 = (msg->data & 512) > 0;
	PS11 = (msg->data & 1024) > 0;
	PS12 = (msg->data & 2048) > 0;
	PS13 = (msg->data & 4096) > 0;
	PS14 = (msg->data & 8192) > 0;
	PS15 = (msg->data & 16384) > 0;
	PS16 = (msg->data & 32768) > 0;
	CP01 = (msg->data & 65536) > 0;
	CP02 = (msg->data & 131072) > 0;
	CP03 = (msg->data & 262144) > 0;
	CP04 = (msg->data & 524288) > 0;
	CP05 = (msg->data & 1048576) > 0;
	CP06 = (msg->data & 2097152) > 0;
	CP07 = (msg->data & 4194304) > 0;
	CP08 = (msg->data & 8388608) > 0;
	CP09 = (msg->data & 16777216) > 0;
	CP10 = (msg->data & 33554432) > 0;
}



void CapteurCallbackStation(const std_msgs::Int32::ConstPtr& msg)
{
	CPI01 = (msg->data & 1) > 0;
	CPI02 = (msg->data & 2) > 0;
	CPI03 = (msg->data & 4) > 0;
	CPI04 = (msg->data & 8) > 0;
	CPI05 = (msg->data & 16) > 0;
	CPI06 = (msg->data & 32) > 0;
	CPI07 = (msg->data & 64) > 0;
	CPI08 = (msg->data & 128) > 0;
}

int ChangeStateStop(int numStop)
{
	stateStop += pow(2,numStop-1);
}


int main(int argc, char **argv)
{	
	bool pub = false ; 
	ros::init(argc, argv, "tp_etudiant");
	ros::NodeHandle nh;
	ros::Publisher switchState = nh.advertise<std_msgs::String>("/tp_etudiant/switchState", 1);
	ros::Publisher stopState = nh.advertise<std_msgs::Int32>("/tp_etudiant/stopState", 1);
	ros::Subscriber subSensorRailState = nh.subscribe("/simulation/RailSensorState", 1, CapteurCallbackRail);
	ros::Subscriber subSensorStationState = nh.subscribe("/simulation/StationSensorState", 1, CapteurCallbackStation);
	
	int publish = 0 ; 
	bool pubps05=false;
	std_msgs::String valueSwitch ; 
	while (ros::ok())
	{
		if (PS05 == 1){
			pubps05=true;
		}
		if (pubps05 == true){
			if (PS05 == 0){
				pubps05=false;	
				//valueSwitch.data = "10 1";

				ChangeStateStop(1); 
				ChangeStateStop(2);
				ChangeStateStop(3);

				resultStop.data = stateStop ; 
				stopState.publish(resultStop); 	
				ROS_INFO("publ tp_etudiant");	
			}		
		}
		/*if (PS05 == 1 && pub == false){
			valueSwitch.data = "10 1"; 
			pub = true ; 
		}
		if (CP03 == 1 && pub == false){
			valueSwitch.data = "6 1"; 
			pub = true ; 
		}
		if (pub)
		{
			switchState.publish(valueSwitch);
			pub = false ; 
		}*/
		ros::spinOnce();
	}

	return 0;


}
