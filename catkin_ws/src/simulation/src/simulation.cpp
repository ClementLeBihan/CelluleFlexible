#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <signal.h>
#include <termios.h>
#include <sstream>
#include <string>

using namespace std;

#include "vrepController.h"
#include "UI.h"
#include "inOutController.h"

#include <ros/ros.h>

vrepController VREPController;

int main(int argc, char **argv)
{
    pid_t pid;
 
    if ((pid = fork()) < 0)
        return EXIT_FAILURE;
    else if (pid == 0) 
		{
			VREPController.launch();

			ros::init(argc, argv, "simulation2");
			ros::NodeHandle nh2;
			
			VREPController.init(nh2);

			for(int i=1;i<=argc-1;i++) VREPController.loadModel(argv[i][0]); 

			// Demarrage de la simulation pour avoir la premiere image
			VREPController.play();
			// Pause pour laisser à l'utilisateur le soin de Commencer la simulation
			VREPController.pause();
		}
    else if (pid > 0) 
		{

			VREPController.waitVrep();

			//Initialisation du noeud
			ros::init(argc, argv, "simulation");
			ros::NodeHandle nh;

			UI userInterface;
			userInterface.init(nh);

			inOutController IOController(&userInterface);
			IOController.init(nh);

			while (ros::ok())
			{
				ros::spinOnce();
			}				
				
			userInterface.kill();
			VREPController.close();
			return 0;
		} 
}
