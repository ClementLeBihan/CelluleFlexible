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

// Image Streaming
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

// Used data structures:
#include "vrep_common/VrepInfo.h"
#include "vrep_common/JointSetStateData.h"
#include "vrep_common/simRosGetObjectHandle.h"
#include "vrep_common/simRosSetObjectQuaternion.h"
#include "std_msgs/String.h"

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>

#include <ros/ros.h>


int CP1,CP10,CP2,CP9,CPI1,CPI2,CPI7,CPI8,D10G,D11D,D11G,D12D,D12G,D1D,D1G,D2D,D2G,D3G;
int PS1,PS2,PS20,PS21,PS22,PS23,PS24,PS3,PS4,PS5,PS6 ; 


// Fonction qui stocke dans capteur 1 la valeur du capteur (0 ou 1)
void CapteurCallback(const std_msgs::Int32::ConstPtr& msg)
{
	CP1 = (msg->data & 1) == 1;
	CP10 = (msg->data & 2) == 2;
	CP2 = (msg->data & 4) == 4;
	CP9 = (msg->data & 8) == 8;
	CPI1 = (msg->data & 16) == 16;
	CPI2 = (msg->data & 32) == 32;
	CPI7 = (msg->data & 64) == 64;
	CPI8 = (msg->data & 128) == 128;
	D10G = (msg->data & 256) == 256;
	D11D = (msg->data & 512) == 512;
	D11G = (msg->data & 1024) == 1024;
	D12D = (msg->data & 2048) == 2048;
	D12G = (msg->data & 4096) == 4096;
	D1D = (msg->data & 8192) == 8192;
	D1G = (msg->data & 16384) == 16384;
	D2D = (msg->data & 32768) == 32768;
	D2G = (msg->data & 65536) == 65536;
	D3G = (msg->data & 131072) == 131072;
	PS1 = (msg->data & 262144) == 262144;
	PS2 = (msg->data & 524288) == 524288;
	PS20 = (msg->data & 1048576) == 1048576;
	PS21 = (msg->data & 2097152) == 2097152;
	PS22 = (msg->data & 4194304) == 4194304;
	PS23 = (msg->data & 8388608) == 8388608;
	PS24 = (msg->data & 16777216) == 16777216;
	PS3 = (msg->data & 33554432) == 33554432;
	PS4 = (msg->data & 67108864) == 67108864;
	PS5 = (msg->data & 134217728) == 134217728;
	PS6 = (msg->data & 268435456) == 268435456;
	
}


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    cv::imshow("Simulation", cv_bridge::toCvShare(msg, "bgr8")->image);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}


string float2string(float f)
{
  ostringstream os;
  os << f;
  return os.str();
}

string int2string(int f)
{
  ostringstream os;
  os << f;
  return os.str();
}



int main(int argc, char **argv)
{
    pid_t pid;
 
    if ((pid = fork()) < 0)
        return EXIT_FAILURE;
    else if (pid == 0) 
		{
          system("~/Projet_Long/V-Rep/vrep.sh -h -sXXX -q ~/Projet_Long/Simulation.ttt");
		}
    else if (pid > 0) 
		{
			sleep(2);
			//initialisation du noeud
			ros::init(argc, argv, "simulation");
			ros::NodeHandle n,nh;

			///////// SUBSCRIBERS ////////
				// Image Streaming 
		 		cv::namedWindow("Simulation");
				cv::startWindowThread();
				image_transport::ImageTransport it(nh);
				image_transport::Subscriber subImage = it.subscribe("vrep/VisionSensorData", 1, imageCallback);

				// Sensors State
		 		ros::Subscriber subSensor = nh.subscribe("vrep/CapteurTot", 1, CapteurCallback);

			///////// PUBLISHERS ////////
				//MotorVelocity
				ros::Publisher MotorVelocity = n.advertise<std_msgs::Float64>("/simulation/MotorVelocityTopic", 1);

			///////// VARIABLES ////////
				std_msgs::Float64 velo30, velo0;
				int handle_A1,handle_A2,handle_A3,handle_A10,handle_A11,handle_A12, handle_sol;
				int consigne = 0;
				bool choix_aiguill = true ; 
				ros::spinOnce();

			////////// VREP HANDLE  ///////
				ros::ServiceClient client_getObjectHandle=nh.serviceClient<vrep_common::simRosGetObjectHandle>("/vrep/simRosGetObjectHandle");
				vrep_common::simRosGetObjectHandle srv_getObjectHandle;
				
				// Aiguillage 1
				srv_getObjectHandle.request.objectName="A1";
				if ( client_getObjectHandle.call(srv_getObjectHandle))
				    handle_A1 = srv_getObjectHandle.response.handle;
				// Aiguillage 2
				srv_getObjectHandle.request.objectName="A2";
				if ( client_getObjectHandle.call(srv_getObjectHandle))
				    handle_A2 = srv_getObjectHandle.response.handle;
				// Aiguillage 3
				srv_getObjectHandle.request.objectName="A3";
				if ( client_getObjectHandle.call(srv_getObjectHandle))
				    handle_A3 = srv_getObjectHandle.response.handle;
				// Aiguillage 10
				srv_getObjectHandle.request.objectName="A10";
				if ( client_getObjectHandle.call(srv_getObjectHandle))
				    handle_A10 = srv_getObjectHandle.response.handle;
				// Aiguillage 11
				srv_getObjectHandle.request.objectName="A11";
				if ( client_getObjectHandle.call(srv_getObjectHandle))
				    handle_A11 = srv_getObjectHandle.response.handle;
				// Aiguillage 12
				srv_getObjectHandle.request.objectName="A12";
				if ( client_getObjectHandle.call(srv_getObjectHandle))
				    handle_A12 = srv_getObjectHandle.response.handle;

				// Sol
				srv_getObjectHandle.request.objectName="ResizableFloor_5_25";
				if ( client_getObjectHandle.call(srv_getObjectHandle))
				    handle_sol = srv_getObjectHandle.response.handle;

			//////// VREP  SetObjectQuaternion /////
				ros::ServiceClient client_SetObjectQuaternion=nh.serviceClient<vrep_common::simRosSetObjectQuaternion>("/vrep/simRosSetObjectQuaternion");
				vrep_common::simRosSetObjectQuaternion srv_SetObjectQuaternion;
				
				srv_SetObjectQuaternion.request.relativeToObjectHandle = handle_sol;			

			while (ros::ok())
			{
				ros::spinOnce();
				
				//On donne le quaternion voulu en fonction de la position (en fonction des états des capteurs)
				if (PS1==1) { 
					srv_SetObjectQuaternion.request.handle = handle_A1; //A1 double choix
					if ((D1G==1)&&(D1D==1)&&(consigne == 0)){ //Droite
						srv_SetObjectQuaternion.request.quaternion.x=0;
						srv_SetObjectQuaternion.request.quaternion.y=0;
						srv_SetObjectQuaternion.request.quaternion.z=0.173665;
						srv_SetObjectQuaternion.request.quaternion.w=0.984807;
						consigne = 1; 					
					}
					/*else{//gauche
						srv_SetObjectQuaternion.request.quaternion.x=0;
						srv_SetObjectQuaternion.request.quaternion.y=0;
						srv_SetObjectQuaternion.request.quaternion.z=-0.939695;
						srv_SetObjectQuaternion.request.quaternion.w=-0.342014;
						consigne = 1; 
					}*/
					
				}
				if (PS5==1){ 
					srv_SetObjectQuaternion.request.handle = handle_A2;
					if ((D2D==0) && (consigne == 0)) { //A2 a droite
						srv_SetObjectQuaternion.request.quaternion.x=0;
						srv_SetObjectQuaternion.request.quaternion.y=0;
						srv_SetObjectQuaternion.request.quaternion.z=-0.996195;
						srv_SetObjectQuaternion.request.quaternion.w=-0.087150;
						consigne = 1; 
					}
				}
				if (PS4==1){ 
					srv_SetObjectQuaternion.request.handle = handle_A2;
					if ((D2G==0) && (consigne == 0)) { //A2 a gauche
						srv_SetObjectQuaternion.request.quaternion.x=0;
						srv_SetObjectQuaternion.request.quaternion.y=0;
						srv_SetObjectQuaternion.request.quaternion.z=-0.573572;
						srv_SetObjectQuaternion.request.quaternion.w=0.819155;
						consigne = 1; 
					}
				}
				if (PS20==1){ 
					srv_SetObjectQuaternion.request.handle = handle_A11;	
					if (consigne == 0) { //A11 double choix
						if (choix_aiguill){//droite
							srv_SetObjectQuaternion.request.quaternion.x=0;
							srv_SetObjectQuaternion.request.quaternion.y=0;
							srv_SetObjectQuaternion.request.quaternion.z=-0.906305;
							srv_SetObjectQuaternion.request.quaternion.w=0.422624;
						}
						else{//gauche
							srv_SetObjectQuaternion.request.quaternion.x=0;
							srv_SetObjectQuaternion.request.quaternion.y=0;
							srv_SetObjectQuaternion.request.quaternion.z=0.819156;
							srv_SetObjectQuaternion.request.quaternion.w=0.573571;
						}			
					consigne = 1; 
					}
				}
				if (PS23==1){
					srv_SetObjectQuaternion.request.handle = handle_A12;
					if ((D12G==0) && (consigne == 0)) { //A12 a gauche
						srv_SetObjectQuaternion.request.quaternion.x=0;
						srv_SetObjectQuaternion.request.quaternion.y=0;
						srv_SetObjectQuaternion.request.quaternion.z=-0.766040;
						srv_SetObjectQuaternion.request.quaternion.w=0.642792;
						consigne = 1; 
					}
				}
				if (PS24==1){
					srv_SetObjectQuaternion.request.handle = handle_A12;
					if ((D12D==0) && (consigne == 0)) { //A12 a droite
					srv_SetObjectQuaternion.request.quaternion.x=0;
					srv_SetObjectQuaternion.request.quaternion.y=0;
					srv_SetObjectQuaternion.request.quaternion.z=0.173665;
					srv_SetObjectQuaternion.request.quaternion.w=0.984807;
					consigne = 1; 
					}
				}
				
				if(consigne == 1){
					client_SetObjectQuaternion.call(srv_SetObjectQuaternion);
					consigne = 0;
				}
				!choix_aiguill;
			}	
			cv::destroyWindow("view");
			return 0;
		}
}





