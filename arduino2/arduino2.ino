#include <LinkedList.h>
#include <ros.h>
#include <std_msgs/UInt16.h>

int clock = 0;

ros::NodeHandle nh;

std_msgs::UInt16 aSonar;

ros::Publisher sonar0("sonnars/0", &aSonar);
ros::Publisher sonar1("sonnars/1", &aSonar);
ros::Publisher sonar2("sonnars/2", &aSonar);

LinkedList<ros::Publisher* > sonars;

void checkAllSonars()
{
  for(int i = 0 ; i < sonars.size() ; i++)
  {
    aSonar.data = readSonar(11-i);
    
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
}


long microsecondsToMilimeters(long microseconds)
{
  return microseconds * 10 / 29 / 2;
}


void setup()
{
  nh.initNode();
  //On doit declarer chaque publisher au pres de ROS
  sonars = LinkedList<ros::Publisher*>();
  
  sonars.add(&sonar0);
  sonars.add(&sonar1);
  sonars.add(&sonar2);
  
  for(int i = 0 ; i < sonars.size() ; i++)
  {
    nh.advertise(*sonars.get(i));
  }
  
  
  //On initialise les messages
  aSonar.data = 0;
}

void loop()
{
  checkAllSonars();
  nh.spinOnce();
  
  delay(1);
}
