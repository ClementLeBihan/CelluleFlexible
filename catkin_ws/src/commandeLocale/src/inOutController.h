#ifndef IN_OUT_CONTROLLER
#define IN_OUT_CONTROLLER

#include "UI.h"
#include <ros/ros.h>
#include "commandeLocale/Msg_SwitchControl.h"
#include "commandeLocale/Msg_StopControl.h"
#include "commandeLocale/Msg_SensorState.h"

#include <std_msgs/Int32.h>

class inOutController
{
private:
	UI* userInterface;
	ros::Publisher VREPSwitchControllerRight;
	ros::Publisher VREPSwitchControllerLeft;
	ros::Publisher VREPSwitchControllerLock;
	ros::Publisher VREPStopController;
	ros::Publisher VREPGoController;
	ros::Publisher planifRailSensorState;
	ros::Subscriber subSensorState;
	ros::Subscriber VREPsubRailSensor;
	ros::Subscriber VREPsubStationSensor;
	ros::Subscriber VREPsubSwitchSensor;
	ros::Subscriber planifSubSwitchState;
	ros::Subscriber planifSubStopState;

	commandeLocale::Msg_StopControl StopControl;
	commandeLocale::Msg_SwitchControl SwitchControl;
	commandeLocale::Msg_SensorState SensorState;
public:
	inOutController(UI* usrInt);
	void init(ros::NodeHandle nh);
	void SensorCallbackRail(const std_msgs::Int32::ConstPtr& msg);
	void SensorCallbackSwitch(const std_msgs::Int32::ConstPtr& msg);
	void SensorCallbackStation(const std_msgs::Int32::ConstPtr& msg);
	void StateSwitchCallBack(const commandeLocale::Msg_SwitchControl::ConstPtr&  msg);
	void StateStopCallBack(const commandeLocale::Msg_StopControl::ConstPtr&  msg);
};
#endif
