#include "ros/ros.h"
#include "std_msgs/String.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cmath>

int fd;
int currPosition[4];
int current; // Current part
int idx; // # of servo that's gonna move
int speed; // Speed
int rate; // How big is an interval
int interval; // How many parts there are 
bool postiveMovement; // Whether it's moving toward larger number or not

// Setup the initizl behavior
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

  int setupSpeed = 5000;
  int setupPosition = 1500;
  int thirdPosition = 800;
  for (int i = 0; i < 4; i++) {
    if (i == 3) {
      currPosition[i] = thirdPosition;
      setupPosition = thirdPosition;
    } else {
      currPosition[i] = setupPosition;
      std::stringstream input;
      input << "#" << i << " P" << setupPosition << " S" << setupSpeed;

      // Writes to the file
      const char *cInput = input.str().c_str();
      int length = strlen(cInput);
      char *buff = strndup(cInput, length); 
      buff[length - 1] = '\r';
      int written = write(fd, buff, length);
      free(buff);

      // Sleep
      usleep(200);
    }
  }
}

void send(const std_msgs::String::ConstPtr& msg) {
	ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "arm_controller_sub");
  setup(); // Set up all the attributes
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("chatter", 1000, send);
  ROS_INFO("Ready to perform actions");
  ros::spin();
  return 0;
}
