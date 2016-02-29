#include "switchState.h"
#include <std_msgs/Int32.h>
#include <ros/ros.h>

#include <math.h> // pow(2,4) = 2⁴

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

#include "simulation/Msg_SwitchControl.h"

switchState::switchState () {
	for(int i=1;i<=12;i++) stateSwitches.LOCK[i] = 1;
	for(int i=1;i<=12;i++) stateSwitches.RD[i] = 1;
	for(int i=1;i<=12;i++) stateSwitches.RG[i] = 0;
}

void switchState::init(ros::NodeHandle n)
{
	switchStatePublisher = n.advertise<simulation::Msg_SwitchControl>("/tp_etudiant/TPSwitchControl", 1);
}

void switchState::unlockSwitch(int numSwitch)
{
	stateSwitches.LOCK[numSwitch] = 0;
	publish();
}
void switchState::lockSwitch(int numSwitch)
{
	stateSwitches.LOCK[numSwitch] = 1;
	publish();
}
void switchState::turnRight(int numSwitch)
{
	stateSwitches.RD[numSwitch] = 1;
	stateSwitches.RG[numSwitch] = 0;
	publish();		
}
void switchState::turnLeft(int numSwitch)
{
	stateSwitches.RD[numSwitch] = 0;
	stateSwitches.RG[numSwitch] = 1;
	publish();		
}

void switchState::publish()
{
	switchStatePublisher.publish(stateSwitches);
}
