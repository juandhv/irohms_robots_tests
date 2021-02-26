
# Simulation tests

## Mapping

For mapping the following command should be run:

> test-cob4-simulation-mapping robot_env:=ipa-apartment

The above command runs everything needed in order to test the mapping procedure with COB4 and gmapping.

After it is is run,  a Gazebo and an RViz window will open. In this case gazebo is needed in order to simulate the space to be mapped and RViz to see the status of the map published by `gmapping`. Once the simulation is running, wait till the robot puts its right arm at the front and the left arm behind before moving the base.

An additional terminal like screen will open corresponding to a teleop node to move the base (`cob_teleop` isn't used since it can't change linear nor angular speeds). Start moving the robot around using this terminal and the map will start appearing in the RViz window.

Once all the mapping procedure is finished or it is desired to save the map, open a new terminal and run the following command without closing the simulation:

> rosrun map_server map_saver -f {map_name}

`{map_name}` should be replaced. This will save two files, a `.pgm` and a `.yaml`, both are needed so that the map can be used afterwards.

## Localization

For localization in this example, `amcl` ros node is used. In the directory `test_cob4_simulation_bringup/maps` a map has already been provided. Then you can test localization enough by running the following: 

> test-cob4-simulation-localization robot_env:=ipa-apartment x_pose:=4 y_pose:=-4

To spawn the robot in the actual simulation in a different position in the map you can change the values from `x_pose` and `y_pose`.

If an own map is wanted to be used, you can run the following command instead:

> test-cob4-simulation-localization robot_env:=ipa-apartment x_pose:=4 y_pose:=-4 map_file:={map_file_location}

Remember to replace `{map_file_location}` by the actual location of the `.yaml` of the map saved. Also just as the mapping example, wait for the robot to move its arms to its home state.

Afterwards use the `2D Pose Estimate` tool located in the upper part of RViz and mark the estimated position of the robot with reference of the gazebo simulation. This will make it easier for the `amcl` node to locate the robot. Then start moving the base with the teleop window as in the mapping example. After moving the robot around you will see eventually the particle cloud to converge and the laser readings to position aligned to the map provided.

## Navigation

Once mapping and localization have been accomplished, the robot has everything needed in order to navigate by itself to defined goals in the map. For the navigation example run the following command (omit `map_file` option if default provided map is wanted):

> test-cob4-simulation-navigation robot_env:=ipa-apartment x_pose:=4 y_pose:=-4 map_file:={map_file_location}

For this example, a planner node is used, in this case it is `DWAPlanner`.

Wait again till the robot moves its arms to the home state. At first, if RViz shows a completely wrong location of the robot based on the initial X and Y pose given, then it is necessary for it to go through the localization process, this means moving the base around the space using the teleop window before requesting goals. However if a good reference of location with the `2D Pose Estimate` tool is given, then you could start requesting goals right away as long as the robot shown in RViz is near to the location in the actual simulation. If the teleop window isn't used, it can be closed. 

Then, in order to make the robot navigate by itself, use the `2D Nav Goal` option in the upper part of RViz window and request the robot to move to a certain part with the RViz interface. If the robot isn't completely located with `amcl`, by moving the robot this way it will eventually accomplish localization by itself.





















