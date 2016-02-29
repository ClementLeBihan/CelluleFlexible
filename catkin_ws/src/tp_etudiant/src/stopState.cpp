#include "stopState.h"
#include <std_msgs/Int32.h>
#include <simulation/Msg_StopControl.h>
#include <ros/ros.h>

#include <math.h> // pow(2,4) = 2⁴

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

stopState::stopState () {
	for(int i=1;i<=24;i++) stateStop.ST[i] = 0;
	for(int i=1;i<=24;i++) stateStop.GO[i] = 1;
}

void stopState::stop(int numStop)
{
	stateStop.ST[numStop] = 1;
	stateStop.GO[numStop] = 0;
	publish();
}

void stopState::go(int numStop)
{
	stateStop.ST[numStop] = 0;
	stateStop.GO[numStop] = 1;
	publish();
}
void stopState::publish()
{
	stopStatePublisher.publish(stateStop);
}

void stopState::init(ros::NodeHandle n)
{
	stopStatePublisher = n.advertise<simulation::Msg_StopControl>("/tp_etudiant/TPStopControl", 1);
}
