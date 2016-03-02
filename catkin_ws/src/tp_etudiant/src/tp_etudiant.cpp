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

	ros::Rate loop_rate(25);

///////////////////////////////////////////
//////// Déclaration des variables ////////
///////////////////////////////////////////

	int jeton_d = 1;
	int jeton_g = 1;

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
	while (ros::ok())
	{
		switch(jeton_d)
		{

		///////////////////////////////////
		//////////// Coder ICI ////////////
		///////////////////////////////////


			case 1 :
				if (CapteurSt.PS[6] == 1) {
					stopSt.stop(7); 							
					jeton_d++;	}
				break;

			case 2 :			
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
					jeton_d++;	}
				break;

			case 3 :
				if (CapteurSt.DG[5] == 1){
					stopSt.go(8);
					jeton_d++;	}
				break;

			case 4 :
				if(CapteurSt.PS[9]==1){
					switchSt.unlockSwitch(6); 		
					switchSt.turnRight(6);	
					switchSt.lockSwitch(6);
					jeton_d++;	}
			break;

			case 5:
				if (CapteurSt.DD[6] == 1){
					stopSt.go(12); 	
					stopSt.stop(13);
					jeton_d++;	}
			break;
		
			case 6:
				if (CapteurSt.CP[6] == 1){	
					stopSt.stop(17);
					switchSt.unlockSwitch(7); 
					switchSt.turnLeft(7);	
					switchSt.lockSwitch(7); 
					switchSt.unlockSwitch(8); 							
					switchSt.turnLeft(8);	
					switchSt.lockSwitch(8); 		
					jeton_d++;	}
				break;
			
			case 7:
				if (CapteurSt.DG[7] == 1){
					stopSt.go(13);
					jeton_d++;	}
				break;

			case 8:
				if (CapteurSt.PS[12] == 1){	
					switchSt.unlockSwitch(8); 
					switchSt.turnRight(8);	
					switchSt.lockSwitch(8); 
					stopSt.stop(19); 
					jeton_d++;	}
				break;

			case 9:
				if (CapteurSt.DD[8] == 1){
					stopSt.go(17); 	
					jeton_d++;	}
				break;
		}

		switch(jeton_g)
		{
			case 1:
				if (CapteurSt.PS[14] == 1){	
					stopSt.stop(19);
					jeton_g++;	}
			break;
			
			case 2:
				if (CapteurSt.CP[9] == 1){
					stopSt.go(19);	
					stopSt.stop(20);
					stopSt.stop(22);
					jeton_g++;	}
			break;

			case 3:
				if (CapteurSt.CPI[8] == 1){
					stopSt.stop(21);			
					stopSt.go(20);
					jeton_g++;	}
			break;

			case 4:
				if (CapteurSt.CP[9] == 1){
					stopSt.go(22);
					jeton_g++;	}
				break;

			case 5 :
				if (CapteurSt.CP[1] == 1){
					stopSt.go(21);
					stopSt.stop(3);								
					switchSt.unlockSwitch(1); 
					switchSt.turnLeft(1);	
					switchSt.lockSwitch(1); 
					switchSt.unlockSwitch(2); 
					switchSt.turnLeft(2);	
					switchSt.lockSwitch(2); 
					jeton_g++;	}
				break;

			case 6 :
				if (CapteurSt.PS[2] == 1){	
					stopSt.go(3);	
					stopSt.stop(5);
					jeton_g++;	}
				break;

			case 7 :
				if (CapteurSt.PS[4] == 1){	
					switchSt.unlockSwitch(2); 
					switchSt.turnRight(2);	
					switchSt.lockSwitch(2);
					stopSt.stop(7);						
					jeton_g++;	}
				break;

				case 8:
					if (CapteurSt.DD[2] == 1){	
						stopSt.go(5);					
						jeton_g++;	}
					break;
		}

		///////////////////////////////////
		//////////// fin code /////////////
		///////////////////////////////////

		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;


}
