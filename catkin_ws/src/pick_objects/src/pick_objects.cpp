#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// Target locations
float pickUpObject[3] = {1.96, -1.75,3.14};
float dropOffObject[3] = {-2.35, -1.30, 2.90};

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "simple_navigation_goals");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal pickUp;
  move_base_msgs::MoveBaseGoal dropOff;

  // set up the frame parameters
  pickUp.target_pose.header.frame_id = "map";
  pickUp.target_pose.header.stamp = ros::Time::now();
//  goal2.target_pose.header.frame_id = "map";
//  goal2.target_pose.header.stamp = ros::Time::now();
  

  // Define a position and orientation for the robot to reach
  pickUp.target_pose.pose.position.x = pickUpObject[0];
  pickUp.target_pose.pose.position.y = pickUpObject[1];
  pickUp.target_pose.pose.orientation.w = pickUpObject[2];
  
//  goal2.target_pose.pose.position.x = dropOffObject[0];
//  goal2.target_pose.pose.position.y = dropOffObject[1];
//  goal2.target_pose.pose.orientation.w = dropOffObject[2];

   // Send the goal position and orientation for the robot to reach
  ROS_INFO("-- Sending robot to pick up position --");
  ac.sendGoal(pickUp);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_INFO("The Robot has reached the pick up position");
    ros::Duration(5.0).sleep();  
    }
  else {
    ROS_INFO("The robot has failed to reach the pick up position");
    ros::Duration(5.0).sleep(); 
    }
/**  
  // Send the goal position and orientation for the robot to reach
  ROS_INFO("-- Sending robot to drop off position --");
  ac.sendGoal(goal2);
  // Wait an infinite time for the results
  ac.waitForResult();
  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("-- The Robot has reached the drop off position --");
    ros::Duration(5.0).sleep();  
  else
    ROS_INFO("The robot has failed to reach the drop off position");
**/
  return 0;
}