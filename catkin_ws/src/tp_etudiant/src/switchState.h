#ifndef SWITCH_STATE
#define SWITCH_STATE
#include <ros/ros.h>

class switchState
{
private:
	ros::Publisher switchStatePublisher;
	int stateSwitches;
public:

	switchState ();
	void init(ros::NodeHandle n);
	void ChangeSwitchState(int numSwitch);
	void PublishSwitchChange();
};

#endif
