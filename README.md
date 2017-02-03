# Projet_Long GEA N7 2016

[![Join the chat at https://gitter.im/ClementLeBihan/CelluleFlexible](https://badges.gitter.im/ClementLeBihan/CelluleFlexible.svg)](https://gitter.im/ClementLeBihan/CelluleFlexible?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

> Simulation of a transport network

## Auteurs
- __Emilie ANTONIUTTI__ -- emilie.antoniutti@etu.enseeiht.fr
- __Thibault BERTIN__ -- thibault.bertin@etu.enseeiht.fr
- __Simon DEMMER__ -- simon.demmer@etu.enseeiht.fr
- __Clément LE BIHAN__ -- clement.lebihan@etu.enseeiht.fr

## TO DO

1. **in ~/.bashrc, add these lines :**
	- source /opt/ros/jade/setup.bash
	- export VREP_ROOT_DIR=~/CelluleFlexible/V-Rep/
	- source ~/CelluleFlexible/catkin_ws/devel/setup.bash

2. **In /Projet_Long/catkin_ws**
	- catkin_make

3. **Don't forget to launch roscore before launching our application**
	- ~/CelluleFlexible/simulation 1 2 5 6 (or any number of shuttle between 1 and 6 seperated with a space)
	- ~/CelluleFlexible/planification

## MORE INFOS

**If you want to see our simulation.ttt on VREP :**
   - Execute ~/CelluleFlexible/V-Rep/vrep.sh ~/CelluleFlexible/Simulation.ttt

## License

[![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png)](http://creativecommons.org/publicdomain/zero/1.0/)
