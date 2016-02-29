#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <signal.h>
#include <termios.h>
#include <sstream>
#include <stdint.h>
#include "capteurState.h"
#include "switchState.h"
#include "stopState.h"
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

	bool first=true;
	bool first2=false;
	bool firstdd6=false;
	bool firstdd8=false;
	bool firstdg7=true;
	bool firstdg5=true;

	bool firstg=true;
	bool second=false;
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
		}



	/////// exemple coté gauche (simu avec 2 et 6)

		if (CapteurSt.PS[14] == 1){	
			stopSt.stop(19);
		}
		if (CapteurSt.CP[9] == 1){
			stopSt.go(19);	
			stopSt.stop(20);
			stopSt.stop(22);								
		}
		if (CapteurSt.CPI[8] == 1){
			stopSt.stop(21);								
			stopSt.go(20);
			second=true;
		}
		if ((CapteurSt.CP[9] == 1)&&(second==true)){
			stopSt.go(22);
			second=false;
		}
		if (CapteurSt.CP[1] == 1){
			stopSt.go(21);
			stopSt.stop(3);								
			switchSt.unlockSwitch(1); 
			switchSt.turnLeft(1);	
			switchSt.lockSwitch(1); 

			switchSt.unlockSwitch(2); 
			switchSt.turnLeft(2);	
			switchSt.lockSwitch(2); 
			
		}
		if (CapteurSt.PS[2] == 1){	
			stopSt.go(3);	
			stopSt.stop(5);									
		}
		if ((CapteurSt.PS[4] == 1)&&(firstg==true)){	
			switchSt.unlockSwitch(2); 
			switchSt.turnRight(2);	
			switchSt.lockSwitch(2);
			stopSt.stop(7);									
			firstg2=false;	
			firstg=false;							
		}
		if ((CapteurSt.DD[2] == 1)&&(firstg2==false)){	
			stopSt.go(5);	
			firstg2=true;									
								
		}
		
		ros::spinOnce();
		loop_rate.sleep();


	}

	return 0;


}
