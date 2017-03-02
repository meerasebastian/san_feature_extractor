#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>
#include<nav_msgs/Odometry.h>

std::string turtle_name;



void poseCallback(const nav_msgs::Odometry::ConstPtr& msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->pose.pose.position.x, msg->pose.pose.position.y, 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, msg->twist.twist.angular.z);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "robot_1/odom"));
}

int main(int argc, char** argv){
  ros::init(argc, argv, "san_missing_tf_broadcaster");

  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("robot_1/odom", 10, &poseCallback);

  ros::spin();
  return 0;
};
