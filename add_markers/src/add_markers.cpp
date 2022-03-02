#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "nav_msgs/Odometry.h"

const float pickupZoneX = 0.7;
const float pickupZoneY = 1;
const float pickupZoneZ = 1;

const float  dropOffZoneX = -0.5;
const float  dropOffZoneY = -0.7;
const float  dropOffZoneZ = 1.0;


bool isPickupZone = false;
bool isDropoffZone = false;


void odom_callback(const nav_msgs::Odometry::ConstPtr &msg)
{
  float currentPoseX = msg -> pose.pose.position.x;
  float currentPoseY = msg -> pose.pose.position.y;

  float pickup_x_offset = abs (currentPoseX - pickupZoneX);
  float pickup_y_offset = abs (currentPoseY - pickupZoneY);


  float dropoff_x_offset = currentPoseX - dropOffZoneX;
  float dropoff_y_offset = currentPoseY - dropOffZoneY;


  if (pickup_x_offset < 0.2  && pickup_y_offset < 0.2 )
    isPickupZone = true;


  if (dropoff_x_offset < 0.2  && dropoff_y_offset < 0.2 )
    isDropoffZone = true;

}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber odom_sub = n.subscribe("/odom", 10, &odom_callback);
  visualization_msgs::Marker marker;



  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;
  enum State{pick,hide,drop}current_state;
  current_state=pick;


  while (ros::ok())
  {

    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();
    ros::spinOnce();

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }

    if (current_state == pick)
    {
      ROS_INFO("Picking up object...");
      marker.pose.position.x = pickupZoneX;
      marker.pose.position.y = pickupZoneY;
      marker.pose.orientation.w = pickupZoneZ;
      marker.action = visualization_msgs::Marker::ADD;
      marker_pub.publish(marker);
 
      if (isPickupZone){
      // Wait 5 seconds
      ros::Duration(5.0).sleep();
      current_state = hide;
     }
    }
    else if ( current_state == hide )
    {
      ROS_INFO("Pick up point reached and going to dropoff object...");
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);

      if (isDropoffZone){
      // Wait 5 seconds
      ros::Duration(2.0).sleep();
      current_state = drop;
       }
      }
    else if ( current_state == drop )
    {
      ROS_INFO("dropoff point reached...");
      marker.action = visualization_msgs::Marker::ADD;
      // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
      marker.pose.position.x = dropOffZoneX;
      marker.pose.position.y = dropOffZoneY;
      marker.pose.orientation.w = dropOffZoneZ;
      marker_pub.publish(marker);
    }
  }

}

