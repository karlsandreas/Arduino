#include <Servo.h>

const int pingPin = 7;
int largestdist = 0;
int midpointindex = 0;
const byte listsize = 90;
Servo myservo; 
Servo servoRight;
Servo servoLeft; 


int leftObj[listsize];
int rightObj[listsize];

int rightCount = 0;
int leftCount = 0;
  
int degree = 7;



void setup() 
{
 
  myservo.attach(11);
  myservo.write(45);
  delay(1000);

  servoRight.attach(12);
  servoLeft.attach(13);

  Serial.begin(9600);
  sweepDetect();
  delay(1000);
  if (rightCount > leftCount)
  {
    maneuver(-100,100,(abs((leftObj[(leftCount/2)])-90)*(degree)));
    Serial.print("Left Smaller, turns degree: ");
    Serial.println(abs((leftObj[0])-90));
  }
  if (leftCount > rightCount)
  {
    maneuver(100,-100,(abs((rightObj[(rightCount/2)]-90))*(degree)));
    Serial.print("Right Smaller, turns degree: ");
    Serial.println(abs((rightObj[0])-90));
  }
  maneuver(100,100,100);

}

void loop() 
{
  stopbyobj();
  
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void stopbyobj()
{
  Serial.println("In stop loop");
  myservo.write(90);
  delay(50);
  int dist = detect();
  Serial.println(dist);
  if (dist <10)
  {
    maneuver(0,0,-1);
    Serial.println("Stop by object");
  }
  
}

void sweepDetect()
{

  int firstdist = detect();
  for (int pos = 45; pos <= 135; pos += 1) 
  {                    

    myservo.write(pos); 
    delay(40);  
    int seconddist = detect();
    if (seconddist < 100)
    {
      Serial.println("First dist under 100cm");
        
      if ((-2 < (firstdist - seconddist) < 2))
      {
        if (pos<90)
        {
          rightObj[rightCount] = pos;
          rightCount += 1;
          Serial.print("Right obj: ");
          Serial.print("pos :");
          Serial.print(pos);
          Serial.print(" dist :");
          Serial.println(seconddist);
        }
        if (pos>90)
        {
          leftObj[leftCount] = pos;
          leftCount += 1;
          Serial.print("Left obj: ");
          Serial.print("pos :");
          Serial.print(pos);
          Serial.print(" dist :");
          Serial.println(seconddist);
        }
        else 
        {
          continue;
        }
        firstdist = seconddist;
      }
      else
      {
        continue;
      }
      
      
    }
    
  }
  Serial.print("Left count;");
  Serial.println(leftCount);
  Serial.print("Right count;");
  Serial.println(rightCount);
}

int detect()
{
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();
  return(cm);


  delay(100);
}

void maneuver(int speedLeft,int speedRight,int times)
{
  servoLeft.writeMicroseconds(1500 +speedLeft);
  servoRight.writeMicroseconds(1500 -speedRight);
  delay(times);
  if (times ==-1)
  {
    servoLeft.detach();
    servoRight.detach();
  }
}
