#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <signal.h>
#include <termios.h>
#include <sstream>
#include <string>
#include <math.h>
#include <cmath>
#include <stdint.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/String.h>

#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>
#include <ros/ros.h>
int PS[17],CPI[9],CP[11];

void CapteurCallbackRail(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=16;i++) PS[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
	for(int i=1;i<=10;i++) CP[i] = (msg->data & (int32_t)pow(2,15+i)) > 0;
}

void CapteurCallbackStation(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=8;i++) CPI[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
}

int ChangeStopState(int numStop)
{
	int stateStopFunction;
	stateStopFunction = pow(2,numStop-1);
	return stateStopFunction;
}
void PublishStopChange(int stateStopPublish, int oldstateStop, ros::Publisher stopState)
{
	std_msgs::Int32 resultStop; 
	resultStop.data = stateStopPublish;

	if(stateStopPublish!=oldstateStop)
		stopState.publish(resultStop);
}

int ChangeSwitchState(int numSwitch)
{
	int stateSwitchFunction;
	stateSwitchFunction = pow(2,numSwitch-1);
	return stateSwitchFunction;
}
void PublishSwitchChange(int stateSwitchPublish, int oldstateSwitch, ros::Publisher switchState)
{
	std_msgs::Int32 resultSwitch; 
	resultSwitch.data = stateSwitchPublish;

	if(stateSwitchPublish!=oldstateSwitch)
		switchState.publish(resultSwitch);
}

int main(int argc, char **argv)
{	
	ros::init(argc, argv, "tp_etudiant");
	ros::NodeHandle nh;
	ros::Publisher switchStatePublisher = nh.advertise<std_msgs::Int32>("/tp_etudiant/switchState", 20);
	ros::Publisher stopStatePublisher = nh.advertise<std_msgs::Int32>("/tp_etudiant/stopState", 20);

	ros::Subscriber subSensorRailState = nh.subscribe("/simulation/RailSensorState", 1, CapteurCallbackRail);
	ros::Subscriber subSensorStationState = nh.subscribe("/simulation/StationSensorState", 1, CapteurCallbackStation);
	
	std_msgs::Int32 resultStopmain; 
	int stateStop, oldstateStop(0), stateSwitch, oldstateSwitch(0);
	bool first=true;
	bool first2=false;

	bool firstg=true;
	bool firstg2=true;

	ros::Time end;
	ros::Rate loop_rate(25);

	while (ros::ok())
	{
		stateStop=0;
		stateSwitch=0;
		

	/////// exemple coté droit (simu avec 1 et 5)

		if (PS[6] == 1){	
			stateStop+=ChangeStopState(7); 						
		}
		if (CP[4] == 1){	
			stateStop+=ChangeStopState(7);
			stateStop+=ChangeStopState(12); 	
			stateSwitch+=ChangeSwitchState(5); 							
			stateSwitch+=ChangeSwitchState(6); 									
		}
		if((PS[9]==1)&&(first==true)){		
			stateSwitch+=ChangeSwitchState(6); 
			first=false;
		}
	
		if (CP[6] == 1){	
			stateStop+=ChangeStopState(17);
			stateStop+=ChangeStopState(12); 	
			stateSwitch+=ChangeSwitchState(7); 							
			stateSwitch+=ChangeSwitchState(8); 								
		}
		if ((PS[12] == 1)&&(first2==false)){	
			stateSwitch+=ChangeSwitchState(8); 	
			first2=true;							
		}

		if ((CP[8] == 1)&&(first==false)){
			stateStop+=ChangeStopState(17);		
			stateStop+=ChangeStopState(19);	
			first=true;							
		}


	/////// exemple coté gauche (simu avec 2 et 6)

		if (PS[14] == 1){	
			stateStop+=ChangeStopState(19);						
		}
		if (CP[9] == 1){	
			stateStop+=ChangeStopState(22);	
			stateStop+=ChangeStopState(19);
			stateSwitch+=ChangeSwitchState(11); 
			stateSwitch+=ChangeSwitchState(12); 									
		}


		if (CPI[1] == 1){
			stateSwitch+=ChangeSwitchState(12);	
			stateStop+=ChangeStopState(3);
			stateStop+=ChangeStopState(22);	
			stateSwitch+=ChangeSwitchState(1);	
			stateSwitch+=ChangeSwitchState(2);	
		}

		if((CP[3]==1)&&(firstg==true)){		
			stateSwitch+=ChangeSwitchState(2); 
			stateStop+=ChangeStopState(3);
			stateStop+=ChangeStopState(7);
			first=false;
		}


		PublishStopChange(stateStop, oldstateStop ,stopStatePublisher);
		PublishSwitchChange(stateSwitch, oldstateSwitch ,switchStatePublisher);
		oldstateStop=stateStop;
		oldstateSwitch=stateSwitch;
		ros::spinOnce();
		loop_rate.sleep();

	}

	return 0;


}



