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

bool send(arm_controller::Move::Request  &req,
         arm_controller::Move::Response  &res)
{
  res.response = "Received";
  ROS_INFO("sending command: [idx: %ld, Position:%ld, Speed: %ld]", req.index, req.position, req.speed);
  
  current = 0;
  idx = req.index;
  speed = req.speed;
  interval = abs(req.position - currPosition[idx]);
  int start = currPosition[idx];

  if (req.position - start > 0) {
    postiveMovement = true;
  } else {
    postiveMovement = false;
  }
      
  currPosition[idx] = req.position;
  return true;
  // std::stringstream input;
  // input << "#" << idx << " P" << position << " S" << speed;
  // // Turn string into commands

  // const char *cInput = input.str().c_str();
  // int length = strlen(cInput);
  // char *buff = strndup(cInput, length); 
  // buff[length - 1] = '\r';
  // int written = write(fd, buff, length);
  // free(buff);
}

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

  // MEH
  // arm_controller::Move::Request req;
  // req.position = 800;
  // req.speed = 2000;

  // arm_controller::Move::Response res;
  // res.response = "";

  // Sets the initial value
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


int main(int argc, char **argv)
{
  ros::init(argc, argv, "arm_controller_server");
  setup(); // Set up all the attributes
  ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("arm_controller", send);
  ROS_INFO("Ready to perform actions");

  // Non-blocking solution
  while (1) {
      if (current != interval) {
        ROS_INFO("NUM: %d, idx: %d, SPEED: %d",currPosition[idx], idx, speed);

        current++;
        if (postiveMovement) {
          currPosition[idx] += rate;
        } else {
          currPosition[idx] -= rate;
        }

        std::stringstream input;
        input << "#" << idx << " P" << currPosition[idx] << " S" << speed;
        
        // Writes to the file
        const char *cInput = input.str().c_str();
        int length = strlen(cInput);
        char *buff = strndup(cInput, length); 
        buff[length - 1] = '\r';
        int written = write(fd, buff, length);
        free(buff);

        usleep(200);
      }
    ros::spinOnce();
  }

  return 0;
}
