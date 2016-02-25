#ifndef STOP_STATE
#define STOP_STATE
#include <ros/ros.h>
#include "tp_etudiant/Msg_StopControl.h"

class stopState
{
private:
	ros::Publisher stopStatePublisher;
	tp_etudiant::Msg_StopControl stateStop;
public:

	stopState ();
	void stop(int numStop);
	void go(int numStop);
	void PublishStopChange();
	void init(ros::NodeHandle n);
};

#endif
