#include "inOutController.h"

#include <std_msgs/Int32.h>
#include <ros/ros.h>

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

#include <simulation/Msg_SwitchControl.h>
#include <simulation/Msg_StopControl.h>
#include <simulation/Msg_SensorState.h>

inOutController::inOutController(UI* usrInt)
{
	userInterface = usrInt;
}

void inOutController::SensorCallbackRail(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=16;i++) SensorState.PS[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
	for(int i=1;i<=10;i++) SensorState.CP[i] = (msg->data & (int32_t)pow(2,15+i)) > 0;
	userInterface->DrawRailSensorImg(SensorState);
	TPrailSensorState.publish(SensorState);
}

void inOutController::SensorCallbackSwitch(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=12;i++){
		SensorState.DD[i] = (msg->data & (int32_t)pow(2,2*i-2)) > 0;
		SensorState.DG[i] = (msg->data & (int32_t)pow(2,2*i-1)) > 0;
	}
	userInterface->DrawSwitchSensorImg(SensorState);
	TPrailSensorState.publish(SensorState);
}

void inOutController::SensorCallbackStation(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=8;i++) SensorState.CPI[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
	userInterface->DrawStationSensorImg(SensorState);
	TPrailSensorState.publish(SensorState);
}

void inOutController::StateSwitchCallBack(const simulation::Msg_SwitchControl::ConstPtr&  msg)
{
	int SwitchRightControl(0), SwitchLeftControl(0), SwitchLockControl(0);
	std_msgs::Int32 Right, Left, Lock;

	SwitchControl.RD = msg->RD;
	SwitchControl.RG = msg->RG;
	SwitchControl.LOCK = msg->LOCK;

	for (int i=1;i<=12;i++){
		if (msg->RD[i]==true)
			SwitchRightControl+=pow(2,i-1);
		if (msg->RG[i]==true)
			SwitchLeftControl+=pow(2,i-1);
		if (msg->LOCK[i]==true)
			SwitchLockControl+=pow(2,i-1);
	}

	Right.data=SwitchRightControl;
	VREPSwitchControllerRight.publish(Right);
	Left.data=SwitchLeftControl;
	VREPSwitchControllerLeft.publish(Left);
	Lock.data=SwitchLockControl;
	VREPSwitchControllerLock.publish(Lock);
}

void inOutController::StateStopCallBack(const simulation::Msg_StopControl::ConstPtr&  msg)
{
	int StopControlInt(0), GoControlInt(0);
	std_msgs::Int32 Stop, Go;

	StopControl.STOP = msg->STOP;
	StopControl.GO = msg->GO;

	for (int i=1;i<=24;i++){
		if (msg->STOP[i]==true)
			StopControlInt+=pow(2,i-1);
		if (msg->GO[i]==true)
			GoControlInt+=pow(2,i-1);
	}

	Stop.data=StopControlInt;
	VREPStopController.publish(Stop);
	Go.data=GoControlInt;
	VREPGoController.publish(Go);
}

void inOutController::init(ros::NodeHandle nh)
{
	// Subscribe
   	VREPsubRailSensor = nh.subscribe("vrep/RailSensor", 1, &inOutController::SensorCallbackRail, this);
	VREPsubStationSensor = nh.subscribe("vrep/StationSensor", 1, &inOutController::SensorCallbackStation, this);
	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1, &inOutController::SensorCallbackSwitch, this);
	TPsubSwitchState = nh.subscribe("tp_etudiant/TPSwitchControl", 1, &inOutController::StateSwitchCallBack, this);
	TPsubStopState = nh.subscribe("tp_etudiant/TPStopControl", 1, &inOutController::StateStopCallBack, this);

	// Publishers
	VREPSwitchControllerRight = nh.advertise<std_msgs::Int32>("/simulation/SwitchControllerRight", 1);
	VREPSwitchControllerLeft = nh.advertise<std_msgs::Int32>("/simulation/SwitchControllerLeft", 1);
	VREPSwitchControllerLock = nh.advertise<std_msgs::Int32>("/simulation/SwitchControllerLock", 1);
	VREPStopController = nh.advertise<std_msgs::Int32>("/simulation/StopController", 1);
	VREPGoController = nh.advertise<std_msgs::Int32>("/simulation/GoController", 1);
	TPrailSensorState = nh.advertise<simulation::Msg_SensorState>("/simulation/SensorState", 1);
}
