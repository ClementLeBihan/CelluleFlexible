#ifndef CAPTEUR_STATE
#define CAPTEUR_STATE

#include <std_msgs/Int32.h>
#include <ros/ros.h>

class capteurState
{
private:
	ros::Subscriber subSensorStationState;
	ros::Subscriber subSensorRailState;
public:
	int PS[17],CPI[9],CP[11];

	capteurState ();
	void init(ros::NodeHandle n);
	void CapteurCallbackRail(const std_msgs::Int32::ConstPtr& msg);
	void CapteurCallbackStation(const std_msgs::Int32::ConstPtr& msg);
};

#endif
