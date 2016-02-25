#include "capteurState.h"
#include <std_msgs/Int32.h>
#include <ros/ros.h>

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

void capteurState::CapteurCallbackRail(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=16;i++) PS[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
	for(int i=1;i<=10;i++) CP[i] = (msg->data & (int32_t)pow(2,15+i)) > 0;
}

void capteurState::CapteurCallbackStation(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=8;i++) CPI[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
}

capteurState::capteurState () {
	for(int i=1;i<=16;i++) PS[i] = 0;
	for(int i=1;i<=10;i++) CP[i] = 0;
	for(int i=1;i<=8;i++) CPI[i] = 0;
}
void capteurState::init(ros::NodeHandle n)
{
	subSensorRailState = n.subscribe("/simulation/RailSensorState", 1, &capteurState::CapteurCallbackRail,this);
	subSensorStationState = n.subscribe("/simulation/StationSensorState", 1, &capteurState::CapteurCallbackStation, this);
}
