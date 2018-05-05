#include "ros/ros.h"
#include "std_msgs/Int8.h"

#include <iostream>
#include "wiringPi.h"


#define LED_OPEN 0   //BCM 17, pin 11
#define LED_CLOSE 1  //BCM 18, pin 12
#define IS_OPEN 2    //BCM 27, pin 13
#define IS_CLOSED 3  //BCM 22, pin 15

void callback(const std_msgs::Int8::ConstPtr& msg)  //Barrier opens with a solicitude of a pulse
{
  if (digitalRead(IS_CLOSED)){
	if(msg->data == 1){
		digitalWrite (LED_OPEN, HIGH);
		digitalWrite (LED_CLOSE, LOW);
		ROS_INFO("Barrier is opening");
  	}
  }
  if (digitalRead(IS_OPEN)){
	if(msg->data == 2){
		digitalWrite (LED_CLOSE, HIGH);
		digitalWrite (LED_OPEN, LOW);
		ROS_INFO("Barrier is closing");
  	}
  }
  if (digitalRead(IS_OPEN) == 0 && digitalRead(IS_CLOSED) == 0){
	digitalWrite (LED_CLOSE, HIGH);
	digitalWrite (LED_OPEN, LOW);
	ROS_INFO("Barrier is closing for security");
  }
}

int main(int argc, char** argv)
{
    ros::init(argc,argv,"Barrier_node");
    ROS_INFO("Started Barrier_node");

    wiringPiSetup();
    pinMode(LED_OPEN, OUTPUT);
    pinMode(LED_CLOSE, OUTPUT);
    pinMode(IS_OPEN, INPUT);
    pinMode(IS_CLOSED, INPUT);

    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("barrera_solicitud",10,callback);

    ros::spin();
}
