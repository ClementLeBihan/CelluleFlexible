#include "capteurState.h"
#include <std_msgs/Int32.h>
#include <ros/ros.h>

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

#include <simulation/Msg_SensorState.h>

void capteurState::SensorCallback(const simulation::Msg_SensorState::ConstPtr& msg)
{
	for(int i=1;i<=16;i++) PS[i] = msg->PS[i];
	for(int i=1;i<=10;i++) CP[i] = msg->CP[i];
	for(int i=1;i<=8;i++) CPI[i] = msg->CPI[i];
	for(int i=1;i<=12;i++) DG[i] = msg->DG[i];
	for(int i=1;i<=12;i++) DD[i] = msg->DD[i];
}

capteurState::capteurState () {
	for(int i=1;i<=16;i++) PS[i] = 0;
	for(int i=1;i<=10;i++) CP[i] = 0;
	for(int i=1;i<=8;i++) CPI[i] = 0;
	for(int i=1;i<=12;i++) DG[i] = 0;
	for(int i=1;i<=12;i++) DD[i] = 0;
}
void capteurState::init(ros::NodeHandle n)
{
	subSensorState = n.subscribe("/simulation/SensorState", 1, &capteurState::SensorCallback,this);
}
