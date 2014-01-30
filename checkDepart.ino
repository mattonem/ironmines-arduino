/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <std_msgs/Bool.h>

int clock = 0;

//Ici on declare nos constante pour le programme
const int startConnection = 2;
int startConnectionState = LOW;

ros::NodeHandle  nh;

//Ici on peut declarer de nouveau messages si besoin
std_msgs::Bool bool_true;
std_msgs::Bool bool_false;


//Ici on declare le topic publisher ou subscriber
ros::Publisher started("startTrigger", &bool_false);

//On peut ainsi prevoir le comportement de larduino sur un iteration
//Chaque action a sa propre fonction
void checkDepart()
{
  if(clock%10 != 0)
    return;
    
  if (digitalRead(startConnection) != startConnectionState) {
    startConnectionState = digitalRead(startConnection);
    if(startConnectionState == HIGH){
      started.publish( &bool_true );
    }else{
      started.publish( &bool_false );
    }
  }
}


void setup()
{
  nh.initNode();
  //On doit declarer chaque publisher au pres de ROS
  nh.advertise(started);
  
  //On initialise les messages
  bool_true.data = true;
  bool_false.data = false;
}

void loop()
{
  clock ++;
  checkDepart();
  nh.spinOnce();
  delay(1);
}
