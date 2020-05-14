#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/String.h>

class PickObjectsPublisher
{
public:
  PickObjectsPublisher()
  {
    //Topic to publish what the status of the marker should be
    pub_status = n_.advertise<std_msgs::String>("marker_status", 1);

  }

  //Public method to publish the marker status
  void publish(std_msgs::String status_message){
    pub_status.publish(status_message);
  }

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_status;

};

//Client to send navigation goals to MoveBase server
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){

  ros::init(argc, argv, "marker_status");

  //Variables to hold current message
  std_msgs::String msg;
  std::string str_status;

  //Instance of publisher class
  PickObjectsPublisher po_publisher;

  
  MoveBaseClient new_mb_client("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!new_mb_client.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

   //Publish initial status
  str_status = "Pickup";
  msg.data = str_status;
  po_publisher.publish(msg);

  //Create pickup navigation goal
  move_base_msgs::MoveBaseGoal pickup;


  pickup.target_pose.header.frame_id = "map";
  pickup.target_pose.header.stamp = ros::Time::now();

  // Pick up position for the robot
  pickup.target_pose.pose.position.x = 2.8;
  pickup.target_pose.pose.position.y = -1.8;
  pickup.target_pose.pose.orientation.w = 3.14;

 //Create drop off navigation goal
  move_base_msgs::MoveBaseGoal drop_off;


  drop_off.target_pose.header.frame_id = "map";
  drop_off.target_pose.header.stamp = ros::Time::now();

  // Pick up position for the robot
  drop_off.target_pose.pose.position.x = 0.0;
  drop_off.target_pose.pose.position.y = -1.6;
  drop_off.target_pose.pose.orientation.w = 2.9;


   // Send the goal position for object pickup
  ROS_INFO("Robot is traveling to the pick up location.");
  new_mb_client.sendGoal(pickup);

  // Wait an infinite time for the results
  new_mb_client.waitForResult();

  // Check if the robot reached the pickup location
  if(new_mb_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
   {
  
     
       //Display message
       ROS_INFO("Robot has picked up object.");

       //Publish updated status
       str_status = "Invisible";
       msg.data = str_status;
       po_publisher.publish(msg);


       // Wait 5 seconds to simulate object pick up
       ros::Duration(5.0).sleep();


       //Send new goal position for object drop off  
       ROS_INFO("Robot is traveling to the drop off location.");
       new_mb_client.sendGoal(drop_off);

     
       // Wait an infinite time for the results
       new_mb_client.waitForResult();
       
       // Check if the robot reached the drop off location
       if(new_mb_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
	{
            //Display message
            ROS_INFO("Robot has dropped off the object!");

            
            //Publish updated status
            str_status = "Dropped";
            msg.data = str_status;
            po_publisher.publish(msg);


            //Wait 5 seconds to simulate drop off
            ros::Duration(5.0).sleep();
       	 }

         else
         //Display message
         ROS_INFO("Robot could not drop off the object.");
  }
  else
    //Display message
    ROS_INFO("Robot could not pick up the object.");

  return 0;
}
