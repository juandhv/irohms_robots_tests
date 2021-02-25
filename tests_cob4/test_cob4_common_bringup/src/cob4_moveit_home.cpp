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

//!  Cob4MoveItHome class.
/*!
 * Cob4 MoveIt! Home.
 *
 */
class Cob4MoveItHome
{
public:
    //! Constructor
    Cob4MoveItHome();
    //! Function for waiting for user confirmation
    void waitForUserConfirmation(const std::string& next_task);
    //! Run
    void run();

private:
    // ROS
    ros::NodeHandle node_handler_, local_nh_;
    bool replan_;
};

Cob4MoveItHome::Cob4MoveItHome() : local_nh_("~"), replan_(false)
{
    //=======================================================================
    // Subscribers
    //=======================================================================
}

void Cob4MoveItHome::run()
{
    ros::AsyncSpinner spinner(4);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    moveit::planning_interface::MoveGroupInterface move_group_right("arm_right");
    moveit::planning_interface::MoveGroupInterface move_group_left("arm_left");

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

            target_pose.pose.position.y = 0.0;
            target_pose.pose.position.z = 0.6;
            target_pose.header.frame_id = "base_link";
            if (arm.compare("right") == 0)
            {  
                target_pose.pose.position.x = 0.3;
                target_pose.pose.orientation.w = 1.57;
                move_group_right.setPoseTarget(target_pose);
            }
            else
            {
                target_pose.pose.position.x = -0.3;
                target_pose.pose.orientation.w = -1.57;
                move_group_left.setPoseTarget(target_pose);
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

int main(int argc, char** argv)
{
    ros::init(argc, argv, "cob4_moveit_home");

    Cob4MoveItHome home;
    home.run();

    return 0;
}
