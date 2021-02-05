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

typedef actionlib::SimpleActionClient<control_msgs::GripperCommandAction> gripper_action_client_t;
typedef actionlib::SimpleActionClient<control_msgs::PointHeadAction> point_head_action_client_t;

//!  Cob4ViconDemo class.
/*!
 * Cob4 Vicon Demo.
 *
 */
class Cob4ViconDemo
{
public:
    //! Constructor
    Cob4ViconDemo();
    //! Callback for getting game block position
    void gameBlockPosSubCallback(const geometry_msgs::TransformStampedConstPtr& pos_msg);
    //! Callback for getting cob4 position
    void cob4PosSubCallback(const geometry_msgs::TransformStampedConstPtr& pos_msg);
    //! Function for waiting for user confirmation
    void waitForUserConfirmation(const std::string& next_task);
    //! Run
    void run();

private:
    // ROS
    ros::NodeHandle node_handler_, local_nh_;
    ros::Subscriber game_block_pos_sub_, cob4_pos_sub_;

    // Objects
    std::vector<double> last_game_block_pos_, last_cob4_pos_;
    std::string safety_obj_vicon_topic_, cob4_vicon_topic_;

    bool replan_, cob4_pos_available_;

    // Controller action
    boost::shared_ptr<gripper_action_client_t> gripper_action_client_;
};

Cob4ViconDemo::Cob4ViconDemo()
  : replan_(false)
  , cob4_pos_available_(false)
  , local_nh_("~")
  , safety_obj_vicon_topic_("/vicon/game_block/game_block")
  , cob4_vicon_topic_("/vicon/robot/robot")
{
    last_game_block_pos_.resize(3);
    last_cob4_pos_.resize(3);

    //=======================================================================
    // Get parameters
    //=======================================================================
    local_nh_.param("safety_obj_vicon_topic", safety_obj_vicon_topic_, safety_obj_vicon_topic_);
    local_nh_.param("cob4_vicon_topic", cob4_vicon_topic_, cob4_vicon_topic_);

    //=======================================================================
    // Action lib
    //=======================================================================
    //    std::string action_name = "gripper_controller/gripper_action";
    //    gripper_action_client_.reset(new gripper_action_client_t(action_name, true));
    //    if (!gripper_action_client_->waitForServer(ros::Duration(2.0)))
    //        ROS_ERROR("%s may not be connected.", action_name.c_str());

    //=======================================================================
    // Subscribers
    //=======================================================================
    game_block_pos_sub_ = node_handler_.subscribe(safety_obj_vicon_topic_, 1,
                                                  &Cob4ViconDemo::gameBlockPosSubCallback, this);
    cob4_pos_sub_ =
        node_handler_.subscribe(cob4_vicon_topic_, 1, &Cob4ViconDemo::cob4PosSubCallback, this);
}

void Cob4ViconDemo::gameBlockPosSubCallback(const geometry_msgs::TransformStampedConstPtr& pos_msg)
{
    last_game_block_pos_[0] = pos_msg->transform.translation.x;
    last_game_block_pos_[1] = pos_msg->transform.translation.y;
    last_game_block_pos_[2] = pos_msg->transform.translation.z;

    if (cob4_pos_available_)
    {
        double dist = sqrt(pow(last_game_block_pos_[0] - last_cob4_pos_[0], 2.0) +
                           pow(last_game_block_pos_[1] - last_cob4_pos_[1], 2.0) +
                           pow(last_game_block_pos_[2] - last_cob4_pos_[2], 2.0));
        // std::cout << dist << std::endl;
        if (dist < 1.40)
            replan_ = true;
        else
            replan_ = false;
    }
}

void Cob4ViconDemo::cob4PosSubCallback(const geometry_msgs::TransformStampedConstPtr& pos_msg)
{
    last_cob4_pos_[0] = pos_msg->transform.translation.x;
    last_cob4_pos_[1] = pos_msg->transform.translation.y;
    last_cob4_pos_[2] = pos_msg->transform.translation.z;

    cob4_pos_available_ = true;
}

void Cob4ViconDemo::run()
{
    const std::string TRAJECTORIES_CAN = "Wood Can Top Grasp";
    const std::string TRAJECTORIES_TABLE = "Table Set Object On";
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
    std::string object = "Can1";
    while (ros::ok())
    {
        if (replan_)
        {
            //"Waiting the user to move away."
            replan_ = false;
            while (replan_ && ros::ok())
                loop_rate.sleep();
            replan_ = true;

            // Pregrasp
            geometry_msgs::PoseStamped target_pose_pre_grasp;
            //            tf::poseEigenToMsg(pre_grasp, target_pose_pre_grasp.pose);
            target_pose_pre_grasp.header.frame_id = "vicon_object_server/" + object;
            if (object.compare("Can1") == 0)
            {
                move_group_right.setPoseTarget(target_pose_pre_grasp);
            }
            else
            {
                move_group_left.setPoseTarget(target_pose_pre_grasp);
            }

            if (object.compare("Can1") == 0)
            {
                success = move_group_right.plan(my_plan);
            }
            else
            {
                success = move_group_left.plan(my_plan);
            }

            if (success == moveit::planning_interface::MoveItErrorCode::SUCCESS)
            {
                if (object.compare("Can1") == 0)
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

            // Grasp
            move_group_right.clearPoseTargets();
            move_group_left.clearPoseTargets();
            geometry_msgs::PoseStamped target_pose_grasp;
            //            tf::poseEigenToMsg(grasp, target_pose_grasp.pose);
            target_pose_grasp.header.frame_id = "vicon_object_server/" + object;
            if (object.compare("Can1") == 0)
            {
                move_group_right.setPoseTarget(target_pose_grasp);
            }
            else
            {
                move_group_left.setPoseTarget(target_pose_grasp);
            }

            loop_rate.sleep();
            if (object.compare("Can1") == 0)
            {
                success = move_group_right.plan(my_plan);
            }
            else
            {
                success = move_group_left.plan(my_plan);
            }

            if (success == moveit::planning_interface::MoveItErrorCode::SUCCESS)
            {
                if (object.compare("Can1") == 0)
                {
                    move_group_right.execute(my_plan);
                }
                else
                {
                    move_group_left.execute(my_plan);
                }
            }
            sleep(2.0);
            if (object.compare("Can1") == 0)
            {
                std::cout << "SEtting  CAN2" << std::endl;
                object = "Can2";
            }
            else
            {
                replan_ = false;
            }
        }
        // std::cout << "Waiting the user to move close."<< std::endl;
        loop_rate.sleep();
    }

    spinner.stop();
    ros::shutdown();
}

void Cob4ViconDemo::waitForUserConfirmation(const std::string& next_task)
{
    // std::cout << "Robot is about to ";
    if (isatty(STDOUT_FILENO))
    {
        // std::cout << "\033[1;32m<" << next_task << ">\033[0m. ";
    }
    else
    {
        // std::cout << "<" << next_task << ">. ";
    }
    // std::cout << "Press enter when ready."<< std::endl;
    std::cin.ignore();
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "cob4_vicon_demo");

    Cob4ViconDemo demo;
    demo.run();

    return 0;
}
