#include "switchState.h"
#include <std_msgs/Int32.h>
#include <ros/ros.h>

#include <math.h> // pow(2,4) = 2⁴

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

void switchState::ChangeSwitchState(int numSwitch)
{
	if((stateSwitches & int(pow(2,numSwitch-1)))>0) stateSwitches = stateSwitches - int(pow(2,numSwitch-1));
	else stateSwitches = stateSwitches + int(pow(2,numSwitch-1));

	PublishSwitchChange();
}
void switchState::PublishSwitchChange()
{
	std_msgs::Int32 resultSwitch; 
	resultSwitch.data = stateSwitches;

	switchStatePublisher.publish(resultSwitch);

	stateSwitches = 0;
}

switchState::switchState () {}

void switchState::init(ros::NodeHandle n)
{
	switchStatePublisher = n.advertise<std_msgs::Int32>("/tp_etudiant/switchState", 20);
}
