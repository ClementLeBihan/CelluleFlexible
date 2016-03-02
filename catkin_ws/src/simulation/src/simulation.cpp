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
#include "opencv2/highgui/highgui.hpp"

#include <cv_bridge/cv_bridge.h>

// Used data structures:
#include "vrep_common/VrepInfo.h"
#include "vrep_common/simRosLoadModel.h"
#include "vrep_common/simRosPauseSimulation.h"
#include "vrep_common/simRosStartSimulation.h"

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

// Msg
#include <std_msgs/Int32.h>
#include <simulation/Msg_SwitchControl.h>
#include <simulation/Msg_StopControl.h>
#include <simulation/Msg_SensorState.h>

#include <vector>

#include <ros/ros.h>

#define NB_AIGUILLAGE 12

cv::Mat imageSensor = cv::imread("img/Schema_cellule.png",CV_LOAD_IMAGE_COLOR);
cv::Mat imageSimu = cv::Mat::zeros(512, 1024, CV_8UC3 );
cv::Mat imageTot = cv::Mat::zeros(950, 1180, CV_8UC3 );
cv::Mat playButton = cv::imread("img/PlayButton.png",CV_LOAD_IMAGE_COLOR);
cv::Mat playButton_Down = cv::imread("img/PlayButton_Down.png",CV_LOAD_IMAGE_COLOR);
cv::Mat playButton_On = cv::imread("img/PlayButton_On.png",CV_LOAD_IMAGE_COLOR);
cv::Mat pauseButton = cv::imread("img/PauseButton.png",CV_LOAD_IMAGE_COLOR);
cv::Mat pauseButton_Down = cv::imread("img/PauseButton_Down.png",CV_LOAD_IMAGE_COLOR);
cv::Mat pauseButton_On = cv::imread("img/PauseButton_On.png",CV_LOAD_IMAGE_COLOR);

simulation::Msg_StopControl StopControl;
simulation::Msg_SwitchControl SwitchControl;
simulation::Msg_SensorState SensorState;

ros::ServiceClient client_simRosStartSimulation,client_simRosPauseSimulation; 

ros::Publisher VREPSwitchControllerRight, VREPSwitchControllerLeft, VREPSwitchControllerLock,  VREPStopController, VREPGoController;

int mode = 0;

void updateUI()
{
	imageSimu.copyTo(imageTot.rowRange(18,530).colRange(78,1102));
  imageSensor.copyTo(imageTot.rowRange(570,936).colRange(78,1102));
			
  cv::imshow("Simulation", imageTot);
}

// Fonction qui stocke dans sensor 1 la valeur du sensor (0 ou 1)
void SensorCallbackRail(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=16;i++) SensorState.PS[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
	for(int i=1;i<=10;i++) SensorState.CP[i] = (msg->data & (int32_t)pow(2,15+i)) > 0;
}

void SensorCallbackStation(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=8;i++) SensorState.CPI[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
}

void SensorCallbackSwitch(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=12;i++){
		SensorState.DD[i] = (msg->data & (int32_t)pow(2,2*i-2)) > 0;
		SensorState.DG[i] = (msg->data & (int32_t)pow(2,2*i-1)) > 0;
	}
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	imageSimu = cv_bridge::toCvShare(msg, "bgr8")->image;
	updateUI();
}
void onMouse( int event, int x, int y, int, void* )
{

	static vrep_common::simRosStartSimulation srv_StartSimulation;		
	static vrep_common::simRosPauseSimulation srv_PauseSimulation;

	switch(event) {
		case cv::EVENT_LBUTTONDOWN :
   			if(y>545 && y < 570 && x>453 && x<563 && mode!= 1) {
					playButton_Down.copyTo(imageTot.rowRange(545,570).colRange(453,563));
					pauseButton.copyTo(imageTot.rowRange(545,570).colRange(617,727));
					client_simRosStartSimulation.call(srv_StartSimulation);
					mode = 1; }
    		else if(y>545 && y < 570 && x>617 && x<727 && mode !=0) {
					playButton.copyTo(imageTot.rowRange(545,570).colRange(453,563));
					pauseButton_Down.copyTo(imageTot.rowRange(545,570).colRange(617,727));
					client_simRosPauseSimulation.call(srv_PauseSimulation);
					mode = 0; }
		break;
		case cv::EVENT_MOUSEMOVE :
   			if(y>545 && y < 570 && x>453 && x<563 && mode!= 1)
					playButton_On.copyTo(imageTot.rowRange(545,570).colRange(453,563));
				else if (mode!= 1)
					playButton.copyTo(imageTot.rowRange(545,570).colRange(453,563));
    		if(y>545 && y < 570 && x>617 && x<727 && mode !=0)
					pauseButton_On.copyTo(imageTot.rowRange(545,570).colRange(617,727));
				else if (mode!= 0)
					pauseButton.copyTo(imageTot.rowRange(545,570).colRange(617,727));
		break;
	}
}

void SensorStateCallback(const std_msgs::Int32::ConstPtr& msg)
{
	// CP
	circle(imageSensor, cv::Point(45, 349), 5, cv::Scalar(0,255*SensorState.CP[1],255-255*SensorState.CP[1]), -1, 8 );
	circle(imageSensor, cv::Point(122, 231), 5, cv::Scalar(0,255*SensorState.CP[2],255-255*SensorState.CP[2]), -1, 8 );
  circle(imageSensor, cv::Point(447, 231), 5, cv::Scalar(0,255*SensorState.CP[3],255-255*SensorState.CP[3]), -1, 8 );
	circle(imageSensor, cv::Point(788, 348), 5, cv::Scalar(0,255*SensorState.CP[4],255-255*SensorState.CP[4]), -1, 8 );
	circle(imageSensor, cv::Point(806, 230), 5, cv::Scalar(0,255*SensorState.CP[5],255-255*SensorState.CP[5]), -1, 8 );
	circle(imageSensor, cv::Point(981, 18), 5, cv::Scalar(0,255*SensorState.CP[6],255-255*SensorState.CP[6]), -1, 8 );
	circle(imageSensor, cv::Point(878, 135), 5, cv::Scalar(0,255*SensorState.CP[7],255-255*SensorState.CP[7]), -1, 8 );
	circle(imageSensor, cv::Point(576, 135), 5, cv::Scalar(0,255*SensorState.CP[8],255-255*SensorState.CP[8]), -1, 8 );
	circle(imageSensor, cv::Point(236, 16), 5, cv::Scalar(0,255*SensorState.CP[9],255-255*SensorState.CP[9]), -1, 8 );
	circle(imageSensor, cv::Point(190, 135), 5, cv::Scalar(0,255*SensorState.CP[10],255-255*SensorState.CP[10]), -1, 8 );

	// CPI
	circle(imageSensor, cv::Point(100, 346), 5, cv::Scalar(0,255*SensorState.CPI[1],255-255*SensorState.CPI[1]), -1, 8 );
	circle(imageSensor, cv::Point(185, 346), 5, cv::Scalar(0,255*SensorState.CPI[2],255-255*SensorState.CPI[2]), -1, 8 );
  circle(imageSensor, cv::Point(847, 345), 5, cv::Scalar(0,255*SensorState.CPI[3],255-255*SensorState.CPI[3]), -1, 8 );
	circle(imageSensor, cv::Point(931, 345), 5, cv::Scalar(0,255*SensorState.CPI[4],255-255*SensorState.CPI[4]), -1, 8 );
	circle(imageSensor, cv::Point(925, 21), 5, cv::Scalar(0,255*SensorState.CPI[5],255-255*SensorState.CPI[5]), -1, 8 );
	circle(imageSensor, cv::Point(841, 22), 5, cv::Scalar(0,255*SensorState.CPI[6],255-255*SensorState.CPI[6]), -1, 8 );
	circle(imageSensor, cv::Point(183, 19), 5, cv::Scalar(0,255*SensorState.CPI[7],255-255*SensorState.CPI[7]), -1, 8 );
	circle(imageSensor, cv::Point(97, 20), 5, cv::Scalar(0,255*SensorState.CPI[8],255-255*SensorState.CPI[8]), -1, 8 );

	// PS
	circle(imageSensor, cv::Point(27, 181), 5, cv::Scalar(0,255*SensorState.PS[1],255-255*SensorState.PS[1]), -1, 8 );
	circle(imageSensor, cv::Point(241, 230), 5, cv::Scalar(0,255*SensorState.PS[2],255-255*SensorState.PS[2]), -1, 8 );
	circle(imageSensor, cv::Point(248, 342), 5, cv::Scalar(0,255*SensorState.PS[3],255-255*SensorState.PS[3]), -1, 8 );
	circle(imageSensor, cv::Point(326, 230), 5, cv::Scalar(0,255*SensorState.PS[4],255-255*SensorState.PS[4]), -1, 8 );
	circle(imageSensor, cv::Point(572, 231), 5, cv::Scalar(0,255*SensorState.PS[5],255-255*SensorState.PS[5]), -1, 8 );
	circle(imageSensor, cv::Point(700, 231), 5, cv::Scalar(0,255*SensorState.PS[6],255-255*SensorState.PS[6]), -1, 8 );
	circle(imageSensor, cv::Point(934, 232), 5, cv::Scalar(0,255*SensorState.PS[7],255-255*SensorState.PS[7]), -1, 8 );
	circle(imageSensor, cv::Point(992, 339), 5, cv::Scalar(0,255*SensorState.PS[8],255-255*SensorState.PS[8]), -1, 8 );
	circle(imageSensor, cv::Point(995, 189),5, cv::Scalar(0,255*SensorState.PS[9],255-255*SensorState.PS[9]), -1, 8 );
	circle(imageSensor, cv::Point(783, 135), 5, cv::Scalar(0,255*SensorState.PS[10],255-255*SensorState.PS[10]), -1, 8 );
	circle(imageSensor, cv::Point(770, 28), 5, cv::Scalar(0,255*SensorState.PS[11],255-255*SensorState.PS[11]), -1, 8 );
	circle(imageSensor, cv::Point(701, 136), 5, cv::Scalar(0,255*SensorState.PS[12],255-255*SensorState.PS[12]), -1, 8 );
	circle(imageSensor, cv::Point(454, 135), 5, cv::Scalar(0,255*SensorState.PS[13],255-255*SensorState.PS[13]), -1, 8 );
	circle(imageSensor, cv::Point(321, 135), 5, cv::Scalar(0,255*SensorState.PS[14],255-255*SensorState.PS[14]), -1, 8 );
	circle(imageSensor, cv::Point(66, 135), 5, cv::Scalar(0,255*SensorState.PS[15],255-255*SensorState.PS[15]), -1, 8 );
	circle(imageSensor, cv::Point(29, 26), 5, cv::Scalar(0,255*SensorState.PS[16],255-255*SensorState.PS[16]), -1, 8 );

	// Aiguillages
	circle(imageSensor, cv::Point(21, 241), 17, cv::Scalar(200,200,200), -1); //A01
	circle(imageSensor, cv::Point(270, 252), 17, cv::Scalar(200,200,200), -1); //A02
	circle(imageSensor, cv::Point(378, 235), 17, cv::Scalar(200,200,200), -1); //A03
	circle(imageSensor, cv::Point(642, 235), 17, cv::Scalar(200,200,200), -1); //A04
	circle(imageSensor, cv::Point(751, 252), 17, cv::Scalar(200,200,200), -1); //A05
	circle(imageSensor, cv::Point(1002, 240), 17, cv::Scalar(200,200,200), -1); //A06
	circle(imageSensor, cv::Point(1000, 126), 17, cv::Scalar(200,200,200), -1); //A07
	circle(imageSensor, cv::Point(752, 111), 17, cv::Scalar(200,200,200), -1); //A08
	circle(imageSensor, cv::Point(645, 126), 17, cv::Scalar(200,200,200), -1); //A09
	circle(imageSensor, cv::Point(377, 129), 17, cv::Scalar(200,200,200), -1); //A10
	circle(imageSensor, cv::Point(269, 112), 17, cv::Scalar(200,200,200), -1); //A11
	circle(imageSensor, cv::Point(22, 125), 17, cv::Scalar(200,200,200), -1); //A12

	if(SensorState.DD[1]) line(imageSensor, cv::Point(14, 227), cv::Point(14, 255), cv::Scalar(0,100,0), 3); //A01
	if(SensorState.DG[1]) ellipse(imageSensor, cv::Point(37, 220),cv::Size(23,23),0,92,163,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[2]) ellipse(imageSensor, cv::Point(290, 268),cv::Size(23,23),0,183,255,cv::Scalar(0,100,0), 3);//A02
	if(SensorState.DG[2]) line(imageSensor, cv::Point(257, 245), cv::Point(283, 245), cv::Scalar(0,100,0), 3);
	if(SensorState.DD[3]) line(imageSensor, cv::Point(365, 243), cv::Point(391, 243), cv::Scalar(0,100,0), 3); //A03
	if(SensorState.DG[3]) ellipse(imageSensor, cv::Point(358, 221),cv::Size(23,23),0,73,1,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[4]) line(imageSensor, cv::Point(629, 243), cv::Point(655, 243), cv::Scalar(0,100,0), 3); //A04
	if(SensorState.DG[4]) ellipse(imageSensor, cv::Point(665, 220),cv::Size(23,23),0,113,181,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[5]) ellipse(imageSensor, cv::Point(732, 265),cv::Size(23,23),0,284,363,cv::Scalar(0,100,0), 3);//A05
	if(SensorState.DG[5]) line(imageSensor, cv::Point(738, 245), cv::Point(764, 245), cv::Scalar(0,100,0), 3); 
	if(SensorState.DD[6]) line(imageSensor, cv::Point(1008, 226), cv::Point(1008, 253), cv::Scalar(0,100,0), 3); //A06
	if(SensorState.DG[6]) ellipse(imageSensor, cv::Point(985, 221),cv::Size(23,23),0,83,12,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[7]) line(imageSensor, cv::Point(1008, 113), cv::Point(1008, 139), cv::Scalar(0,100,0), 3); //A07
	if(SensorState.DG[7]) ellipse(imageSensor, cv::Point(985, 145),cv::Size(23,23),0,271,348,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[8]) ellipse(imageSensor, cv::Point(733, 96),cv::Size(23,23),0,75,3,cv::Scalar(0,100,0), 3);//A08
	if(SensorState.DG[8]) line(imageSensor, cv::Point(740, 121), cv::Point(764, 121), cv::Scalar(0,100,0), 3); 
	if(SensorState.DD[9]) line(imageSensor, cv::Point(631, 121), cv::Point(659, 121), cv::Scalar(0,100,0), 3); //A09
	if(SensorState.DG[9]) ellipse(imageSensor, cv::Point(665, 143),cv::Size(23,23),0,184,256,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[10]) line(imageSensor, cv::Point(363, 122), cv::Point(390, 122), cv::Scalar(0,100,0), 3); //A10
	if(SensorState.DG[10]) ellipse(imageSensor, cv::Point(358, 144),cv::Size(23,23),0,282,357,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[11]) ellipse(imageSensor, cv::Point(290, 98),cv::Size(23,23),0,110,183,cv::Scalar(0,100,0), 3); //A11
	if(SensorState.DG[11]) line(imageSensor, cv::Point(257, 121), cv::Point(281, 121), cv::Scalar(0,100,0), 3);
	if(SensorState.DD[12]) line(imageSensor, cv::Point(14, 112), cv::Point(14, 138), cv::Scalar(0,100,0), 3); //A12
	if(SensorState.DG[12]) ellipse(imageSensor, cv::Point(38, 145),cv::Size(23,23),0,194,268,cv::Scalar(0,100,0), 3);
    updateUI();
}

void LoadModel(ros::ServiceClient client_simRosLoadModel, std::string model_name)
{
	static vrep_common::simRosLoadModel srv_LoadModel;
	srv_LoadModel.request.fileName = model_name;			
	client_simRosLoadModel.call(srv_LoadModel);
}

void StateSwitchCallBack(const simulation::Msg_SwitchControl::ConstPtr&  msg)
{
	int SwitchRightControl(0), SwitchLeftControl(0), SwitchLockControl(0);
	std_msgs::Int32 Right, Left, Lock;

	SwitchControl.RD = msg->RD;
	SwitchControl.RG = msg->RG;
	SwitchControl.LOCK = msg->LOCK;

	for (int i=1;i<=12;i++){
		if (msg->RD[i]==true)
			SwitchRightControl+=pow(2,i-1);
		if (msg->RG[i]==true)
			SwitchLeftControl+=pow(2,i-1);
		if (msg->LOCK[i]==true)
			SwitchLockControl+=pow(2,i-1);
	}

	Right.data=SwitchRightControl;
	VREPSwitchControllerRight.publish(Right);
	Left.data=SwitchLeftControl;
	VREPSwitchControllerLeft.publish(Left);
	Lock.data=SwitchLockControl;
	VREPSwitchControllerLock.publish(Lock);
}


void StateStopCallBack(const simulation::Msg_StopControl::ConstPtr&  msg)
{
	int StopControlInt(0), GoControlInt(0);
	std_msgs::Int32 Stop, Go;

	StopControl.STOP = msg->STOP;
	StopControl.GO = msg->GO;

	for (int i=1;i<=24;i++){
		if (msg->STOP[i]==true)
			StopControlInt+=pow(2,i-1);
		if (msg->GO[i]==true)
			GoControlInt+=pow(2,i-1);
	}

	Stop.data=StopControlInt;
	VREPStopController.publish(Stop);
	Go.data=GoControlInt;
	VREPGoController.publish(Go);
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

			  imageTot.setTo(cv::Scalar(200,200,200));
			  rectangle(imageTot, cv::Point(75,15), cv::Point(1105,533),cv::Scalar(100,100,100),2);
			  playButton.copyTo(imageTot.rowRange(545,570).colRange(453,563));
			  pauseButton_Down.copyTo(imageTot.rowRange(545,570).colRange(617,727));

			  client_simRosStartSimulation = nh.serviceClient<vrep_common::simRosStartSimulation>("/vrep/simRosStartSimulation");	
			  client_simRosPauseSimulation = nh.serviceClient<vrep_common::simRosPauseSimulation>("/vrep/simRosPauseSimulation");	

			///////// SUBSCRIBERS ////////
			  // Image Streaming 
	   		cv::namedWindow("Simulation");
			  cv::startWindowThread();
			  cv::moveWindow("Simulation",0,0);
			  cv::setMouseCallback("Simulation", onMouse,0);
			  image_transport::ImageTransport it(nh);
			  image_transport::Subscriber subImage = it.subscribe("vrep/VisionSensorData", 1, imageCallback);

			  // Sensors State
	   		ros::Subscriber VREPsubRailSensor = nh.subscribe("vrep/RailSensor", 1, SensorCallbackRail);
	   		ros::Subscriber VREPsubStationSensor = nh.subscribe("vrep/StationSensor", 1, SensorCallbackStation);
	   		ros::Subscriber VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1, SensorCallbackSwitch);
			  ros::Subscriber VREPsubSensorState = nh.subscribe("vrep/RailSensor",1, SensorStateCallback);

			///////// VREP PUBLISHERS ////////
			  VREPSwitchControllerRight = nh.advertise<std_msgs::Int32>("/simulation/SwitchControllerRight", 1);
			  VREPSwitchControllerLeft = nh.advertise<std_msgs::Int32>("/simulation/SwitchControllerLeft", 1);
			  VREPSwitchControllerLock = nh.advertise<std_msgs::Int32>("/simulation/SwitchControllerLock", 1);
			  VREPStopController = nh.advertise<std_msgs::Int32>("/simulation/StopController", 1);
			  VREPGoController = nh.advertise<std_msgs::Int32>("/simulation/GoController", 1);

			///////// TP PUBLISHERS ////////
			  ros::Publisher TPrailSensorState = nh.advertise<simulation::Msg_SensorState>("/simulation/SensorState", 1);

			///////// TP_ETUDIANT SUBSCRIBER ////////
			ros::Subscriber TPsubSwitchState = nh.subscribe("tp_etudiant/TPSwitchControl", 1, StateSwitchCallBack);
			ros::Subscriber TPsubStopState = nh.subscribe("tp_etudiant/TPStopControl", 1, StateStopCallBack);

			while (ros::ok())
			{
				ros::spinOnce();
				TPrailSensorState.publish(SensorState);
			}				
				

			cv::destroyWindow("view");
			system("pkill vrep");
			return 0;
		} 
}
