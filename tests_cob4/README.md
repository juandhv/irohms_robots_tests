# IROHMS Lab Demos (Care-O-Bot 4). How to Use:

These packages can be installed through [irohms-env](https://github.com/juandhv/tue-env).

## 0. Installing (checking out) Cob4 simulation tests

Before installing any new package, update your installed packages:
```
irohms-get update
```

To install (check out) the Cob4 tests in simulation:
```
irohms-get install ros-test_cob4_simulation_bringup
```


To build the new packages in either case:
```
irohms-make
```

## 1. MoveIt! tests:

To test the basic MoveIt! functionalities with Care-O-Bot4, you should in separate terminals:

To start the Care-O-Bot 4 in simulation (Gazebo):
```
test-cob4-simulation-full robot_env:=ipa-apartment moveit:=true
```

To start RViz:
```
test-cob4-desktop-rviz
```

Once the simulation is running, wait till the robot puts its right arm at the front and the left arm behind before moving the base. To run the demo with MoveIt! functionalities:
```
rosrun test_cob4_common_bringup cob4_moveit_demo
```

## 2. Mapping tests:

To test the basic `gmapping` functionalities with Care-O-Bot4, you should in separate terminals:

To start the Care-O-Bot 4 in simulation (Gazebo):
```
test-cob4-simulation-full robot_env:=ipa-apartment moveit:=true mapping:=true
```

To start RViz:
```
test-cob4-desktop-rviz
```

Two separate windows, one for Gazebo and one for RViz window will open. In this case Gazebo is needed in order to simulate the space to be mapped and RViz to see the status of the map published by `gmapping`. Once the simulation is running, wait till the robot puts its right arm at the front and the left arm behind before moving the base.

To teleoperate and to move the base:
```
test-cob4-desktop-base-teleop
```

Start moving the robot around using this terminal and the map will start appearing in the RViz window.

Once all the mapping procedure is finished or it is desired to save the map, open a new terminal and run the following command without closing the simulation:
```
rosrun map_server map_saver -f {map_name}
```

`{map_name}` should be replaced. This will save two files, a `.pgm` and a `.yaml`, both are needed so that the map can be used afterwards.

## 3. Localization tests:

To test the basic `amcl` functionalities with Care-O-Bot4, you should in separate terminals:

To start the Care-O-Bot 4 in simulation (Gazebo):
```
test-cob4-simulation-full robot_env:=ipa-apartment moveit:=true localization:=true x_pose:=4 y_pose:=-4
```

To start RViz:
```
test-cob4-desktop-rviz
```

In the directory `test_cob4_simulation_bringup/maps` a map has already been provided. Then you can test localization enough by running the following: 

> test-cob4-simulation-localization robot_env:=ipa-apartment x_pose:=4 y_pose:=-4

To spawn the robot in the actual simulation in a different position in the map you can change the values from `x_pose` and `y_pose`.

If an own map is wanted to be used, you can run the following command instead:

> test-cob4-simulation-localization robot_env:=ipa-apartment x_pose:=4 y_pose:=-4 map_file:={map_file_location}

Remember to replace `{map_file_location}` by the actual location of the `.yaml` of the map saved. Also just as the mapping example, wait for the robot to move its arms to its home state.

Afterwards use the `2D Pose Estimate` tool located in the upper part of RViz and mark the estimated position of the robot with reference of the gazebo simulation. This will make it easier for the `amcl` node to locate the robot. Then start moving the base with the teleop window as in the mapping example. After moving the robot around you will see eventually the particle cloud to converge and the laser readings to position aligned to the map provided.