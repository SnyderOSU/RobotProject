/********************************/
/*      Proteus Robot Code      */
/*     OSU FEH Spring 2024      */
/*        Evan Snyder           */
/*    02/28/24  Version 1.0.0   */
/********************************/


/* Include preprocessor directives */
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>
#include <FEHRCS.h>

#define DriveWheelDiameter 3
#define Pi 3.14
#define degreeConvert 0.018362174
#define blueval 1
#define redval 0

#define SERVO_MIN 500
#define SERVO_MAX 1900

FEHServo servo(FEHServo::Servo0);

FEHMotor left_motor(FEHMotor::Motor0,9.0);
FEHMotor right_motor(FEHMotor::Motor2,9.0);
AnalogInputPin CdS_cell(FEHIO::P2_3);

DigitalEncoder right_encoder(FEHIO::P0_1);
DigitalEncoder left_encoder(FEHIO::P0_3);

//positive percent is to the right
void rotateAprox(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts()+right_encoder.Counts())/2<=counts)
    {

    }

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void rotateDegree(int percent, int degree) //using encoders
{
    int counts=318*degreeConvert*degree;
    LCD.WriteLine(counts);
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts()+right_encoder.Counts())/2<=counts)
    {
    LCD.WriteLine(left_encoder.Counts());
    LCD.WriteLine(right_encoder.Counts());
    }
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}
void rotateTime(int percent, float time) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    Sleep(time);
    //While the average of the left and right encoder is less than counts,
    //keep running motors
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}


void driveTime(int percent,float time)
{
    left_motor.SetPercent(percent);
    right_motor.SetPercent(-percent);
    Sleep(time);
    left_motor.Stop();
    right_motor.Stop();
}
void driveInches(int percent, float inches) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent

    int counts = (inches/(DriveWheelDiameter*Pi))*318;
    LCD.Clear();
    LCD.Write("Driving Distance: ");
    LCD.WriteLine(inches);
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);
    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while(((left_encoder.Counts()+right_encoder.Counts())/2)<=counts)
    {
    LCD.WriteLine(" ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.WriteLine(right_encoder.Counts());
    }
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void MoveDistance(float dist)
{
    //motor ramping
    LCD.WriteLine("Performing Ramping Up");
    for(int i =0;i<=50;i++)
    {
        left_motor.SetPercent(i);
        right_motor.SetPercent(-i);
        LCD.Clear();
        LCD.WriteLine("Motor %: "+i);
        Sleep(.25);
    }
        LCD.WriteLine("Performing Ramping Down");
        for(int i =50;i>=0;i--)
    {
        left_motor.SetPercent(i);
        right_motor.SetPercent(-i);
        LCD.Clear();
        LCD.WriteLine("Motor %: "+i);
        Sleep(.25);
    }
    LCD.WriteLine("Stopping");
    left_motor.Stop();
    right_motor.Stop();
}

void RightMotorPower(int power)
{
    right_motor.SetPercent(-power);
}
void LeftMotorPower(int power)
{
    left_motor.SetPercent(power);
}

void RotateRight(double time)
{

    LCD.Clear();
    LCD.WriteLine("Rotating Right");
    RightMotorPower(-40);
    LeftMotorPower(40);
    Sleep(time);
    LCD.Clear();
    LCD.WriteLine("Done Rotating");
        RightMotorPower(0);
    LeftMotorPower(0);
}

void RotateLeft(double time)
{
    LCD.Clear();
    LCD.WriteLine("Rotating Left");
    RightMotorPower(40);
    LeftMotorPower(-40);
    Sleep(time);
    LCD.Clear();
    LCD.WriteLine("Done Rotating");
        RightMotorPower(0);
    LeftMotorPower(0);
}

void Drive(int direction,double time)
{
    LCD.Clear();
    LCD.WriteLine("Move Forward");
    RightMotorPower(direction*40);
    LeftMotorPower(direction*40);
    Sleep(time);
    LCD.Clear();
    LCD.WriteLine("Done Moving Forward");
    RightMotorPower(0);
    LeftMotorPower(0);
}
void DriveForward(double time)
{
    LCD.Clear();
    LCD.WriteLine("Move Forward");
    RightMotorPower(40);
    LeftMotorPower(40);
    Sleep(time);
    LCD.Clear();
    LCD.WriteLine("Done Moving Forward");
    RightMotorPower(0);
    LeftMotorPower(0);
}
void DriveBackward(double time)
{
    LCD.Clear();
    LCD.WriteLine("Move Backward");
    RightMotorPower(-40);
    LeftMotorPower(-40);
    Sleep(time);
    LCD.Clear();
    LCD.WriteLine("Done Moving Backward");
    RightMotorPower(0);
    LeftMotorPower(0);
}

void DriveTurnRight(double time,int ratio)
{
    LCD.Clear();
    LCD.WriteLine("Move Forward Partial Right");
    RightMotorPower(40/ratio);
    LeftMotorPower(40);
    Sleep(time);
    LCD.Clear();
    LCD.WriteLine("Done Moving Forward");
    RightMotorPower(0);
    LeftMotorPower(0);
}
void DriveTurnLeft(double time,int ratio)
{
    LCD.Clear();
    LCD.WriteLine("Move Forward Partial Left");
    RightMotorPower(40);
    LeftMotorPower(40/ratio);
    Sleep(time);
    LCD.Clear();
    LCD.WriteLine("Done Moving Forward");
    RightMotorPower(0);
    LeftMotorPower(0);
}
void DriveBackTurnLeft(double time,int ratio)
{
    LCD.Clear();
    LCD.WriteLine("Move Backward Partial Left");
    RightMotorPower(-40);
    LeftMotorPower(-40/ratio);
    Sleep(time);
    LCD.Clear();
    LCD.WriteLine("Done Moving Backward");
    RightMotorPower(0);
    LeftMotorPower(0);
}
void DriveBackTurnRight(double time,int ratio)
{
    LCD.Clear();
    LCD.WriteLine("Move Backward Partial Right");
    RightMotorPower(-40/ratio);
    LeftMotorPower(-40);
    Sleep(time);
    LCD.Clear();
    LCD.WriteLine("Done Moving Backward");
    RightMotorPower(0);
    LeftMotorPower(0);
}
void calibrateArmAtStart()
{
    servo.SetMin(SERVO_MIN);
    servo.SetMax(SERVO_MAX);
    //servo.SetDegree(179);
    //Sleep(1.0);
    //servo.SetDegree(1);
    //Sleep(1.0);
}
/* Main function to control menu system */

//1 sec is about 45 degrees

//Identify number B3eWV1yBh

//1150 is a 180 turn rights
int main(void)
{
    calibrateArmAtStart();


    //RCS.InitializeTouchMenu("B3eWV1yBh");
    //Get correct lever from the RCS
    //int correctLever = RCS.GetCorrectLever();
    int correctLever = 0;
    LCD.Clear();
    LCD.Write(correctLever);

    while (CdS_cell.Value()>2.75)
    {
       LCD.WriteLine(CdS_cell.Value());
    }

    LCD.WriteLine("Starting");
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();

    LCD.Clear();
    LCD.WriteLine("Starting"); 
    /*
    move to luggage drop
    driveInches(50,6);s
    Sleep(.2);
    left_motor.SetPercent(50);
    right_motor.SetPercent(-20);
    Sleep(2.5);
    right_motor.SetPercent(-50);
    Sleep(2.0);
    left_motor.Stop();
    right_motor.Stop();
    */
    servo.SetDegree(80);
    Sleep(1.0);

    double leftPower = 17;
    double rightPower = 50;
    float x_position, y_position;
    /*
    while(true)
    {
    LCD.ClearBuffer();
    LCD.Touch(&x_position, &y_position);
    while(x_position<250)
   { 
    LCD.ClearBuffer();
    Sleep(.1);
    if(LCD.Touch(&x_position, &y_position)){
        LCD.Clear();
        LCD.WriteLine(y_position);
        if(x_position<125)
        {
            leftPower--;
        }
        else if(x_position<250)
        {
            leftPower++;
        }
        LCD.WriteLine(leftPower);
    }
    LCD.ClearBuffer();
    Sleep(.5);
    x_position=20;
    LCD.Touch(&x_position, &y_position);
   }
       while(x_position<250)
   { 
    LCD.ClearBuffer();
    Sleep(.1);
    if(LCD.Touch(&x_position, &y_position)){
        LCD.Clear();
        if(x_position<125)
        {
            rightPower--;
        }
        else if(x_position<250)
        {
            rightPower++;
        }
        LCD.WriteLine(rightPower);
    }
    LCD.Touch(&x_position, &y_position);
   }
   LCD.Clear();
   rightPower=rightPower*-1;
   LCD.WriteLine("Left Motor Power");
   LCD.WriteLine(leftPower);
   LCD.WriteLine("Right Motor Power");
   LCD.WriteLine(rightPower);
   
   Sleep(5.0);
   */
  driveInches(70,30);
  servo.SetDegree(170);
  Sleep(1.0);
  driveInches(50,4);
  servo.SetDegree(130);
  Sleep(1.0);
  driveInches(50,10);
  Sleep(1.0);
  driveInches(-50,10);
  Sleep(60.0);
    rightPower=rightPower*-1;
    right_motor.SetPercent(rightPower);
    left_motor.SetPercent(leftPower);
    Sleep(.85);
    left_motor.Stop();
    right_motor.Stop();
    Sleep(1.0);
    driveInches(50,correctLever*2.0);
    servo.SetDegree(170);
    Sleep(1.0);
    servo.SetDegree(80);
    Sleep(1.0);
    double time = TimeNowSec();
    driveInches(-50,4.0+correctLever*2.0);
    servo.SetDegree(175);
    Sleep(1.0);
    driveInches(50,4.0+correctLever*2.0);
    while((TimeNowSec()-time)<5.0);
    servo.SetDegree(1);
    Sleep(1.0);
    servo.SetDegree(175);
    Sleep(1.0);
    driveInches(-50,12.0);
    rotateDegree(50,30);
    driveInches(50,40);

    //driveInches(-50,6);
    //Sleep(1.0);
    //rotateDegree(-50,90);
  

    //hi
}

