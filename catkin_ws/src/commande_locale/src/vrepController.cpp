#include "vrepController.h"
#include "vrep_common/simRosPauseSimulation.h"
#include "vrep_common/simRosStartSimulation.h"

#include "vrep_common/simRosLoadModel.h"

#include <ros/ros.h>

void vrepController::pause()
{
	client_simRosPauseSimulation.call(srv_PauseSimulation);
}

void vrepController::play()
{
	client_simRosStartSimulation.call(srv_StartSimulation);				
}

void vrepController::loadModel(char shuttleNumber)
{
	if(shuttleNumber>54 || shuttleNumber<48) printf(" ATTENTION, LE NUMERO DU SHUTTLE DOIT ETRE COMPRIS ENTRE 0 ET 6 \n");
	else {		
		if(shuttleNumber == 48) shuttleNumber = char(74); // SI 0 -> ShuttleZ
		shuttleNumber = char(shuttleNumber+16);
		std::string shuttleName = "/home/etudiant/Projet_Long/V-Rep/models/Projet_Long/shuttle"+std::string(&shuttleNumber)+".ttm";
		srv_LoadModel.request.fileName = shuttleName;			
		client_simRosLoadModel.call(srv_LoadModel);
	}
}
void vrepController::close()
{
	system("pkill vrep");
}
void vrepController::init(ros::NodeHandle n)
{
	system("~/Projet_Long/V-Rep/vrep.sh -h ~/Projet_Long/Simulation.ttt &");
	sleep(2);
	client_simRosLoadModel = n.serviceClient<vrep_common::simRosLoadModel>("/vrep/simRosLoadModel");
	client_simRosStartSimulation = n.serviceClient<vrep_common::simRosStartSimulation>("/vrep/simRosStartSimulation");	
	client_simRosPauseSimulation = n.serviceClient<vrep_common::simRosPauseSimulation>("/vrep/simRosPauseSimulation");	
}
