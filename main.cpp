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



/* Define time for beep */
//#define beep_t 10 // int milliseconds


FEHMotor left_motor(FEHMotor::Motor0,9.0);
FEHMotor right_motor(FEHMotor::Motor3,9.0);
AnalogInputPin CdS_cell(FEHIO::P2_0);

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
/* Main function to control menu system */

//1 sec is about 45 degrees
int main(void)
{
    while (CdS_cell.Value()>2.75)
    {
        LCD.WriteLine(CdS_cell.Value());
    }
    DriveForward(1.0);
    RotateRight(2.0);
    DriveForward(1.8);
    RotateLeft(1.0);
    Drive(1,10.0);
    RotateLeft(2.0);
    DriveForward(3.0);
    RotateRight(2.0);
    DriveForward(3.0);

    DriveBackward(5.0);
    DriveForward(1.0);
    RotateRight(2.0);
    DriveForward(4.0);
    DriveBackward(0.2);
    RotateRight(2.0);
    DriveForward(10.0);

    //RotateLeft(5.0);
   // RotateRight(5.0);
}

