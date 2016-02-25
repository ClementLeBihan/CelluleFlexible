#ifndef STOP_STATE
#define STOP_STATE
#include <ros/ros.h>

class stopState
{
private:
	ros::Publisher stopStatePublisher;
	int stateStop;
public:

	stopState ();
	void init(ros::NodeHandle n);
	int ChangeStopState(int numStop);
	void PublishStopChange();
};

#endif
