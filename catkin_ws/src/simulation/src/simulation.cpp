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

using namespace std;

// Used data structures:
#include "vrep_common/VrepInfo.h"
#include "vrep_common/simRosLoadModel.h"
#include "vrep_common/simRosPauseSimulation.h"
#include "vrep_common/simRosStartSimulation.h"

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

#include "UI.h"
#include "inOutController.h"

#include <ros/ros.h>

void LoadModel(ros::ServiceClient client_simRosLoadModel, std::string model_name)
{
	static vrep_common::simRosLoadModel srv_LoadModel;
	srv_LoadModel.request.fileName = model_name;			
	client_simRosLoadModel.call(srv_LoadModel);
}

int main(int argc, char **argv)
{
    pid_t pid;
 
    if ((pid = fork()) < 0)
        return EXIT_FAILURE;
    else if (pid == 0) 
		{
			system("~/Projet_Long/V-Rep/vrep.sh -h ~/Projet_Long/Simulation.ttt &");
			sleep(2);
			ros::init(argc, argv, "simulation2");
			ros::NodeHandle nh2;

			ros::ServiceClient client_simRosLoadModel = nh2.serviceClient<vrep_common::simRosLoadModel>("/vrep/simRosLoadModel");	
			
				for(int i=1;i<=argc-1;i++) {
					if(argv[i][0]>54 || argv[i][0]<48) printf(" ATTENTION, LE NUMERO DU SHUTTLE DOIT ETRE COMPRIS ENTRE 0 ET 6 \n");
					else {		
					if(argv[i][0] == 48) argv[i][0] = char(74);
					argv[i][0] = char(argv[i][0]+16);
					std::string shuttleName = "/home/etudiant/Projet_Long/V-Rep/models/Projet_Long/shuttle"+std::string(argv[i])+".ttm";
					LoadModel(client_simRosLoadModel,shuttleName); }
				}

			//Demarrage de la simulation
				system("rosservice call /vrep/simRosStartSimulation > /dev/null");
				system("rosservice call /vrep/simRosPauseSimulation > /dev/null");
		}
    else if (pid > 0) 
		{

			while(system("pidof -x vrep > /dev/null")) // Attente d'un process du nom de "vrep"
			sleep(2);
		
			//Initialisation du noeud
			ros::init(argc, argv, "simulation");
			ros::NodeHandle nh;

			UI userInterface;
			userInterface.init(nh);

			inOutController IOController(&userInterface);
			IOController.init(nh);

			while (ros::ok())
			{
				ros::spinOnce();
			}				
				
			userInterface.kill();
			
			system("pkill vrep");
			return 0;
		} 
}
