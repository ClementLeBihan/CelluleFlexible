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
#include "capteurState.h"
#include "switchState.h"
#include "stopState.h"
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>
#include <ros/ros.h>

int main(int argc, char **argv)
{	
	ros::init(argc, argv, "tp_etudiant");
	ros::NodeHandle nh;

	capteurState CapteurSt;
	CapteurSt.init(nh);

	switchState switchSt;
	switchSt.init(nh);

	stopState stopSt;
	stopSt.init(nh);

	std_msgs::Int32 resultStopmain; 
	int stateStop, oldstateStop(0), stateSwitch, oldstateSwitch(0);
	bool first=true;
	bool first2=false;
	bool firstdd6=false;
	bool firstdd8=false;
	bool firstdg7=true;
	bool firstdg5=true;

	bool firstg=true;
	bool firstg2=true;

	ros::Rate loop_rate(25);

	while (ros::ok())
	{
	/////// exemple coté droit (simu avec 1 et 5)
	
		if (CapteurSt.PS[6] == 1){
			stopSt.stop(7); 							
		} 
		if (CapteurSt.CP[4] == 1){
			stopSt.go(7); 	
			stopSt.stop(8); 
			switchSt.unlockSwitch(5); 
			switchSt.turnLeft(5);
			switchSt.lockSwitch(5); 
			stopSt.stop(12); 
			switchSt.unlockSwitch(6); 
			switchSt.turnLeft(6);	
			switchSt.lockSwitch(6); 							
		}
		if ((CapteurSt.DG[5] == 1)&&(firstdg5==true)){
			stopSt.go(8);
			firstdg5=false;	 	 	
		}

		if((CapteurSt.PS[9]==1)&&(first==true)){
			switchSt.unlockSwitch(6); 		
			switchSt.turnRight(6);	
			switchSt.lockSwitch(6);
			firstdd6=true; 							
			first=false;
		}
		if ((CapteurSt.DD[6] == 1)&&(firstdd6==true)){
			stopSt.go(12); 	
			stopSt.stop(13);
			firstdd6=false;	
		}
		if (CapteurSt.CP[6] == 1){	
			stopSt.stop(17);
			switchSt.unlockSwitch(7); 
			switchSt.turnLeft(7);	
			switchSt.lockSwitch(7); 
			switchSt.unlockSwitch(8); 							
			switchSt.turnLeft(8);	
			switchSt.lockSwitch(8); 										
		}
		if ((CapteurSt.DG[7] == 1)&&(firstdg7==true)){
			stopSt.go(13);
			firstdg7=false;	 	
		}
		if ((CapteurSt.PS[12] == 1)&&(first2==false)){	
			switchSt.unlockSwitch(8); 
			switchSt.turnRight(8);	
			switchSt.lockSwitch(8); 
			stopSt.stop(19); 
			firstdd8=true;								
			first2=true;							
		}
		if ((CapteurSt.DD[8] == 1)&&(firstdd8==true)){
			stopSt.go(17); 	
			firstdd8=false;	
		}/**/

/*

	/////// exemple coté gauche (simu avec 2 et 6)

		if (CapteurSt.PS[14] == 1){	
			stopSt.ChangeStopState(19);						
		}
		if (CapteurSt.CP[9] == 1){	
			stopSt.ChangeStopState(22);	
			stopSt.ChangeStopState(19);
			switchSt.ChangeSwitchState(11); 
			switchSt.ChangeSwitchState(12); 									
		}


		if (CapteurSt.CPI[1] == 1){
			switchSt.ChangeSwitchState(12);	
			stopSt.ChangeStopState(3);
			stopSt.ChangeStopState(22);	
			switchSt.ChangeSwitchState(1);	
			switchSt.ChangeSwitchState(2);	
		}


		if((CapteurSt.CP[3]==1)&&(firstg==true)){		
			switchSt.ChangeSwitchState(2); 
			stopSt.ChangeStopState(3);
			stopSt.ChangeStopState(7);
			first=false;}
*/		
		ros::spinOnce();
		loop_rate.sleep();


	}

	return 0;


}
