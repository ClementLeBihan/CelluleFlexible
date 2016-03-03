#include "sensorState.h"
#include "switchState.h"
#include "stopState.h"
#include <ros/ros.h>

int main(int argc, char **argv)
{	
//initialisation du noeud ros et création d'un handle associé au noeud
	ros::init(argc, argv, "planificateur");	
	ros::NodeHandle nh;

//création et initialisation des objets Capteur, Aiguillage et point d'arret
	sensorState SensorSt;
	SensorSt.init(nh);

	switchState switchSt;
	switchSt.init(nh);

	stopState stopSt;
	stopSt.init(nh);

	ros::Rate loop_rate(25); //fréquence de la boucle (25Hz)

///////////////////////////////////////////
//////// Déclaration des variables ////////
///////////////////////////////////////////

	int jeton_d(1), jeton_g(1);
	int jeton_verrd1(0), jeton_verrd2(0), jeton_verrd3(0), jeton_verrd4(0);
	int jeton_verrg1(0), jeton_verrg2(0);

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
	while (ros::ok())
	{
		///////////////////////////////////
		//////////// Coder ICI ////////////
		///////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Exemple partie droite/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
		switch(jeton_d)
		{
			case 1 :
				if (SensorSt.PS[6] == 1) {
					stopSt.stop(7); 							
					jeton_d++;	}
				break;

			case 2 :			
				if (SensorSt.CP[4] == 1){
					stopSt.go(7); 	
					stopSt.stop(8); 
					switchSt.unlockSwitch(5); 
					switchSt.turnLeft(5);
					stopSt.stop(12); 
					switchSt.unlockSwitch(6); 
					switchSt.turnLeft(6);	
					jeton_verrd1++;
					jeton_d++;	}
				break;

			case 3 :
				if (SensorSt.DG[5] == 1){
					stopSt.go(8);
					jeton_d++;	}
				break;

			case 4 :
				if(SensorSt.PS[9]==1){
					switchSt.unlockSwitch(6); 		
					switchSt.turnRight(6);	
					jeton_verrd2++;
					jeton_d++;	}
			break;

			case 5:
				if (SensorSt.DD[6] == 1){
					stopSt.go(12); 	
					stopSt.stop(13);
					jeton_d++;	}
			break;
		
			case 6:
				if (SensorSt.CP[6] == 1){	
					stopSt.stop(17);
					switchSt.unlockSwitch(7); 
					switchSt.turnLeft(7);	
					switchSt.unlockSwitch(8); 							
					switchSt.turnLeft(8);	
					jeton_verrd3++; 		
					jeton_d++;	}
				break;
			
			case 7:
				if (SensorSt.DG[7] == 1){
					stopSt.go(13);
					jeton_d++;	}
				break;

			case 8:
				if (SensorSt.PS[12] == 1){	
					switchSt.unlockSwitch(8); 
					switchSt.turnRight(8);	
					stopSt.stop(19); 
					jeton_verrd4++; 		
					jeton_d++;	}
				break;

			case 9:
				if (SensorSt.DD[8] == 1){
					stopSt.go(17); 	
					jeton_d++;	}
				break;
		}

//lorsqu'on tourne un aiguillage, on doit créer un RdP parallèle qui attend que l'aiguillage soit en place pour le revérouiller.

		switch(jeton_verrd1)
		{
			case 1||2:
				if (SensorSt.DG[5] == 1){
					switchSt.lockSwitch(5);
					jeton_verrd1++;
				}
				if (SensorSt.DG[6] == 1){
					switchSt.lockSwitch(6);
					jeton_verrd1++;
				}
		}
		switch(jeton_verrd2)
		{
			case 1:
				if (SensorSt.DD[6] == 1){
					switchSt.lockSwitch(6);
					jeton_verrd2++;
				}
		}
		switch(jeton_verrd3)
		{
			case 1||2:
				if (SensorSt.DG[7] == 1){
					switchSt.lockSwitch(7);
					jeton_verrd3++;
				}
				if (SensorSt.DG[8] == 1){
					switchSt.lockSwitch(8);
					jeton_verrd3++;
				}
		}
		switch(jeton_verrd4)
		{
			case 1:
				if (SensorSt.DD[8] == 1){
					switchSt.lockSwitch(8);
					jeton_verrd4++;
				}
		}



///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Exemple partie gauche/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

		switch(jeton_g)
		{
			case 1:
				if (SensorSt.PS[14] == 1){	
					stopSt.stop(19);
					jeton_g++;	}
			break;
			
			case 2:
				if (SensorSt.CP[9] == 1){
					stopSt.go(19);	
					stopSt.stop(20);
					stopSt.stop(22);
					jeton_g++;	}
			break;

			case 3:
				if (SensorSt.CPI[8] == 1){
					stopSt.stop(21);			
					stopSt.go(20);
					jeton_g++;	}
			break;

			case 4:
				if (SensorSt.CP[9] == 1){
					stopSt.go(22);
					jeton_g++;	}
				break;

			case 5 :
				if (SensorSt.CP[1] == 1){
					stopSt.go(21);
					stopSt.stop(3);								
					switchSt.unlockSwitch(1); 
					switchSt.turnLeft(1);	
					switchSt.unlockSwitch(2); 
					switchSt.turnLeft(2);	
					jeton_verrg1++;
					jeton_g++;	}
				break;

			case 6 :
				if (SensorSt.PS[2] == 1){	
					stopSt.go(3);	
					stopSt.stop(5);
					jeton_g++;	}
				break;

			case 7 :
				if (SensorSt.PS[4] == 1){	
					switchSt.unlockSwitch(2); 
					switchSt.turnRight(2);	
					stopSt.stop(7);
					jeton_verrg2++;						
					jeton_g++;	}
				break;

				case 8:
					if (SensorSt.DD[2] == 1){	
						stopSt.go(5);					
						jeton_g++;	}
					break;
		}

//lorsqu'on tourne un aiguillage, on doit créer un RdP parallèle qui attend que l'aiguillage soit en place pour le revérouiller.
		switch(jeton_verrg1)
		{
			case 1||2:
				if (SensorSt.DG[1] == 1){
					switchSt.lockSwitch(1);
					jeton_verrg1++;
				}
				if (SensorSt.DG[2] == 1){
					switchSt.lockSwitch(2);
					jeton_verrg1++;
				}
		}
		switch(jeton_verrg2)
		{
			case 1:
				if (SensorSt.DD[2] == 1){
					switchSt.lockSwitch(2);
					jeton_verrg2++;
				}
		}

		///////////////////////////////////
		//////////// fin code /////////////
		///////////////////////////////////


		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;


}
