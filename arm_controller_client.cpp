#include "ros/ros.h"
#include "arm_controller/Move.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "arm_controller_server");
  if (argc != 3)
  {
    ROS_INFO("usage: lynx_service_client Position Speed"); // #0 P1000 S500
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<arm_controller::Move>("arm_controller");
  
  arm_controller::Move srv;
  srv.request.index = atoll(argv[1]);
  srv.request.position = atoll(argv[2]);
  srv.request.speed = atoll(argv[3]);
  if (client.call(srv))
  {
    ROS_INFO(("response: " + srv.response.response).c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service arm_controller");
    return 1;
  }

  return 0;
}
