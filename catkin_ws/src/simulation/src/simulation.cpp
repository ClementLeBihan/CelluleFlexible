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
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

// Used data structures:
#include "vrep_common/VrepInfo.h"
#include "vrep_common/simRosGetObjectHandle.h"
#include "vrep_common/simRosSetJointTargetPosition.h"
#include "std_msgs/String.h"

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>

#include <ros/ros.h>

#define NB_AIGUILLAGE 12

cv::Mat image_Capteur;

int PS01,PS02,PS03,PS04,PS05,PS06,PS07,PS08,PS09,PS10,PS11,PS12,PS13,PS14,PS15,PS16;
int CP01,CP02,CP03,CP04,CP05,CP06,CP07,CP08,CP09,CP10;

// Fonction qui stocke dans capteur 1 la valeur du capteur (0 ou 1)
void CapteurCallbackRail(const std_msgs::Int32::ConstPtr& msg)
{
	PS01 = (msg->data & (int32_t)pow(2,0)) > 0;
	PS02 = (msg->data & (int32_t)pow(2,1)) > 0;
	PS03 = (msg->data & (int32_t)pow(2,2)) > 0;
	PS04 = (msg->data & (int32_t)pow(2,3)) > 0;
	PS05 = (msg->data & (int32_t)pow(2,4)) > 0;
	PS06 = (msg->data & (int32_t)pow(2,5)) > 0;
	PS07 = (msg->data & (int32_t)pow(2,6)) > 0;
	PS08 = (msg->data & (int32_t)pow(2,7)) > 0;
	PS09 = (msg->data & (int32_t)pow(2,8)) > 0;
	PS10 = (msg->data & (int32_t)pow(2,9)) > 0;
	PS11 = (msg->data & (int32_t)pow(2,10)) > 0;
	PS12 = (msg->data & (int32_t)pow(2,11)) > 0;
	PS13 = (msg->data & (int32_t)pow(2,12)) > 0;
	PS14 = (msg->data & (int32_t)pow(2,13)) > 0;
	PS15 = (msg->data & (int32_t)pow(2,14)) > 0;
	PS16 = (msg->data & (int32_t)pow(2,15)) > 0;
	CP01 = (msg->data & (int32_t)pow(2,16)) > 0;
	CP02 = (msg->data & (int32_t)pow(2,17)) > 0;
	CP03 = (msg->data & (int32_t)pow(2,18)) > 0;
	CP04 = (msg->data & (int32_t)pow(2,19)) > 0;
	CP05 = (msg->data & (int32_t)pow(2,20)) > 0;
	CP06 = (msg->data & (int32_t)pow(2,21)) > 0;
	CP07 = (msg->data & (int32_t)pow(2,22)) > 0;
	CP08 = (msg->data & (int32_t)pow(2,23)) > 0;
	CP09 = (msg->data & (int32_t)pow(2,24)) > 0;
	CP10 = (msg->data & (int32_t)pow(2,25)) > 0;
}

int CPI01,CPI02,CPI03,CPI04,CPI05,CPI06,CPI07,CPI08;

void CapteurCallbackStation(const std_msgs::Int32::ConstPtr& msg)
{
	CPI01 = (msg->data & (int32_t)pow(2,0)) > 0;
	CPI02 = (msg->data & (int32_t)pow(2,1)) > 0;
	CPI03 = (msg->data & (int32_t)pow(2,2)) > 0;
	CPI04 = (msg->data & (int32_t)pow(2,3)) > 0;
	CPI05 = (msg->data & (int32_t)pow(2,4)) > 0;
	CPI06 = (msg->data & (int32_t)pow(2,5)) > 0;
	CPI07 = (msg->data & (int32_t)pow(2,6)) > 0;
	CPI08 = (msg->data & (int32_t)pow(2,7)) > 0;
}

int D01D,D01G,D02D,D02G,D03D,D03G,D04D,D04G,D05D,D05G,D06D,D06G,D07D,D07G,D08D,D08G,D09D,D09G,D10D,D10G,D11D,D11G,D12D,D12G;

void CapteurCallbackSwitch(const std_msgs::Int32::ConstPtr& msg)
{
	D01D = (msg->data & (int32_t)pow(2,0)) > 0;
	D01G = (msg->data & (int32_t)pow(2,1)) > 0;
	D02D = (msg->data & (int32_t)pow(2,2)) > 0;
	D02G = (msg->data & (int32_t)pow(2,3)) > 0;
	D03D = (msg->data & (int32_t)pow(2,4)) > 0;
	D03G = (msg->data & (int32_t)pow(2,5)) > 0;
	D04D = (msg->data & (int32_t)pow(2,6)) > 0;
	D04G = (msg->data & (int32_t)pow(2,7)) > 0;
	D05D = (msg->data & (int32_t)pow(2,8)) > 0;
	D05G = (msg->data & (int32_t)pow(2,9)) > 0;
	D06D = (msg->data & (int32_t)pow(2,10)) > 0;
	D06G = (msg->data & (int32_t)pow(2,11)) > 0;
	D07D = (msg->data & (int32_t)pow(2,12)) > 0;
	D07G = (msg->data & (int32_t)pow(2,13)) > 0;
	D08D = (msg->data & (int32_t)pow(2,14)) > 0;
	D08G = (msg->data & (int32_t)pow(2,15)) > 0;
	D09D = (msg->data & (int32_t)pow(2,16)) > 0;
	D09G = (msg->data & (int32_t)pow(2,17)) > 0;
	D10D = (msg->data & (int32_t)pow(2,18)) > 0;
	D10G = (msg->data & (int32_t)pow(2,19)) > 0;
	D11D = (msg->data & (int32_t)pow(2,20)) > 0;
	D11G = (msg->data & (int32_t)pow(2,21)) > 0;
	D12D = (msg->data & (int32_t)pow(2,22)) > 0;
	D12G = (msg->data & (int32_t)pow(2,23)) > 0;
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    cv::imshow("Simulation", cv_bridge::toCvShare(msg, "bgr8")->image);
}

void CapteurStateCallback(const std_msgs::Int32::ConstPtr& msg)
{
	// CP
	circle(image_Capteur, cv::Point(54, 359), 5, cv::Scalar(0,255*CP01,255-255*CP01), -1, 8 );
	circle(image_Capteur, cv::Point(95, 237), 5, cv::Scalar(0,255*CP02,255-255*CP02), -1, 8 );
    	circle(image_Capteur, cv::Point(446, 237), 5, cv::Scalar(0,255*CP03,255-255*CP03), -1, 8 );
	circle(image_Capteur, cv::Point(785, 348), 5, cv::Scalar(0,255*CP04,255-255*CP04), -1, 8 );
	circle(image_Capteur, cv::Point(814, 236), 5, cv::Scalar(0,255*CP05,255-255*CP05), -1, 8 );
	circle(image_Capteur, cv::Point(1005, 24), 5, cv::Scalar(0,255*CP06,255-255*CP06), -1, 8 );
	circle(image_Capteur, cv::Point(912, 136), 5, cv::Scalar(0,255*CP07,255-255*CP07), -1, 8 );
	circle(image_Capteur, cv::Point(577, 137), 5, cv::Scalar(0,255*CP08,255-255*CP08), -1, 8 );
	circle(image_Capteur, cv::Point(242, 27), 5, cv::Scalar(0,255*CP09,255-255*CP09), -1, 8 );
	circle(image_Capteur, cv::Point(182, 136), 5, cv::Scalar(0,255*CP10,255-255*CP10), -1, 8 );

	// PS
	circle(image_Capteur, cv::Point(15, 191), 5, cv::Scalar(0,255*PS01,255-255*PS01), -1, 8 );
	circle(image_Capteur, cv::Point(201, 237), 5, cv::Scalar(0,255*PS02,255-255*PS02), -1, 8 );
	circle(image_Capteur, cv::Point(244, 345), 5, cv::Scalar(0,255*PS03,255-255*PS03), -1, 8 );
	circle(image_Capteur, cv::Point(331, 237), 5, cv::Scalar(0,255*PS04,255-255*PS04), -1, 8 );
	circle(image_Capteur, cv::Point(573, 237), 5, cv::Scalar(0,255*PS05,255-255*PS05), -1, 8 );
	circle(image_Capteur, cv::Point(705, 236), 5, cv::Scalar(0,255*PS06,255-255*PS06), -1, 8 );
	circle(image_Capteur, cv::Point(944, 238), 5, cv::Scalar(0,255*PS07,255-255*PS07), -1, 8 );
	circle(image_Capteur, cv::Point(1004, 347), 5, cv::Scalar(0,255*PS08,255-255*PS08), -1, 8 );
	circle(image_Capteur, cv::Point(1008, 188),5, cv::Scalar(0,255*PS09,255-255*PS09), -1, 8 );
	circle(image_Capteur, cv::Point(847, 137), 5, cv::Scalar(0,255*PS10,255-255*PS10), -1, 8 );
	circle(image_Capteur, cv::Point(776, 29), 5, cv::Scalar(0,255*PS11,255-255*PS11), -1, 8 );
	circle(image_Capteur, cv::Point(706, 136), 5, cv::Scalar(0,255*PS12,255-255*PS12), -1, 8 );
	circle(image_Capteur, cv::Point(453, 136), 5, cv::Scalar(0,255*PS13,255-255*PS13), -1, 8 );
	circle(image_Capteur, cv::Point(316, 137), 5, cv::Scalar(0,255*PS14,255-255*PS14), -1, 8 );
	circle(image_Capteur, cv::Point(75, 137), 5, cv::Scalar(0,255*PS15,255-255*PS15), -1, 8 );
	circle(image_Capteur, cv::Point(21, 27), 5, cv::Scalar(0,255*PS16,255-255*PS16), -1, 8 );
    cv::imshow("EtatCapteurs", image_Capteur);
}

void SetSwitchPosition(ros::ServiceClient client_simRosSetJointTargetPosition, int handle, double angle)
{
		static vrep_common::simRosSetJointTargetPosition srv_SetJointTargetPosition;
		srv_SetJointTargetPosition.request.handle = handle;		
		srv_SetJointTargetPosition.request.targetPosition = angle*M_PI/180;			
		client_simRosSetJointTargetPosition.call(srv_SetJointTargetPosition);
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

			while(system("pidof -x vrep > /dev/null")) // Attente d'un process du nom de "vrep"
			sleep(2);
		
			//Initialisation du noeud
			ros::init(argc, argv, "simulation");
			ros::NodeHandle nh;

			///////// SUBSCRIBERS ////////
				// Image Streaming 
		 		cv::namedWindow("Simulation");
				cv::startWindowThread();
				image_transport::ImageTransport it(nh);
				image_transport::Subscriber subImage = it.subscribe("vrep/VisionSensorData", 1, imageCallback);

				// Image Streaming 
		 		cv::namedWindow("EtatCapteurs");
				cv::startWindowThread();
				image_Capteur = cv::imread("Schema_cellule.png",CV_LOAD_IMAGE_COLOR);
				
				// Sensors State
		 		ros::Subscriber subRailSensor = nh.subscribe("vrep/RailSensor", 1, CapteurCallbackRail);
		 		ros::Subscriber subStationSensor = nh.subscribe("vrep/StationSensor", 1, CapteurCallbackStation);
		 		ros::Subscriber subSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1, CapteurCallbackSwitch);
				ros::Subscriber subCapteurState = nh.subscribe("vrep/RailSensor",1, CapteurStateCallback);
			///////// VARIABLES ////////
				int handle_A[NB_AIGUILLAGE+1];
				bool put=true;
				std_msgs::Int32 JointNumber;

			////////// VREP HANDLE  ///////
				ros::ServiceClient client_getObjectHandle=nh.serviceClient<vrep_common::simRosGetObjectHandle>("/vrep/simRosGetObjectHandle");
				vrep_common::simRosGetObjectHandle srv_getObjectHandle;
				
				for(int i=1;i<=12;i++)
				{
					std::ostringstream i_string;
    					i_string << i;
					std::string ObjectName_i;
					ObjectName_i = "A" + i_string.str();

					srv_getObjectHandle.request.objectName=ObjectName_i;
					if ( client_getObjectHandle.call(srv_getObjectHandle))
					    handle_A[i] = srv_getObjectHandle.response.handle;
				}
				  
			////////// VREP SERVICES  ///////
				ros::ServiceClient client_simRosSetJointTargetPosition = 			nh.serviceClient<vrep_common::simRosSetJointTargetPosition>("/vrep/simRosSetJointTargetPosition");

			while (ros::ok())
			{
				ros::spinOnce();
				//CELLULE DROITE
				
				if (PS09 == 1 && D08D == 1)
				{	
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[6],120);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[4],120);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[5],-120);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[9],-120);
				}
				if (PS12 == 1 && D08D == 1)
				{
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[8],120);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[7],-120);
				}
				if (PS12 == 1 && D08G == 1)
				{
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[8],0);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[5],0);
				}
				if (PS10 == 1 && D08G == 1)
				{
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[4],0);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[9],0);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[6],0);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[7],0);
				}

				//CELLULE GAUCHE
				if (PS13 == 1 && D11D == 1)
				{
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[11],-120);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[12],120);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[1],-120);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[2],120);
				}
				if (PS14 == 1 && D10D == 1)
				{
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[10],120);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[3],-120);
				}
				if (PS04 == 1 && D02G == 1)
				{
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[11],0);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[12],0);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[1],0);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[2],0);
				}

				if (PS14 == 1 && D10G == 1)
				{
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[10],0);
					SetSwitchPosition(client_simRosSetJointTargetPosition,handle_A[3],0);
				}

			// test on stop control
				if ((PS05==1)&&(pub==true)){
					JointNumber.data = 7;
					StopController.publish(JointNumber);	
					pub=false;
					sleep(2);
					StopController.publish(JointNumber);		
				}

			}				

			cv::destroyWindow("view");
			system("pkill vrep");
			return 0;
		} 
}
