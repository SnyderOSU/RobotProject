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

#define DriveWheelDiameter 1.65
#define Pi 3.14
#define degreeConvert 6
#define blueval 1
#define redval 0


FEHMotor left_motor(FEHMotor::Motor0,9.0);
FEHMotor right_motor(FEHMotor::Motor2,9.0);
AnalogInputPin CdS_cell(FEHIO::P2_3);

// Declarations for analog optosensors
AnalogInputPin right_opto(FEHIO::P2_0);
AnalogInputPin middle_opto(FEHIO::P2_1);
AnalogInputPin left_opto(FEHIO::P2_2);


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
    int counts=degreeConvert*degree;
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

void driveCounts(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts()+right_encoder.Counts())/2<=counts);
    LCD.WriteLine(left_encoder.Counts());
    LCD.WriteLine(right_encoder.Counts());
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
    LCD.WriteLine(left_encoder.Counts());
    LCD.WriteLine(right_encoder.Counts());
    }
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}
float moveWhileSensing(int percent,float inches)
{
    int counts = (inches/(DriveWheelDiameter*Pi))*318;
    LCD.Clear();
    int low = 4;
    left_motor.SetPercent(percent);
    right_motor.SetPercent(-percent);
    while(((left_encoder.Counts()+right_encoder.Counts())/2)<=counts)
    {
        float val = CdS_cell.Value();
        if (val<low)
        {
            low=val;
            LCD.Write("Predicted For: ");
            LCD.Write(low);
            if(low<.7)
            {
                LCD.SetFontColor(RED);
                LCD.WriteLine(": Red");
                LCD.SetFontColor(BLACK);
            }
            else{
                LCD.SetFontColor(BLUE);
                LCD.WriteLine(": Blue");
                LCD.SetFontColor(BLACK);
            }
        }
        
    }
    left_motor.Stop();
    right_motor.Stop();
    return low;

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
/* Main function to control menu system */

//1 sec is about 45 degrees

//1150 is a 180 turn rights
int main(void)
{


    left_encoder.ResetCounts();
    right_encoder.ResetCounts();

    while (CdS_cell.Value()>2.75)
    {
        LCD.WriteLine(CdS_cell.Value());
    }
    LCD.WriteLine("Starting");
    rotateDegree(50,45);
    driveInches(50,40);
    driveInches(-50,5);
    rotateDegree(-50,90);
    driveInches(50,12);
    rotateDegree(50,90);
    

    // do cds check
    float sensor=moveWhileSensing(50,20);
    if(sensor <.7)
    {
        LCD.SetFontColor(RED);
        LCD.WriteLine("Running Red");
        LCD.SetFontColor(BLACK);
        //run red
    }


    //Initialize the screen


}

