
#include <ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>
#include <MemoryFree.h>

int clock = 0;

//Ici on declare nos constante pour le programme
const int startConnection = 2;
int startConnectionState = LOW;

ros::NodeHandle  nh;

//Ici on peut declarer de nouveau messages si besoin
std_msgs::Bool bool_true;
std_msgs::Bool bool_false;
std_msgs::Int16 free_memory;


//Ici on declare le topic publisher ou subscriber
ros::Publisher started("startTrigger", &bool_false);
ros::Publisher freememory("freeMemory", &free_memory);

//On peut ainsi prevoir le comportement de larduino sur un iteration
//Chaque action a sa propre fonction
void checkDepart()
{
  if(clock%10 != 0)
    return;
    
  if (digitalRead(startConnection) == HIGH) {
    startConnectionState = HIGH;
    started.publish( &bool_true );
  }
}

void memory()
{
  free_memory.data = freeMemory() ;
  freememory.publish(&free_memory);
}


void setup()
{
  nh.initNode();
  //On doit declarer chaque publisher au pres de ROS
  nh.advertise(started);
  nh.advertise(freememory);
  
  //On initialise les messages
  bool_true.data = true;
  bool_false.data = false;
}

void loop()
{
  clock ++;
  checkDepart();
  memory();
  nh.spinOnce();
  delay(1);
}
