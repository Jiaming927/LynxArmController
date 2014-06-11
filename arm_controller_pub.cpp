#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>


int main(int argc, char **argv) {
  ros::init(argc, argv, "arm_controller_pub");
  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000, 1);

  // Ros might not be able to respond that fast,
  // So we need some way to work around like this while loop
  ros::Rate poll_rate(100);
  while (chatter_pub.getNumSubscribers() == 0) {
    ROS_INFO("Waiting");
    poll_rate.sleep();
  }

  // Still takes the data
  std_msgs::String msg;
  std::stringstream ss;
  ss << argv[1] << " " << argv[2] << " " << argv[3];
  msg.data = ss.str();

  ROS_INFO("Published %s", msg.data.c_str());
  chatter_pub.publish(msg);
  return 0;
}


