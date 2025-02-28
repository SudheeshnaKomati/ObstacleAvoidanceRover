#include <NewPing.h>
#include <Servo.h>
#include <AFMotor.h>

#define trigPin A1
#define echoPin A2
#define maxDist 300
#define colDist 30
#define maxSpeed 70

Servo s;

NewPing sonar(trigPin, echoPin, maxDist);  //ping library for measuring dist from sensor...

AF_DCMotor rMotor1(1, MOTOR12_1KHZ);
AF_DCMotor rMotor2(2, MOTOR12_1KHZ);
AF_DCMotor lMotor1(3, MOTOR34_1KHZ);
AF_DCMotor lMotor2(4, MOTOR34_1KHZ);


int livDist = 0;                      //distance measured from SonarSensor...
int speedBuffer = 0;

//int caller=0; //flagging concept for turnAround f'n to minimize the code and use turnRight only with change in delay...

void setup() 
{
    Serial.begin(9600);  
    s.attach(10);

    s.write(0);
    delay(1000);

    s.write(180);
    delay(1000);

    s.write(90);
    delay(1000);

    livDist = readPing();
    delay(100);
    livDist = readPing();
    delay(100);
    livDist = readPing();
    delay(100);
    livDist = readPing();
    delay(100);
}

void loop() 
{
    s.write(90);
    delay(10);

    if(livDist <= colDist)
    {
      stallStop();
      delay(100);   //for  0.1 s it'll stop
      changePath();
    }
    else
    {
      moveForward();
    }

    livDist = readPing();
}

//-------------------------------------ChangePath------------------------------------

void changePath()
{
  int leftDist = 0, rightDist = 0;

  stallStop();
  delay(100);
  moveBackward();   //for 0.3s it'll move back
  delay(300);
  stallStop();
  delay(200);

  s.write(150);
  delay(500);
  leftDist = readPing();
  delay(100);
  // Serial.println("Left_Distance = ");
  // Serial.print(leftDist);
  s.write(90);
  delay(200);

  s.write(30);
  delay(500);
  rightDist = readPing();
  delay(100);
  // Serial.println("Right_Distance = ");
  // Serial.print(rightDist);
  delay(200);

  s.write(90);
  delay(100);

  decideMotion(leftDist, rightDist);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-CompareDistances-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void decideMotion(int leftDist, int rightDist)
{
  if(leftDist <= rightDist)
  {
    turnRight();
    stallStop();
  }
  else
  {
    turnLeft();
    stallStop();
  }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-ReadDistancesFromSensor-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

int readPing()
{
    delay(70);
    int cm = sonar.ping_cm();

    if(cm == 0)
    {
      return maxDist;
    }
    return cm;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-MotionStop Speed=0-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void stallStop()
{
    rMotor1.run(RELEASE);
    rMotor2.run(RELEASE);
    lMotor1.run(RELEASE);
    lMotor2.run(RELEASE);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-Forward Locomotion-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void moveForward()
{
    rMotor1.run(FORWARD);
    rMotor2.run(FORWARD);
    lMotor1.run(FORWARD);
    lMotor2.run(FORWARD);

  
    for(speedBuffer = 0; speedBuffer < maxSpeed; speedBuffer += 2)
    {
      rMotor1.setSpeed(speedBuffer);
      rMotor2.setSpeed(speedBuffer);
      lMotor1.setSpeed(speedBuffer);
      lMotor2.setSpeed(speedBuffer);
    }
    
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-Backward Locomotion-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void moveBackward()
{
    rMotor1.run(BACKWARD);
    rMotor2.run(BACKWARD);
    lMotor1.run(BACKWARD);
    lMotor2.run(BACKWARD);

    for(speedBuffer = 0; speedBuffer < maxSpeed; speedBuffer += 2)
    {
      rMotor1.setSpeed(speedBuffer);
      rMotor2.setSpeed(speedBuffer);
      lMotor1.setSpeed(speedBuffer);
      lMotor2.setSpeed(speedBuffer);
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-Right Movement-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void turnRight()
{
  lMotor1.run(FORWARD);
  lMotor2.run(FORWARD);

  rMotor1.run(BACKWARD);
  rMotor2.run(BACKWARD);

  delay(500);

  rMotor1.run(FORWARD);
  rMotor2.run(FORWARD);
  lMotor1.run(FORWARD);
  lMotor2.run(FORWARD);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-Left Movement-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


void turnLeft()
{
  lMotor1.run(BACKWARD);
  lMotor2.run(BACKWARD);

  rMotor1.run(FORWARD);
  rMotor2.run(FORWARD);

  delay(500);
  
  rMotor1.run(FORWARD);
  rMotor2.run(FORWARD);
  lMotor1.run(FORWARD);
  lMotor2.run(FORWARD);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-TurnAround Locomotion phaseAngle(180 deg)-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void turnAround()
{
  lMotor1.run(BACKWARD);
  lMotor2.run(BACKWARD);

  rMotor1.run(FORWARD);
  rMotor2.run(FORWARD);
  delay(500);

  rMotor1.run(FORWARD);
  rMotor2.run(FORWARD);
  lMotor1.run(FORWARD);
  lMotor2.run(FORWARD);
}

