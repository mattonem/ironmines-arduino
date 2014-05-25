#include <ros.h>
#include <std_msgs/Bool.h>

int clock = 0;

//Ici on declare nos constante pour le programme
const int startConnection = 8;
const int canonConnection = 9;
int startConnectionState = LOW; 
//Les pins 2-7 sont pour les capteurs ultrason

ros::NodeHandle  nh;

//Ici on peut declarer de nouveau messages si besoin
std_msgs::Bool bool_true;
std_msgs::Bool bool_false;


//Ici on declare le topic publisher ou subscriber
ros::Publisher started("startTrigger", &bool_false);

void fire(const std_msgs::Bool& msg) 
{
  if(msg.data)
  {
      digitalWrite(canonConnection, HIGH);   // set the LED on
      delay(1000);              // wait for a second
      digitalWrite(canonConnection, LOW);    // set the LED off
      delay(1000);
  }
}

ros::Subscriber<std_msgs::Bool> canon("canonTrigger", fire);


//On peut ainsi prevoir le comportement de larduino sur un iteration
//Chaque action a sa propre fonction


void checkDepart()
{
    
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
  nh.subscribe(canon);

  
  pinMode(startConnection, INPUT);
  pinMode(canonConnection, OUTPUT);
  
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
