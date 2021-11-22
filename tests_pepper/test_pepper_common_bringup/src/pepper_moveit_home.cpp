#include <iostream>
#include <vector>

#include <cmath>
#include <cstdlib>
#include <string>

// ROS stuff
#include <ros/ros.h>
#include <eigen_conversions/eigen_msg.h>
#include <tf_conversions/tf_eigen.h>

#include <geometry_msgs/PoseStamped.h>
#include <moveit/robot_state/robot_state.h>

#include <control_msgs/GripperCommandAction.h>
#include <control_msgs/PointHeadAction.h>
#include <control_msgs/PointHeadGoal.h>
#include <geometry_msgs/TransformStamped.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene/planning_scene.h>
#include <moveit/planning_scene_monitor/planning_scene_monitor.h>

#include <actionlib/client/simple_action_client.h>

//!  PepperMoveItHome class.
/*!
 * Pepper MoveIt! Home.
 *
 */
class Pepper
{
public:
    //! Constructor
    Pepper();
    //! Function for waiting for user confirmation
    void waitForUserConfirmation(const std::string &next_task);
    //! Run
    void run();

private:
    // ROS
    ros::NodeHandle node_handler_, local_nh_;
    bool replan_;
};

Pepper::Pepper() : local_nh_("~"), replan_(false)
{
    //=======================================================================
    // Subscribers
    //=======================================================================
}

void Pepper::run()
{
    ros::AsyncSpinner spinner(4);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    moveit::planning_interface::MoveGroupInterface move_group_right("right_arm");
    moveit::planning_interface::MoveGroupInterface move_group_left("left_arm");

    move_group_right.setPlannerId("RRTConnectkConfigDefault");
    move_group_left.setPlannerId("RRTConnectkConfigDefault");

    auto pcm = planning_scene_monitor::PlanningSceneMonitorPtr(
        new planning_scene_monitor::PlanningSceneMonitor("robot_description"));
    planning_scene_monitor::LockedPlanningSceneRO planning_scene(pcm);

    // Open the gripper before starting
    moveit::planning_interface::MoveItErrorCode success =
        moveit::planning_interface::MoveItErrorCode::FAILURE;

    ros::Rate loop_rate(10);

    //=======================================================================
    // Replanning if needed
    //=======================================================================
    replan_ = true;
    std::string arm = "right";

    while (ros::ok())
    {
        if (replan_)
        {
            replan_ = true;

            // Pregrasp
            geometry_msgs::PoseStamped target_pose;

            target_pose.header.frame_id = "base_link";
            if (arm.compare("right") == 0)
            {
                target_pose.pose.position.x = 0.018;
                target_pose.pose.position.y = -0.222;
                target_pose.pose.position.z = -0.186;
                target_pose.pose.orientation.x = 0.586;
                target_pose.pose.orientation.y = 0.226;
                target_pose.pose.orientation.z = -0.295;
                target_pose.pose.orientation.w = 0.719;
                move_group_right.setPoseReferenceFrame("base_link");
                move_group_right.setGoalTolerance(0.05);
                move_group_right.setGoalOrientationTolerance(0.05);
                move_group_right.setPoseTarget(target_pose);
                move_group_right.setApproximateJointValueTarget(target_pose, "r_wrist");
            }
            else
            {
                target_pose.pose.position.x = 0.044;
                target_pose.pose.position.y = 0.205;
                target_pose.pose.position.z = -0.195;
                target_pose.pose.orientation.x = -0.57;
                target_pose.pose.orientation.y = 0.255;
                target_pose.pose.orientation.z = 0.286;
                target_pose.pose.orientation.w = 0.726;
                move_group_left.setPoseReferenceFrame("base_link");
                move_group_left.setGoalTolerance(0.1);
                move_group_left.setGoalOrientationTolerance(0.1);
                move_group_left.setPoseTarget(target_pose);
                move_group_left.setApproximateJointValueTarget(target_pose, "l_wrist");
            }

            if (arm.compare("right") == 0)
            {
                success = move_group_right.plan(my_plan);
            }
            else
            {
                success = move_group_left.plan(my_plan);
            }

            if (success == moveit::planning_interface::MoveItErrorCode::SUCCESS)
            {
                if (arm.compare("right") == 0)
                {
                    move_group_right.execute(my_plan);
                }
                else
                {
                    move_group_left.execute(my_plan);
                }
            }
            loop_rate.sleep();
            sleep(1.0);

            if (!success)
                continue;
            if (arm.compare("right") == 0)
            {
                arm = "left";
            }
            else
            {
                break;
            }
        }
        // std::cout << "Waiting the user to move close."<< std::endl;
        loop_rate.sleep();
    }

    spinner.stop();
    ros::shutdown();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pepper_moveit_home");

    Pepper home;
    home.run();

    return 0;
}
