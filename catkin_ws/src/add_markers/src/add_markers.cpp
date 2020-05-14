#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <std_msgs/String.h>

class AddMarkers {
public:

  visualization_msgs::Marker marker;

  AddMarkers()
      {
         //Publisher
         pub_ = n_.advertise<visualization_msgs::Marker>("visualization_marker", 1);

         //Subscribe to /marker_status from the pick_objects node to know where the marker should be
         sub_status = n_.subscribe("/marker_status", 1, &AddMarkers::marker_status_callback, this);
	   
         marker.header.frame_id = "map";
         marker.header.stamp = ros::Time::now();

         // Set the namespace and id for this marker.
         marker.ns = "add_markers";
         marker.id = 0;

         // Set the marker type to cylinder
         marker.type = visualization_msgs::Marker::CYLINDER;

         // Add the marker
         marker.action = visualization_msgs::Marker::ADD;
         marker.pose.orientation.x = 0.0;
         marker.pose.orientation.y = 0.0;
         marker.pose.orientation.z = 0.0;
         marker.pose.orientation.w = 1.0;

         // Set the scale of the marker
         marker.scale.x = 0.2;
         marker.scale.y = 0.2;
         marker.scale.z = 0.25;

         // Set the color
         marker.color.r = 0.0f;
         marker.color.g = 1.0f;
         marker.color.b = 0.0f;
         marker.color.a = 1.0;

         marker.lifetime = ros::Duration();
	}

  //Function to publish marker
  void PublishMarker()
  {
   
    pub_.publish(marker);

  }

  //Callback function for the /marker_status topic
  void marker_status_callback(const std_msgs::String msg)
  {
      if (msg.data == "Pickup")
	{
	   
         //Set marker to pick up position
         this->SetMarkerPosition(2.8, -1.8, 0.0);
         ROS_INFO("Marker at Target.");

	}

      //Toggle to invisible to simulate pick up
      else if (msg.data == "Invisible")
       {
	 this->ToggleVisibility();
          ROS_INFO("Marker hidden.");
       }

      //Show marker at dop off area
      else
     {
	this->SetMarkerPosition(0, -1.6, 0.0);
        this->ToggleVisibility();
        ROS_INFO("Marker at DropOff.");
     }

      pub_.publish(marker);
      ros::Duration(5.0).sleep();

  }


private:
  //Private variables
  ros::NodeHandle n_; 
  ros::Publisher pub_;
  ros::Subscriber sub_status;


  //Function to toggle marker visibility
  void ToggleVisibility(){
   if(marker.color.a == 1.0){
      marker.color.a = 0.0;
     }
   else
     marker.color.a = 1.0;
  }

  //Function to change position of marker
  void SetMarkerPosition (double x,double y, double z){

    // Set the pose of the marker
    marker.pose.position.x = x;
    marker.pose.position.y = y;
    marker.pose.position.z = z;
  }
 
};

int main( int argc, char** argv )
{

    ros::init(argc, argv, "add_markers");

    //Call AddMarkers constructor
    AddMarkers newAddMarkers;


   ros::spin();

   return 0;

}
