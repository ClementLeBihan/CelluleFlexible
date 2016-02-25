#ifndef SWITCH_STATE
#define SWITCH_STATE
#include <ros/ros.h>
#include "tp_etudiant/Msg_SwitchControl.h"

class switchState
{
private:
	ros::Publisher switchStatePublisher;
	tp_etudiant::Msg_SwitchControl stateSwitches;
public:

	switchState ();
	void unlockSwitch(int numSwitch);
	void lockSwitch(int numSwitch);
	void turnRight(int numSwitch);
	void PublishSwitchChange();
	void init(ros::NodeHandle n);
};

#endif
