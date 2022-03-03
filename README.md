# -Udacity_Robo_SW_ND_P5_Home_Service_Robot
The goal of this project is to program a robot that can autonomously map and environment and navigate to pick up and dropped virtual objects with ROS Kinetic &amp; Gazebo. Solution for Udacity Robotics Software Engineer Nanodegree Program


## Project Directory Structure

                                       # Official ROS packages
    ├── slam_gmapping                  # gmapping_demo.launch file                   
    │   ├── gmapping
    │   ├── ...
    ├── turtlebot                      # keyboard_teleop.launch file
    │   ├── turtlebot_teleop
    │   ├── ...
    ├── turtlebot_interactions         # view_navigation.launch file      
    │   ├── turtlebot_rviz_launchers
    │   ├── ...
    ├── turtlebot_simulator            # turtlebot_world.launch file 
    │   ├── turtlebot_gazebo
    │   ├── ...
    ├──                                 # My packages and direcotries
    |
    ├── map                             # map files
    │   ├── map.yaml
    ├── scripts                          # shell scripts files
    │   ├── test_slam.sh
    │   ├── test_navigation.sh
    │   ├── pick_objects.sh
    │   ├── add_markers.sh
    │   ├── home_service.sh
    │   
    │   
    ├──rvizConfig                      # rviz configuration files
    ├── home_serving_robot.rviz
    ├──pick_objects                    # pick_objects C++ node
    │   ├── src/pick_objects.cpp
    │   ├── ...
    ├──add_markers                     # add_marker C++ node
    │   ├── src/add_markers.cpp
    │   ├── src/add_markers_basics.cpp
    └──


## Pacages Description : 
 
1. map : includes my map file generated from testing
2. Shell scripts: multiple shell scripts were written that represent different steps in creating the overall simulation.
All shell scripts launch the turtlebot robot into my custom world.
   
   * test_slam.sh: to test SLAM with the gmapping package and teleoperation. It can be used to create and save a map of the environment.
   * test_navigation.sh: to test manual navigation with the AMCL package. It is possible to give navigation goals to the robot through Rviz.
   * pick_objects.sh: to test sending navigation goals through code, calls the pick_objects node.
   * add_markers.sh: to test creating virtual objects, calls the add_markers_time node, which spawns a virtual object, removes it after 5 seconds and respawn it in a new location after 5 more seconds.
   * home_service.sh: the complete simulation. In addition to spawning the turtlebot in the custom world, launching the AMCL package for localization and opening Rviz for visualization, it also launches both the add_markers and pick_objects nodes that command the robot to its pickup and dropoff zones, using the ROS navigations stack, while moving the virtual object.
   
   
## Ros Packages used and How does it work :

* Robot: the robot used is a Turtlebot 2. It is a little mobile robot with a Kinect depth sensor that we use here as fake laser measurements to interface with the different packages for localization and mapping.

* Localization: performed with the AMCL package (Adaptive Monte-Carlo Localization). This package is using a particle filter approach for localization with a known map. It generates particles that each represent a possible pose of the robot. At each iteration of the filter, the new laser measurement data is used to give a score to each particle. The ones with the best scores are kept while the other ones are resampled around them. This process, after enough iterations, should make all the particles converge around the true pose of the robot. The particles can sometimes struggle to converge if the environment does not have enough distinct features (such as parallel walls).

*  Mapping: performed with the Gmapping package, that is using the robot's laser scan data to perform SLAM (Simultaneous Localization And Mapping). By simultaneously tracking the pose of the robot and using the laser data to sense the robot's environment, the package is able to output a 2D occupancy grid map as well as an estimation of the robot's pose. Obtaining a map of the environment is crucial for the robot to perform global path planning and avoid any obstacle.

*  Navigation: the navigation capabilities rely on the ROS navigation stack. The map of the environment is used to determine the free and occupied spaces, namely where our robot is allowed to go or not. This is done by creating a cost map, by inflating all the obstacles by the width of the robot, in order to prevent the robot from trying to go to a place where it might touch obstacles. Path planning is then performed on this map. It relies on Dijkstra's algorithm in order to find the shortest path between the robot's current position and its goal. If a new obstacle is detected on the chosen trajectory, a new path is calculated to avoid it. Navigations goals are given here through the pick_objects node and correspond to the initial and final position of the virtual object.

*  Virtual object: as the Turtlebot has no object manipulation capability, we are simulating the picking up and dropping off of an object that does not exist in the simulated world (Gazebo), but we are representing it in Rviz, which is only for visualization, hence the word virtual. That virtual object is created at its initial position, deleted when the robot reaches the pick up location, and finally respawned at its final/drop off location when the robot reaches it. All of this is done in the add_markers node.
    
    



### Related documentation

    Gmapping package: http://wiki.ros.org/gmapping
    AMCL package: http://wiki.ros.org/amcl
    ROS Navigation stack: http://wiki.ros.org/navigation
    Turtlebot: http://wiki.ros.org/turtlebot_gazebo
