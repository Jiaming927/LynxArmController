#include "ros/ros.h"
#include "arm_controller/Move.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <sstream>
int fd;

bool send(arm_controller::Move::Request  &req,
         arm_controller::Move::Response  &res)
{
  res.response = "Received";
  ROS_INFO("sending command: [Index: %ld, Position:%ld, Speed: %ld]", req.index, req.position, req.speed);
  
  std::stringstream input;
  input << "#" << req.index << " P" << req.position << " S" << req.speed;
  // Turn string into commands

  const char *cInput = input.str().c_str();
  int length = strlen(cInput);
  char *buff = strndup(cInput, length); 
  buff[length - 1] = '\r';
  int written = write(fd, buff, length);
  free(buff);
  return true;
}

void setup() {
  // get parameter, change ttyUSB0 at runtime
  fd = open("/dev/ttyUSB0", O_RDWR);
  struct termios options;

  if (fd == -1) {
    ROS_INFO("fd error");
  }
  tcgetattr(fd, &options);
  cfsetospeed(&options, B115200);
  cfsetispeed(&options, B115200);

  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag |= CS8;

  if (tcsetattr(fd, TCSANOW, &options) != 0) {
    ROS_INFO("apply setting error");
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "arm_controller_server");
  setup(); // Set up all the attributes
  ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("arm_controller", send);
  ROS_INFO("Ready to perform actions");
  ros::spin();

  return 0;
}
