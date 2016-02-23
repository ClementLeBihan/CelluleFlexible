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
#include "vrep_common/simRosLoadModel.h"
#include "vrep_common/simRosPauseSimulation.h"
#include "vrep_common/simRosStartSimulation.h"
#include "std_msgs/String.h"

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>

#include <ros/ros.h>

#define NB_AIGUILLAGE 12

cv::Mat image_Capteur;

int PS[17],CPI[9],CP[11],DD[13],DG[13];

int numSwitch, direction, oldNumSwitch(0), oldDirection(0); 
int handle_A[NB_AIGUILLAGE+1];
std_msgs::Int32 sensorRail, sensorStation,StopNumber;

int handle_Shuttle[7];
static int n_shuttle=0;

// Fonction qui stocke dans capteur 1 la valeur du capteur (0 ou 1)
void CapteurCallbackRail(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=16;i++) PS[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
	for(int i=1;i<=10;i++) CP[i] = (msg->data & (int32_t)pow(2,15+i)) > 0;
	sensorRail.data = msg->data ; 

}

void CapteurCallbackStation(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=8;i++) CPI[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
	sensorStation.data = msg->data ; 
}

void CapteurCallbackSwitch(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=12;i++){
		DD[i] = (msg->data & (int32_t)pow(2,2*i-2)) > 0;
		DG[i] = (msg->data & (int32_t)pow(2,2*i-1)) > 0;
	}
}


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    cv::imshow("Simulation", cv_bridge::toCvShare(msg, "bgr8")->image);
}

void CapteurStateCallback(const std_msgs::Int32::ConstPtr& msg)
{
	// CP
	circle(image_Capteur, cv::Point(45, 349), 5, cv::Scalar(0,255*CP[1],255-255*CP[1]), -1, 8 );
	circle(image_Capteur, cv::Point(122, 231), 5, cv::Scalar(0,255*CP[2],255-255*CP[2]), -1, 8 );
    	circle(image_Capteur, cv::Point(447, 231), 5, cv::Scalar(0,255*CP[3],255-255*CP[3]), -1, 8 );
	circle(image_Capteur, cv::Point(788, 348), 5, cv::Scalar(0,255*CP[4],255-255*CP[4]), -1, 8 );
	circle(image_Capteur, cv::Point(806, 230), 5, cv::Scalar(0,255*CP[5],255-255*CP[5]), -1, 8 );
	circle(image_Capteur, cv::Point(981, 18), 5, cv::Scalar(0,255*CP[6],255-255*CP[6]), -1, 8 );
	circle(image_Capteur, cv::Point(878, 135), 5, cv::Scalar(0,255*CP[7],255-255*CP[7]), -1, 8 );
	circle(image_Capteur, cv::Point(576, 135), 5, cv::Scalar(0,255*CP[8],255-255*CP[8]), -1, 8 );
	circle(image_Capteur, cv::Point(236, 16), 5, cv::Scalar(0,255*CP[9],255-255*CP[9]), -1, 8 );
	circle(image_Capteur, cv::Point(190, 135), 5, cv::Scalar(0,255*CP[10],255-255*CP[10]), -1, 8 );

	// PS
	circle(image_Capteur, cv::Point(27, 181), 5, cv::Scalar(0,255*PS[1],255-255*PS[1]), -1, 8 );
	circle(image_Capteur, cv::Point(241, 230), 5, cv::Scalar(0,255*PS[2],255-255*PS[2]), -1, 8 );
	circle(image_Capteur, cv::Point(248, 342), 5, cv::Scalar(0,255*PS[3],255-255*PS[3]), -1, 8 );
	circle(image_Capteur, cv::Point(326, 230), 5, cv::Scalar(0,255*PS[4],255-255*PS[4]), -1, 8 );
	circle(image_Capteur, cv::Point(572, 231), 5, cv::Scalar(0,255*PS[5],255-255*PS[5]), -1, 8 );
	circle(image_Capteur, cv::Point(700, 231), 5, cv::Scalar(0,255*PS[6],255-255*PS[6]), -1, 8 );
	circle(image_Capteur, cv::Point(934, 232), 5, cv::Scalar(0,255*PS[7],255-255*PS[7]), -1, 8 );
	circle(image_Capteur, cv::Point(992, 339), 5, cv::Scalar(0,255*PS[8],255-255*PS[8]), -1, 8 );
	circle(image_Capteur, cv::Point(995, 189),5, cv::Scalar(0,255*PS[9],255-255*PS[9]), -1, 8 );
	circle(image_Capteur, cv::Point(783, 135), 5, cv::Scalar(0,255*PS[10],255-255*PS[10]), -1, 8 );
	circle(image_Capteur, cv::Point(770, 28), 5, cv::Scalar(0,255*PS[11],255-255*PS[11]), -1, 8 );
	circle(image_Capteur, cv::Point(701, 136), 5, cv::Scalar(0,255*PS[12],255-255*PS[12]), -1, 8 );
	circle(image_Capteur, cv::Point(454, 135), 5, cv::Scalar(0,255*PS[13],255-255*PS[13]), -1, 8 );
	circle(image_Capteur, cv::Point(321, 135), 5, cv::Scalar(0,255*PS[14],255-255*PS[14]), -1, 8 );
	circle(image_Capteur, cv::Point(66, 135), 5, cv::Scalar(0,255*PS[15],255-255*PS[15]), -1, 8 );
	circle(image_Capteur, cv::Point(29, 26), 5, cv::Scalar(0,255*PS[16],255-255*PS[16]), -1, 8 );

	// Aiguillages
	circle(image_Capteur, cv::Point(21, 241), 16, cv::Scalar(255,255,255), -1); //A01
	circle(image_Capteur, cv::Point(270, 253), 16, cv::Scalar(255,255,255), -1); //A02
	circle(image_Capteur, cv::Point(378, 235), 16, cv::Scalar(255,255,255), -1); //A03
	circle(image_Capteur, cv::Point(642, 235), 16, cv::Scalar(255,255,255), -1); //A04
	circle(image_Capteur, cv::Point(751, 253), 16, cv::Scalar(255,255,255), -1); //A05
	circle(image_Capteur, cv::Point(1002, 240), 16, cv::Scalar(255,255,255), -1); //A06
	circle(image_Capteur, cv::Point(1000, 127), 16, cv::Scalar(255,255,255), -1); //A07
	circle(image_Capteur, cv::Point(752, 112), 16, cv::Scalar(255,255,255), -1); //A08
	circle(image_Capteur, cv::Point(645, 127), 16, cv::Scalar(255,255,255), -1); //A09
	circle(image_Capteur, cv::Point(377, 129), 16, cv::Scalar(255,255,255), -1); //A10
	circle(image_Capteur, cv::Point(269, 113), 16, cv::Scalar(255,255,255), -1); //A11
	circle(image_Capteur, cv::Point(21, 125), 16, cv::Scalar(255,255,255), -1); //A12

	if(DD[1]) line(image_Capteur, cv::Point(14, 113), cv::Point(14, 137), cv::Scalar(0,100,0), 3); //A01
	if(DG[1]) ellipse(image_Capteur, cv::Point(37, 220),cv::Size(23,23),0,92,163,cv::Scalar(0,100,0), 3);
	if(DD[2]) ellipse(image_Capteur, cv::Point(290, 268),cv::Size(23,23),0,183,255,cv::Scalar(0,100,0), 3);//A02
	if(DG[2]) line(image_Capteur, cv::Point(257, 245), cv::Point(283, 245), cv::Scalar(0,100,0), 3);
	if(DD[3]) line(image_Capteur, cv::Point(366, 243), cv::Point(390, 243), cv::Scalar(0,100,0), 3); //A03
	if(DG[3]) ellipse(image_Capteur, cv::Point(358, 221),cv::Size(23,23),0,73,1,cv::Scalar(0,100,0), 3);
	if(DD[4]) line(image_Capteur, cv::Point(629, 243), cv::Point(654, 243), cv::Scalar(0,100,0), 3); //A04
	if(DG[4]) ellipse(image_Capteur, cv::Point(665, 220),cv::Size(23,23),0,113,181,cv::Scalar(0,100,0), 3);
	if(DD[5]) ellipse(image_Capteur, cv::Point(732, 265),cv::Size(23,23),0,284,363,cv::Scalar(0,100,0), 3);
	if(DG[5]) line(image_Capteur, cv::Point(738, 245), cv::Point(764, 245), cv::Scalar(0,100,0), 3); //A05
	if(DD[6]) line(image_Capteur, cv::Point(1008, 226), cv::Point(1008, 252), cv::Scalar(0,100,0), 3); //A06
	if(DG[6]) ellipse(image_Capteur, cv::Point(985, 221),cv::Size(23,23),0,83,12,cv::Scalar(0,100,0), 3);
	if(DD[7]) line(image_Capteur, cv::Point(1008, 114), cv::Point(1008, 138), cv::Scalar(0,100,0), 3); //A07
	if(DG[7]) ellipse(image_Capteur, cv::Point(985, 145),cv::Size(23,23),0,271,348,cv::Scalar(0,100,0), 3);
	if(DD[8]) ellipse(image_Capteur, cv::Point(733, 96),cv::Size(23,23),0,75,3,cv::Scalar(0,100,0), 3);//A08
	if(DG[8]) line(image_Capteur, cv::Point(740, 121), cv::Point(764, 121), cv::Scalar(0,100,0), 3); 
	if(DD[9]) line(image_Capteur, cv::Point(631, 121), cv::Point(659, 121), cv::Scalar(0,100,0), 3); //A09
	if(DG[9]) ellipse(image_Capteur, cv::Point(665, 143),cv::Size(23,23),0,184,256,cv::Scalar(0,100,0), 3);
	if(DD[10]) line(image_Capteur, cv::Point(364, 122), cv::Point(390, 122), cv::Scalar(0,100,0), 3); //A10
	if(DG[10]) ellipse(image_Capteur, cv::Point(358, 144),cv::Size(23,23),0,282,357,cv::Scalar(0,100,0), 3);
	if(DD[11]) ellipse(image_Capteur, cv::Point(290, 98),cv::Size(23,23),0,110,183,cv::Scalar(0,100,0), 3); //A11
	if(DG[11]) line(image_Capteur, cv::Point(257, 121), cv::Point(281, 121), cv::Scalar(0,100,0), 3);
	if(DD[12]) line(image_Capteur, cv::Point(14, 228), cv::Point(14, 253), cv::Scalar(0,100,0), 3); //A12
	if(DG[12]) ellipse(image_Capteur, cv::Point(38, 145),cv::Size(23,23),0,194,268,cv::Scalar(0,100,0), 3);

    cv::imshow("EtatCapteurs", image_Capteur);
}

void LoadModel(ros::ServiceClient client_simRosLoadModel, std::string model_name)
{
	static vrep_common::simRosLoadModel srv_LoadModel;
	srv_LoadModel.request.fileName = model_name;			
	client_simRosLoadModel.call(srv_LoadModel);
}

void StateSwitchCallBack(const std_msgs::String::ConstPtr&  msg)
{
	string result = msg->data.c_str();
	std::size_t found = result.find(" ");
	numSwitch = atoi(result.substr(0,found).c_str()) ; 
	direction = atoi(result.substr(found+1).c_str());
}

void StateStopCallBack(const std_msgs::Int32::ConstPtr&  msg)
{
	StopNumber.data = msg->data;
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
				system("rosservice call /vrep/simRosStartSimulation");
				ROS_INFO("Reprise de la  simulation");

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
				ros::Subscriber subSwitchState = nh.subscribe("tp_etudiant/switchState", 1, StateSwitchCallBack);


			///////// PUBLISHERS ////////
				ros::Publisher StopController = nh.advertise<std_msgs::Int32>("/simulation/StopController", 1);
				ros::Publisher railSensorState = nh.advertise<std_msgs::Int32>("/simulation/RailSensorState", 1);
				ros::Publisher stationSensorState = nh.advertise<std_msgs::Int32>("/simulation/StationSensorState", 1);
				ros::Publisher SwitchController = nh.advertise<std_msgs::Int32>("/simulation/SwitchController", 1);

			///////// VARIABLES ////////
				bool pub=true;
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
				ros::ServiceClient client_simRosSetJointTargetPosition = nh.serviceClient<vrep_common::simRosSetJointTargetPosition>("/vrep/simRosSetJointTargetPosition");

			while (ros::ok())
			{
				ros::spinOnce();				
				railSensorState.publish(sensorRail);
				stationSensorState.publish(sensorStation);

				//Pour modifier la position d'un aiguillage, il suffit de faire SwitchController.publish(SwitchNumber) en stockant dans SwitchNumber.data la somme des puissances de 2 des numéros d'aiguillages-1 (pour tourner aiguillage 2 et 3, on fait SwitchNumber.data = pow(2,2-1) + pow(2,3-1)
				
				//Pour modifier la position d'un stop, faire pareil mais avec StopController.publish(StopNumber)

			}				

			cv::destroyWindow("view");
			system("pkill vrep");
			return 0;
		} 
}
