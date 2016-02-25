#include "switchState.h"
#include <std_msgs/Int32.h>
#include <ros/ros.h>

#include <math.h> // pow(2,4) = 2⁴

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

#include "tp_etudiant/Msg_SwitchControl.h"

switchState::switchState () {}

void switchState::init(ros::NodeHandle n)
{
	switchStatePublisher = n.advertise<tp_etudiant::Msg_SwitchControl>("/tp_etudiant/switchState", 1);
}

void switchState::unlockSwitch(int numSwitch)
{
	stateSwitches.LOCK[numSwitch] = 0;
}
void switchState::lockSwitch(int numSwitch)
{
	stateSwitches.LOCK[numSwitch] = 1;
}
void switchState::turnRight(int numSwitch)
{
	stateSwitches.RD[numSwitch] = 1;
	stateSwitches.RG[numSwitch] = 0;		
}

void switchState::PublishSwitchChange()
{
	switchStatePublisher.publish(stateSwitches);
}
