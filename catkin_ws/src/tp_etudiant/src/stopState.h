#ifndef STOP_STATE
#define STOP_STATE
#include <ros/ros.h>
#include "simulation/Msg_StopControl.h"

class stopState
{
private:
	ros::Publisher stopStatePublisher;
	simulation::Msg_StopControl stateStop;
public:

	stopState ();
	void stop(int numStop);
	void go(int numStop);
	void publish();
	void init(ros::NodeHandle n);
};

#endif
