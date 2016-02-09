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


int capteur1, capteur2, capteur3, capteur4;


// Fonction qui stocke dans capteur 1 la valeur du capteur (0 ou 1)
void CapteurCallback(const std_msgs::Int32::ConstPtr& msg)
{
	capteur1 = (msg->data & 1) == 1;
	capteur2 = (msg->data & 2) == 2;
	capteur3 = (msg->data & 4) == 4;
	capteur4 = (msg->data & 8) == 8;
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
		 		ros::Subscriber subSensor = nh.subscribe("vrep/SensorsState", 1, CapteurCallback);

			///////// PUBLISHERS ////////
				//MotorVelocity
				ros::Publisher MotorVelocity = n.advertise<std_msgs::Float64>("/simulation/MotorVelocityTopic", 1);

			///////// VARIABLES ////////
				std_msgs::Float64 velo30, velo0;
				int handle_A1, handle_sol;
				int consigne = 0;
				ros::spinOnce();

			////////// VREP HANDLE  ///////
				ros::ServiceClient client_getObjectHandle=nh.serviceClient<vrep_common::simRosGetObjectHandle>("/vrep/simRosGetObjectHandle");
				vrep_common::simRosGetObjectHandle srv_getObjectHandle;
				
				// Aiguillage 1
				srv_getObjectHandle.request.objectName="Aiguillage_1";
				if ( client_getObjectHandle.call(srv_getObjectHandle))
				    handle_A1 = srv_getObjectHandle.response.handle;

				// Sol
				srv_getObjectHandle.request.objectName="ResizableFloor_5_25";
				if ( client_getObjectHandle.call(srv_getObjectHandle))
				    handle_sol = srv_getObjectHandle.response.handle;

			//////// VREP  SetObjectQuaternion /////
				ros::ServiceClient client_SetObjectQuaternion=nh.serviceClient<vrep_common::simRosSetObjectQuaternion>("/vrep/simRosSetObjectQuaternion");
				vrep_common::simRosSetObjectQuaternion srv_SetObjectQuaternion;

				srv_SetObjectQuaternion.request.handle = handle_A1;				
				srv_SetObjectQuaternion.request.relativeToObjectHandle = handle_sol;

			while (ros::ok())
			{
				ros::spinOnce();
				
				//On donne le quaternion voulu en fonction de la position (en fonction des états des capteurs)
	
				if ((capteur2||capteur3) && consigne == 0) {
					srv_SetObjectQuaternion.request.quaternion.x=0.26318;
					srv_SetObjectQuaternion.request.quaternion.y=0.656306;
					srv_SetObjectQuaternion.request.quaternion.z=0.26293;
					srv_SetObjectQuaternion.request.quaternion.w=-0.656408;
					consigne = 1; 
				}
				else if ((capteur1||capteur4) && consigne == 0) {
					srv_SetObjectQuaternion.request.quaternion.x=-0.27806;
					srv_SetObjectQuaternion.request.quaternion.y=0.65000;
					srv_SetObjectQuaternion.request.quaternion.z=-0.27816;
					srv_SetObjectQuaternion.request.quaternion.w=-0.65024;
					consigne = 1;
				}
				else consigne = 0;
				
				if(consigne == 1) 
					client_SetObjectQuaternion.call(srv_SetObjectQuaternion);


			}	
			cv::destroyWindow("view");
			return 0;
		}
}
