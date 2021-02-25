
# Simulation tests

## Mapping

For mapping the following command should be run accordingly in different terminal windows:

> test-cob4-simulation-mapping robot_env:=ipa-apartment

Afterwards a Gazebo and an RVIZ window will open. In this case gazebo is needed in order to simulate the mapping of the space and rviz in order to see the status of the map made by `gmapping`. Once the simulation is running, wait till the robot puts its right arm at the front and the left arm behind before moving the base.

Also an additional terminal like screen will open corresponding to a teleop node to move the base. (`cob_teleop` since it can't change linear nor angular speeds)





