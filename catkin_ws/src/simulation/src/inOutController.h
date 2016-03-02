#ifndef IN_OUT_CONTROLLER
#define IN_OUT_CONTROLLER

#include "UI.h"
#include <ros/ros.h>
#include "simulation/Msg_SwitchControl.h"
#include "simulation/Msg_StopControl.h"
#include "simulation/Msg_SensorState.h"

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
	ros::Publisher TPrailSensorState;
	ros::Subscriber subSensorState;
	ros::Subscriber VREPsubRailSensor;
	ros::Subscriber VREPsubStationSensor;
	ros::Subscriber VREPsubSwitchSensor;
	ros::Subscriber TPsubSwitchState;
	ros::Subscriber TPsubStopState;

	simulation::Msg_StopControl StopControl;
	simulation::Msg_SwitchControl SwitchControl;
	simulation::Msg_SensorState SensorState;
public:
	inOutController(UI* usrInt);
	void init(ros::NodeHandle nh);
	void SensorCallbackRail(const std_msgs::Int32::ConstPtr& msg);
	void SensorCallbackSwitch(const std_msgs::Int32::ConstPtr& msg);
	void SensorCallbackStation(const std_msgs::Int32::ConstPtr& msg);
	void StateSwitchCallBack(const simulation::Msg_SwitchControl::ConstPtr&  msg);
	void StateStopCallBack(const simulation::Msg_StopControl::ConstPtr&  msg);
};
#endif
