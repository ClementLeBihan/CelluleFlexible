#include "stopState.h"
#include <std_msgs/Int32.h>
#include <ros/ros.h>

#include <math.h> // pow(2,4) = 2⁴

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

int stopState::ChangeStopState(int numStop)
{
	if((stateStop & int(pow(2,numStop-1)))>0) stateStop = stateStop - int(pow(2,numStop-1));
	else stateStop = stateStop + int(pow(2,numStop-1));

	PublishStopChange();
}
void stopState::PublishStopChange()
{
	std_msgs::Int32 resultStop; 
	resultStop.data = stateStop;

	stopStatePublisher.publish(resultStop);

	stateStop = 0;
}

stopState::stopState () {}

void stopState::init(ros::NodeHandle n)
{
	stopStatePublisher = n.advertise<std_msgs::Int32>("/tp_etudiant/stopState", 20);
}
