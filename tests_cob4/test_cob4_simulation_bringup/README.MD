
# Simulation tests

## Mapping

For mapping the following command should be run:

> test-cob4-simulation-mapping robot_env:=ipa-apartment

The above command runs everything needed in order to test the mapping procedure with COB4 and gmapping.

After the command is run,  a Gazebo and an RViz window will open. In this case gazebo is needed in order to simulate the mapping of the space and RViz to see the status of the map made by `gmapping`. Once the simulation is running, wait till the robot puts its right arm at the front and the left arm behind before moving the base.

An additional terminal like screen will open corresponding to a teleop node to move the base (`cob_teleop` since it can't change linear nor angular speeds). Start moving the robot around using this terminal and the map will start appearing in the RViz window.

Once all the mapping procedure is done or it is desired to save the map, open a new terminal and run the following command without closing the simulation:

> rosrun map_server map_saver -f {map_name}

`{map_name}` should be replaced. This will save the map in the directory `maps`  so that then it can be used for the localization test. Two files should be saved, a `.pgm` and a `.yaml`.

## Localization

For localization in this example, `amcl` ros node is used. In the directory `test_cob4_simulation_bringup/maps` a map has already been provided. Then you can test localization enough by running the following: 

> test-cob4-simulation-localization robot_env:=ipa-apartment x_pose:=4 y_pose:=-4

To spawn the robot in the actual simulation in a different position in the map you can change the values from `x_pose` and `y_pose`.

If an own map is wanted to be used, you can run the following command instead:

> test-cob4-simulation-localization robot_env:=ipa-apartment x_pose:=4 y_pose:=-4 map_file:={map_file_dir}

Remember to replace `{map_file_dir}` by the actual location of the `.yaml` of the map saved. Also just as the mapping example, wait for the robot to move its arms to its home state.

Afterwards use the `2D Pose Estimate` tool located in the upper part of RViz and mark the estimated position of the robot with reference of the gazebo simulation. This will make it easier for the `amcl` node to locate the robot. Then start moving the base with the teleop window as in the mapping example.






