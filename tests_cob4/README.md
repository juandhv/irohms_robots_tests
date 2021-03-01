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

To run the demo with MoveIt! functionalities:
```
rosrun test_cob4_common_bringup cob4_moveit_demo
```
