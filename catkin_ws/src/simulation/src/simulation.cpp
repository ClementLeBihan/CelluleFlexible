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
#include "vrep_common/simRosGetObjectHandle.h"
#include "vrep_common/simRosSetJointTargetPosition.h"
#include "vrep_common/simRosLoadModel.h"
#include "vrep_common/simRosPauseSimulation.h"
#include "vrep_common/simRosStartSimulation.h"
#include "std_msgs/String.h"

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"

// Msg
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <simulation/Msg_SwitchControl.h>
#include <simulation/Msg_StopControl.h>
#include <simulation/Msg_SensorState.h>


#include <ros/ros.h>

#define NB_AIGUILLAGE 12

cv::Mat imageCapteur = cv::imread("Schema_cellule.png",CV_LOAD_IMAGE_COLOR);
cv::Mat imageSimu = cv::Mat::zeros(512, 1024, CV_8UC3 );
cv::Mat imageTot = cv::Mat::zeros(960, 1180, CV_8UC3 );
cv::Mat playButton = cv::imread("PlayButton.png",CV_LOAD_IMAGE_COLOR);
cv::Mat pauseButton = cv::imread("PauseButton.png",CV_LOAD_IMAGE_COLOR);

int numSwitch, direction, oldNumSwitch(0), oldDirection(0); 
int handle_A[NB_AIGUILLAGE+1];

simulation::Msg_StopControl StopControl;
simulation::Msg_SwitchControl SwitchControl;
simulation::Msg_SensorState SensorState;

int handle_Shuttle[7];
static int n_shuttle=0;


ros::Publisher VREPSwitchControllerRight, VREPSwitchControllerLeft, VREPSwitchControllerLock,  VREPStopController, VREPGoController;

void updateUI()
{
	imageSimu.copyTo(imageTot.rowRange(70,582).colRange(78,1102));
	imageCapteur.copyTo(imageTot.rowRange(590,956).colRange(78,1102));
    	cv::imshow("Simulation", imageTot);
}

// Fonction qui stocke dans capteur 1 la valeur du capteur (0 ou 1)
void CapteurCallbackRail(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=16;i++) SensorState.PS[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
	for(int i=1;i<=10;i++) SensorState.CP[i] = (msg->data & (int32_t)pow(2,15+i)) > 0;

}

void CapteurCallbackStation(const std_msgs::Int32::ConstPtr& msg)
{
	for(int i=1;i<=8;i++) SensorState.CPI[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;
}

void CapteurCallbackSwitch(const std_msgs::Int32::ConstPtr& msg)
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
    if( event != cv::EVENT_LBUTTONDOWN )
        return;
	if(y>25 && y < 82){
		if(x>364 && x<537) system("rosservice call /vrep/simRosStartSimulation > /dev/null");
		else if(x>564 && x<737) system("rosservice call /vrep/simRosPauseSimulation > /dev/null");
	}
}

void CapteurStateCallback(const std_msgs::Int32::ConstPtr& msg)
{
	// CP
	circle(imageCapteur, cv::Point(45, 349), 5, cv::Scalar(0,255*SensorState.CP[1],255-255*SensorState.CP[1]), -1, 8 );
	circle(imageCapteur, cv::Point(122, 231), 5, cv::Scalar(0,255*SensorState.CP[2],255-255*SensorState.CP[2]), -1, 8 );
    	circle(imageCapteur, cv::Point(447, 231), 5, cv::Scalar(0,255*SensorState.CP[3],255-255*SensorState.CP[3]), -1, 8 );
	circle(imageCapteur, cv::Point(788, 348), 5, cv::Scalar(0,255*SensorState.CP[4],255-255*SensorState.CP[4]), -1, 8 );
	circle(imageCapteur, cv::Point(806, 230), 5, cv::Scalar(0,255*SensorState.CP[5],255-255*SensorState.CP[5]), -1, 8 );
	circle(imageCapteur, cv::Point(981, 18), 5, cv::Scalar(0,255*SensorState.CP[6],255-255*SensorState.CP[6]), -1, 8 );
	circle(imageCapteur, cv::Point(878, 135), 5, cv::Scalar(0,255*SensorState.CP[7],255-255*SensorState.CP[7]), -1, 8 );
	circle(imageCapteur, cv::Point(576, 135), 5, cv::Scalar(0,255*SensorState.CP[8],255-255*SensorState.CP[8]), -1, 8 );
	circle(imageCapteur, cv::Point(236, 16), 5, cv::Scalar(0,255*SensorState.CP[9],255-255*SensorState.CP[9]), -1, 8 );
	circle(imageCapteur, cv::Point(190, 135), 5, cv::Scalar(0,255*SensorState.CP[10],255-255*SensorState.CP[10]), -1, 8 );

	// CPI
	circle(imageCapteur, cv::Point(100, 346), 5, cv::Scalar(0,255*SensorState.CPI[1],255-255*SensorState.CPI[1]), -1, 8 );
	circle(imageCapteur, cv::Point(185, 346), 5, cv::Scalar(0,255*SensorState.CPI[2],255-255*SensorState.CPI[2]), -1, 8 );
    	circle(imageCapteur, cv::Point(847, 345), 5, cv::Scalar(0,255*SensorState.CPI[3],255-255*SensorState.CPI[3]), -1, 8 );
	circle(imageCapteur, cv::Point(931, 345), 5, cv::Scalar(0,255*SensorState.CPI[4],255-255*SensorState.CPI[4]), -1, 8 );
	circle(imageCapteur, cv::Point(925, 21), 5, cv::Scalar(0,255*SensorState.CPI[5],255-255*SensorState.CPI[5]), -1, 8 );
	circle(imageCapteur, cv::Point(841, 22), 5, cv::Scalar(0,255*SensorState.CPI[6],255-255*SensorState.CPI[6]), -1, 8 );
	circle(imageCapteur, cv::Point(183, 19), 5, cv::Scalar(0,255*SensorState.CPI[7],255-255*SensorState.CPI[7]), -1, 8 );
	circle(imageCapteur, cv::Point(97, 20), 5, cv::Scalar(0,255*SensorState.CPI[8],255-255*SensorState.CPI[8]), -1, 8 );

	// PS
	circle(imageCapteur, cv::Point(27, 181), 5, cv::Scalar(0,255*SensorState.PS[1],255-255*SensorState.PS[1]), -1, 8 );
	circle(imageCapteur, cv::Point(241, 230), 5, cv::Scalar(0,255*SensorState.PS[2],255-255*SensorState.PS[2]), -1, 8 );
	circle(imageCapteur, cv::Point(248, 342), 5, cv::Scalar(0,255*SensorState.PS[3],255-255*SensorState.PS[3]), -1, 8 );
	circle(imageCapteur, cv::Point(326, 230), 5, cv::Scalar(0,255*SensorState.PS[4],255-255*SensorState.PS[4]), -1, 8 );
	circle(imageCapteur, cv::Point(572, 231), 5, cv::Scalar(0,255*SensorState.PS[5],255-255*SensorState.PS[5]), -1, 8 );
	circle(imageCapteur, cv::Point(700, 231), 5, cv::Scalar(0,255*SensorState.PS[6],255-255*SensorState.PS[6]), -1, 8 );
	circle(imageCapteur, cv::Point(934, 232), 5, cv::Scalar(0,255*SensorState.PS[7],255-255*SensorState.PS[7]), -1, 8 );
	circle(imageCapteur, cv::Point(992, 339), 5, cv::Scalar(0,255*SensorState.PS[8],255-255*SensorState.PS[8]), -1, 8 );
	circle(imageCapteur, cv::Point(995, 189),5, cv::Scalar(0,255*SensorState.PS[9],255-255*SensorState.PS[9]), -1, 8 );
	circle(imageCapteur, cv::Point(783, 135), 5, cv::Scalar(0,255*SensorState.PS[10],255-255*SensorState.PS[10]), -1, 8 );
	circle(imageCapteur, cv::Point(770, 28), 5, cv::Scalar(0,255*SensorState.PS[11],255-255*SensorState.PS[11]), -1, 8 );
	circle(imageCapteur, cv::Point(701, 136), 5, cv::Scalar(0,255*SensorState.PS[12],255-255*SensorState.PS[12]), -1, 8 );
	circle(imageCapteur, cv::Point(454, 135), 5, cv::Scalar(0,255*SensorState.PS[13],255-255*SensorState.PS[13]), -1, 8 );
	circle(imageCapteur, cv::Point(321, 135), 5, cv::Scalar(0,255*SensorState.PS[14],255-255*SensorState.PS[14]), -1, 8 );
	circle(imageCapteur, cv::Point(66, 135), 5, cv::Scalar(0,255*SensorState.PS[15],255-255*SensorState.PS[15]), -1, 8 );
	circle(imageCapteur, cv::Point(29, 26), 5, cv::Scalar(0,255*SensorState.PS[16],255-255*SensorState.PS[16]), -1, 8 );

	// Aiguillages
	circle(imageCapteur, cv::Point(21, 241), 16, cv::Scalar(255,255,255), -1); //A01
	circle(imageCapteur, cv::Point(270, 253), 16, cv::Scalar(255,255,255), -1); //A02
	circle(imageCapteur, cv::Point(378, 235), 16, cv::Scalar(255,255,255), -1); //A03
	circle(imageCapteur, cv::Point(642, 235), 16, cv::Scalar(255,255,255), -1); //A04
	circle(imageCapteur, cv::Point(751, 253), 16, cv::Scalar(255,255,255), -1); //A05
	circle(imageCapteur, cv::Point(1002, 240), 16, cv::Scalar(255,255,255), -1); //A06
	circle(imageCapteur, cv::Point(1000, 127), 16, cv::Scalar(255,255,255), -1); //A07
	circle(imageCapteur, cv::Point(752, 112), 16, cv::Scalar(255,255,255), -1); //A08
	circle(imageCapteur, cv::Point(645, 127), 16, cv::Scalar(255,255,255), -1); //A09
	circle(imageCapteur, cv::Point(377, 129), 16, cv::Scalar(255,255,255), -1); //A10
	circle(imageCapteur, cv::Point(269, 113), 16, cv::Scalar(255,255,255), -1); //A11
	circle(imageCapteur, cv::Point(21, 125), 16, cv::Scalar(255,255,255), -1); //A12

	if(SensorState.DD[1]) line(imageCapteur, cv::Point(14, 228), cv::Point(14, 253), cv::Scalar(0,100,0), 3); //A01
	if(SensorState.DG[1]) ellipse(imageCapteur, cv::Point(37, 220),cv::Size(23,23),0,92,163,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[2]) ellipse(imageCapteur, cv::Point(290, 268),cv::Size(23,23),0,183,255,cv::Scalar(0,100,0), 3);//A02
	if(SensorState.DG[2]) line(imageCapteur, cv::Point(257, 245), cv::Point(283, 245), cv::Scalar(0,100,0), 3);
	if(SensorState.DD[3]) line(imageCapteur, cv::Point(366, 243), cv::Point(390, 243), cv::Scalar(0,100,0), 3); //A03
	if(SensorState.DG[3]) ellipse(imageCapteur, cv::Point(358, 221),cv::Size(23,23),0,73,1,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[4]) line(imageCapteur, cv::Point(629, 243), cv::Point(654, 243), cv::Scalar(0,100,0), 3); //A04
	if(SensorState.DG[4]) ellipse(imageCapteur, cv::Point(665, 220),cv::Size(23,23),0,113,181,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[5]) ellipse(imageCapteur, cv::Point(732, 265),cv::Size(23,23),0,284,363,cv::Scalar(0,100,0), 3);
	if(SensorState.DG[5]) line(imageCapteur, cv::Point(738, 245), cv::Point(764, 245), cv::Scalar(0,100,0), 3); //A05
	if(SensorState.DD[6]) line(imageCapteur, cv::Point(1008, 226), cv::Point(1008, 252), cv::Scalar(0,100,0), 3); //A06
	if(SensorState.DG[6]) ellipse(imageCapteur, cv::Point(985, 221),cv::Size(23,23),0,83,12,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[7]) line(imageCapteur, cv::Point(1008, 114), cv::Point(1008, 138), cv::Scalar(0,100,0), 3); //A07
	if(SensorState.DG[7]) ellipse(imageCapteur, cv::Point(985, 145),cv::Size(23,23),0,271,348,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[8]) ellipse(imageCapteur, cv::Point(733, 96),cv::Size(23,23),0,75,3,cv::Scalar(0,100,0), 3);//A08
	if(SensorState.DG[8]) line(imageCapteur, cv::Point(740, 121), cv::Point(764, 121), cv::Scalar(0,100,0), 3); 
	if(SensorState.DD[9]) line(imageCapteur, cv::Point(631, 121), cv::Point(659, 121), cv::Scalar(0,100,0), 3); //A09
	if(SensorState.DG[9]) ellipse(imageCapteur, cv::Point(665, 143),cv::Size(23,23),0,184,256,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[10]) line(imageCapteur, cv::Point(364, 122), cv::Point(390, 122), cv::Scalar(0,100,0), 3); //A10
	if(SensorState.DG[10]) ellipse(imageCapteur, cv::Point(358, 144),cv::Size(23,23),0,282,357,cv::Scalar(0,100,0), 3);
	if(SensorState.DD[11]) ellipse(imageCapteur, cv::Point(290, 98),cv::Size(23,23),0,110,183,cv::Scalar(0,100,0), 3); //A11
	if(SensorState.DG[11]) line(imageCapteur, cv::Point(257, 121), cv::Point(281, 121), cv::Scalar(0,100,0), 3);
	if(SensorState.DD[12]) line(imageCapteur, cv::Point(14, 113), cv::Point(14, 137), cv::Scalar(0,100,0), 3); //A12
	if(SensorState.DG[12]) ellipse(imageCapteur, cv::Point(38, 145),cv::Size(23,23),0,194,268,cv::Scalar(0,100,0), 3);
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
		if (msg->RD[i]==1)
			SwitchRightControl+=pow(2,i-1);
		if (msg->RG[i]==1)
			SwitchLeftControl+=pow(2,i-1);
		if (msg->LOCK[i]==1)
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

	StopControl.ST = msg->ST;
	StopControl.GO = msg->GO;

	for (int i=1;i<=24;i++){
		if (msg->ST[i]==1)
			StopControlInt+=pow(2,i-1);
		if (msg->GO[i]==1)
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
		}
    else if (pid > 0) 
		{

			while(system("pidof -x vrep > /dev/null")) // Attente d'un process du nom de "vrep"
			sleep(2);
		
			//Initialisation du noeud
			ros::init(argc, argv, "simulation");
			ros::NodeHandle nh;

			imageTot.setTo(cv::Scalar(200,200,200));
			playButton.copyTo(imageTot.rowRange(10,67).colRange(364,537));
			pauseButton.copyTo(imageTot.rowRange(10,67).colRange(564,737));
			///////// SUBSCRIBERS ////////

				// Image Streaming 
		 		cv::namedWindow("Simulation");
				cv::startWindowThread();
				cv::moveWindow("Simulation",0,0);
				cv::setMouseCallback("Simulation", onMouse,0);
				image_transport::ImageTransport it(nh);
				image_transport::Subscriber subImage = it.subscribe("vrep/VisionSensorData", 1, imageCallback);

				// Sensors State
		 		ros::Subscriber VREPsubRailSensor = nh.subscribe("vrep/RailSensor", 1, CapteurCallbackRail);
		 		ros::Subscriber VREPsubStationSensor = nh.subscribe("vrep/StationSensor", 1, CapteurCallbackStation);
		 		ros::Subscriber VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1, CapteurCallbackSwitch);
				ros::Subscriber VREPsubCapteurState = nh.subscribe("vrep/RailSensor",1, CapteurStateCallback);


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


			ros::Rate loop_rate(25);

			while (ros::ok())
			{
				ros::spinOnce();
				TPrailSensorState.publish(SensorState);
				loop_rate.sleep();		
			}				
				

			cv::destroyWindow("view");
			system("pkill vrep");
			return 0;
		} 
}
