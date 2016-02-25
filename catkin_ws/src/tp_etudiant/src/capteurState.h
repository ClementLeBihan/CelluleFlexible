#ifndef CAPTEUR_STATE
#define CAPTEUR_STATE

#include <tp_etudiant/Msg_SensorState.h>
#include <ros/ros.h>

class capteurState
{
private:
	ros::Subscriber subSensorState;
public:
	int PS[17],CPI[9],CP[11], DD[13], DG[13];
	capteurState ();
	void init(ros::NodeHandle n);
	void SensorCallback(const tp_etudiant::Msg_SensorState::ConstPtr& msg);
};

#endif
