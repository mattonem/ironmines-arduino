/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <LinkedList.h>
#include <std_msgs/Bool.h>
#include <std_msgs/UInt16.h>

int clock = 0;

//Ici on declare nos constante pour le programme
const int startConnection = 8;
int startConnectionState = LOW; 
//Les pins 2-7 sont pour les capteurs ultrason

ros::NodeHandle  nh;

//Ici on peut declarer de nouveau messages si besoin
std_msgs::Bool bool_true;
std_msgs::Bool bool_false;
std_msgs::UInt16 aSonar;


//Ici on declare le topic publisher ou subscriber
ros::Publisher started("startTrigger", &bool_false);
ros::Publisher sonar0("sonnars/0", &aSonar);
ros::Publisher sonar1("sonnars/1", &aSonar);
ros::Publisher sonar2("sonnars/2", &aSonar);
ros::Publisher sonar3("sonnars/3", &aSonar);
ros::Publisher sonar4("sonnars/4", &aSonar);
LinkedList<ros::Publisher* > sonars;

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

void checkAllSonars()
{
  for(int i = 0 ; i < sonars.size() ; i++)
  {
    aSonar.data = readSonar(2+i);
    
    sonars.get(i)->publish(&aSonar);
  }
}

long readSonar(int pingPin)
{
  long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH, 20000);
  
  return microsecondsToMilimeters(duration);
};

long microsecondsToMilimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds * 10 / 29 / 2;
}



void setup()
{
  nh.initNode();
  //On doit declarer chaque publisher au pres de ROS
  nh.advertise(started);
  sonars = LinkedList<ros::Publisher*>();
  
  sonars.add(&sonar0);
  sonars.add(&sonar1);
  sonars.add(&sonar2);
  sonars.add(&sonar3);
  sonars.add(&sonar4);
  
  for(int i = 0 ; i < sonars.size() ; i++)
  {
    nh.advertise(*sonars.get(i));
  }
  
  pinMode(startConnection, INPUT);
  
  //On initialise les messages
  aSonar.data = 0;
  bool_true.data = true;
  bool_false.data = false;
}

void loop()
{
  clock ++;
  checkAllSonars();
  checkDepart();
  nh.spinOnce();
  
  delay(1);
}
