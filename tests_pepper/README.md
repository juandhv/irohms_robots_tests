# IROHMS Lab Demos (Pepper). How to Use:

These packages can be installed through [irohms-env](https://github.com/juandhv/tue-env).

## 0. Installing (checking out) pepper simulation tests

Before installing any new package, update your installed packages:

```bash
irohms-get update
```

To install (check out) the Pepper tests in simulation:

```bash
irohms-get install ros-test_pepper_simulation_bringup
```

To build the new packages in either case:

```bash
irohms-make
```

## 1. MoveIt! tests:

To test the basic MoveIt! functionalities with Pepper, you should in separate terminals:

To start the Pepper in simulation (Gazebo):

```bash
test-pepper-simulation-full robot_env:=ipa-apartment moveit:=true
```

To start RViz:

```bash
test-pepper-desktop-rviz
```

Once the simulation is running, wait till the robot puts its right arm at the front and the left arm behind before moving the base. To run the demo with MoveIt! functionalities:

```bash
rosrun test_pepper_common_bringup pepper_moveit_demo
```

## 2. Mapping tests:

To test the basic `gmapping` functionalities with Pepper, you should in separate terminals:

To start the Pepper in simulation (Gazebo):

```bash
test-pepper-simulation-full robot_env:=ipa-apartment moveit:=true mapping:=true
```

To start RViz:

```bash
test-pepper-desktop-rviz
```

Once the simulation is running, wait till the robot puts its right arm at the front and the left arm behind before moving the base.

To teleoperate and move the base:

```bash
test-pepper-desktop-base-teleop
```

Start moving the robot around using this terminal and the map will start appearing in the RViz window.

Once all the mapping procedure is finished or it is desired to save the map, open a new terminal and run the following command without closing the simulation:

```bash
rosrun map_server map_saver -f {map_name}
```

> Note: `{map_name}` should be replaced. This will save two files, a `.pgm` and a `.yaml`, both are needed so that the map can be used afterwards.

## 3. Localization tests:

To test the basic `amcl` functionalities with Pepper, you should in separate terminals:

To start the Pepper in simulation (Gazebo):

```bash
test-pepper-simulation-full robot_env:=ipa-apartment moveit:=true localization:=true x_pose:=4 y_pose:=-4
```

> Note: To really test localization, spawn the robot in different locations by changing `x_pose` and `y_pose` parameters. To provide a custom map include the `map_file:={map_file_location}` parameter.

To start RViz:

```bash
test-pepper-desktop-rviz config:=amcl
```

> Note: A different configuration of RViz is used to see particle cloud from `amcl`.

To teleoperate and move the base:

```bash
test-pepper-desktop-base-teleop
```

Once the simulation is running completely, use the `2D Pose Estimate` tool located in the upper part of RViz and mark the estimated position of the robot with reference of the gazebo simulation.

Then start moving the robot around using the teleop window, eventually you will see the red particles around the robot converging to a single point in RViz meaning it has localized successfully.

## 4. Navigation tests:

To test the basic `move_base` functionalities with Pepper, you should in separate terminals:

To start the Pepper in simulation (Gazebo):

```bash
test-pepper-simulation-full robot_env:=ipa-apartment moveit:=true localization:=true navigation:=true x_pose:=4 y_pose:=-4
```

> Note: For navigation, localization is required. `DWAPlanner` is used with `move_base`. To provide a custom map include the `map_file:={map_file_location}` parameter.

To start RViz:

```bash
test-pepper-desktop-rviz config:=amcl
```

To teleoperate and move the base:

```bash
test-pepper-desktop-base-teleop
```

> Note: Close the teleop window once the robots is succesfully localized.

Use the `2D Nav Goal` tool in the upper side of RViz and set a goal for the robot, it should start moving towards the goal. It is also possible to localize the robot simply by defining goals but closing the teleop window is needed.
