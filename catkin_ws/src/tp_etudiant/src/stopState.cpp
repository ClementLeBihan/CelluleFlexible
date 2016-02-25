#include "stopState.h"
#include <std_msgs/Int32.h>
#include <ros/ros.h>

#include <math.h> // pow(2,4) = 2⁴

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

stopState::stopState () {}

void stopState::stop(int numStop)
{
	stateStop.ST[numStop] = 1;
	stateStop.GO[numStop] = 0;
}

void stopState::go(int numStop)
{
	stateStop.ST[numStop] = 0;
	stateStop.GO[numStop] = 1;
}
void stopState::publish()
{
	stopStatePublisher.publish(stateStop);
}

void stopState::init(ros::NodeHandle n)
{
	stopStatePublisher = n.advertise<tp_etudiant::Msg_StopControl>("/tp_etudiant/stopState", 1);
}
