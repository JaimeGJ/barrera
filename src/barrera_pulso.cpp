#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Byte.h"
#include <iostream>
#include "wiringPi.h"


#define PIN_ABRIR 0   //BCM 17, pin 11
#define PIN_CERRAR 1  //BCM 18, pin 12
#define IS_OPEN 2    //BCM 27, pin 13
#define IS_CLOSED 3  //BCM 22, pin 15


 
void callback(const std_msgs::Int8::ConstPtr& msg)  //Barrier opens with a solicitude of a pulse
{
	std_msgs::Byte estado;
	ros::NodeHandle nh;
	ros::Publisher pub = nh.advertise<std_msgs::Byte>("/barrera_estado", 10);
  if (digitalRead(IS_CLOSED)){
	if(msg->data == 1){
		ROS_INFO("La barrera se está abriendo");
		digitalWrite (PIN_ABRIR, HIGH);
		digitalWrite (PIN_CERRAR, LOW);
		while ( !digitalRead(IS_OPEN)){
			digitalRead(IS_OPEN);
		}
		digitalWrite (PIN_ABRIR, LOW);
		ROS_INFO("La barrera está abierta");
 		estado.data=1;	
		pub.publish(estado);
  	}
  }
  if (digitalRead(IS_OPEN)){
	if(msg->data == 2){
		ROS_INFO("La barrera se está cerrando");
		digitalWrite (PIN_CERRAR, HIGH);
		digitalWrite (PIN_ABRIR, LOW);
		while ( !digitalRead(IS_CLOSED)){
			digitalRead(IS_CLOSED);
		}
		digitalWrite (PIN_CERRAR, LOW);
		ROS_INFO("La barrera está cerrada");
 		estado.data=0;	
		pub.publish(estado);

  	}
  }
  
}

int main(int argc, char** argv)
{
	ros::init(argc,argv,"barrera_nodo");
    ROS_INFO("El nodo barrera_nodo ha sido lanzado");

    wiringPiSetup();
    pinMode(PIN_ABRIR, OUTPUT);
    pinMode(PIN_CERRAR, OUTPUT);
    pinMode(IS_OPEN, INPUT);
    pinMode(IS_CLOSED, INPUT);

    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("barrera_solicitud",10,callback);
    ros::Publisher pub = n.advertise<std_msgs::Byte>("/barrera_estado", 10);
    
    std_msgs::Byte estado_inicial;
        
    if (digitalRead(IS_OPEN) == 0 && digitalRead(IS_CLOSED) == 0){
	  ROS_INFO("La barrera se está cerrando por seguridad");
	  digitalWrite (PIN_CERRAR, HIGH);
	  digitalWrite (PIN_ABRIR, LOW);
	  while ( !digitalRead(IS_CLOSED)){
			digitalRead(IS_CLOSED);
	  }
	  digitalWrite (PIN_CERRAR, LOW);
	  ROS_INFO("La barrera se ha cerrado por seguridad");
	  estado_inicial.data=0;	
	  pub.publish(estado_inicial);
   }
  

	
    
    ros::spin();
    return 0;

}
