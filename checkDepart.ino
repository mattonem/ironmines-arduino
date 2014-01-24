/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <std_msgs/Bool.h>

const int startConnection = 2;
int startConnectionState = LOW;


ros::NodeHandle  nh;

std_msgs::Bool bool_msg;
ros::Publisher startTrigger("startTrigger", &bool_msg);

char hello[13] = "hello world!";

void checkDepart()
{
  if (startConnectionState == LOW && digitalRead(startConnection) == HIGH) {
    startConnectionState = HIGH;
    bool_msg.data = true;
    startTrigger.publish( &bool_msg );
  }
}

void setup()
{
  nh.initNode();
  nh.advertise(startTrigger);
}

void loop()
{
  bool_msg.data = false;
  checkDepart();
  nh.spinOnce();
  delay(100);
}
