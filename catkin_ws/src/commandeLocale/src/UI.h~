#ifndef VAR_UI
#define VAR_UI

#include <ros/ros.h>

// Image Streaming
#include <image_transport/image_transport.h>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <cv_bridge/cv_bridge.h>

#include <simulation/Msg_SensorState.h>

class UI
{
	private:
		cv::Mat imageSensor;
		cv::Mat imageSimu;
		cv::Mat imageTot;
		cv::Mat playButton;
		cv::Mat playButton_Down;
		cv::Mat playButton_On;
		cv::Mat pauseButton;
		cv::Mat pauseButton_Down;
		cv::Mat pauseButton_On;
		int mode;

		image_transport::Subscriber subImage;

		ros::ServiceClient client_simRosStartSimulation;
		ros::ServiceClient client_simRosPauseSimulation; 
	public:
		void DrawRailSensorImg(simulation::Msg_SensorState SensorState);
		void DrawStationSensorImg(simulation::Msg_SensorState SensorState);
		void DrawSwitchSensorImg(simulation::Msg_SensorState SensorState);
		void init(ros::NodeHandle nh);
		void update();
		void getSimuStream(const sensor_msgs::ImageConstPtr& msg);
		void onMouse_internal( int event, int x, int y);
		friend void onMouse(int event, int x, int y, int, void* userdata);
		void close();
};

#endif
