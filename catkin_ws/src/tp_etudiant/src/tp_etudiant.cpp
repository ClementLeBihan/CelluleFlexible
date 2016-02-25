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

	bool firstg=true;
	bool firstg2=true;

	ros::Time end;
	ros::Rate loop_rate(25);

	while (ros::ok())
	{

	/////// exemple coté droit (simu avec 1 et 5)

		if (CapteurSt.PS[6] == 1){	
			stopSt.ChangeStopState(7); 						
		}
		if (CapteurSt.CP[4] == 1){	
			stopSt.ChangeStopState(7);
			stopSt.ChangeStopState(12); 	
			switchSt.ChangeSwitchState(5); 							
			switchSt.ChangeSwitchState(6); 									
		}
		if((CapteurSt.PS[9]==1)&&(first==true)){		
			switchSt.ChangeSwitchState(6); 
			first=false;
		}
	
		if (CapteurSt.CP[6] == 1){	
			stopSt.ChangeStopState(17);
			stopSt.ChangeStopState(12); 	
			switchSt.ChangeSwitchState(7); 							
			switchSt.ChangeSwitchState(8); 								
		}
		if ((CapteurSt.PS[12] == 1)&&(first2==false)){	
			switchSt.ChangeSwitchState(8); 	
			first2=true;							
		}

		if ((CapteurSt.CP[8] == 1)&&(first==false)){
			stopSt.ChangeStopState(17);		
			stopSt.ChangeStopState(19);	
			first=true;							
		}


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

<<<<<<< HEAD
		if((CapteurSt.CP[3]==1)&&(firstg==true)){		
			switchSt.ChangeSwitchState(2); 
			stopSt.ChangeStopState(3);
			stopSt.ChangeStopState(7);
			first=false;
=======
		if((CP[3]==1)&&(firstg==true)){		
			stateSwitch+=ChangeSwitchState(2); 
			stateStop+=ChangeStopState(3);
			stateStop+=ChangeStopState(7);
			firstg=false;
>>>>>>> b8f047e50db888dd8f148c97ec139a19fd9e17c9
		}

		ros::spinOnce();
		loop_rate.sleep();

	}

	return 0;


}
