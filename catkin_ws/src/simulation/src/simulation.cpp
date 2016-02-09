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
				velo30.data = 30;
				velo0.data = 0;

			while (ros::ok())
			{
				ros::spinOnce();
	
				if (capteur1) {
					MotorVelocity.publish(velo0);
					sleep(2);
					do
					{
						MotorVelocity.publish(velo30);
						ros::spinOnce();
					} while(capteur1);
				}

				else MotorVelocity.publish(velo30);
			}	
			cv::destroyWindow("view");
			return 0;
		}
}
