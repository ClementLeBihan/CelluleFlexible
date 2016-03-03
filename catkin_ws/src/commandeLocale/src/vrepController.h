#include <ros/ros.h>
#include "vrep_common/simRosLoadModel.h"
class vrepController
{
	private:
		ros::ServiceClient client_simRosLoadModel;
		vrep_common::simRosLoadModel srv_LoadModel;
	public:
		void init(ros::NodeHandle n);
		void close();
		void loadModel(char shuttleNumber);
		void waitVrep();
		void play();
		void pause();
};
