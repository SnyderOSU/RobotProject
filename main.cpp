/********************************/
/*      Proteus Robot Code      */
/*     OSU FEH Spring 2024      */
/*        Evan Snyder           */
/*    4/15/24  Version 1.0.0   */
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
#define degreeConvertOld 0.018362174
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


//rotates to the right on positive percent and to the left on negative percent
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


void gradualTurn(int percentLeft,int percentRight, double time) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    LCD.Clear();
    right_motor.SetPercent(-percentRight);
    left_motor.SetPercent(percentLeft);
    //While the average of the left and right encoder is less than counts,
    //keep running motors
    Sleep(time);
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
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
            /*
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
            */
        }

    }
    left_motor.Stop();
    right_motor.Stop();
    return low;

}
/* Main function to control menu system */

//1 sec is about 45 degrees

//Identify number B3eWV1yBh

//1150 is a 180 turn rights
int main(void)
{
    calibrateArmAtStart();


    RCS.InitializeTouchMenu("B3eWV1yBh");
    //Get correct lever from the RCS
    int correctLever = RCS.GetCorrectLever();
    //int correctLever = 1;

    
    //int correctLever = 1;
    LCD.Clear();
    LCD.Write(correctLever);
    

    while (CdS_cell.Value()>2.75)
    {
       LCD.WriteLine(CdS_cell.Value());
       LCD.WriteLine("Final Action Done");
    }

    LCD.WriteLine("Starting");
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();

    //hit start button
    driveInches(-50,2);
    Sleep(.1);
    driveInches(50,2);

    // move to luggage drop
    //was 4.5
    driveInches(50,4.5);
    Sleep(.2);
    left_motor.SetPercent(50);
    right_motor.SetPercent(-20);
    Sleep(2.5);
    right_motor.SetPercent(-50);
    Sleep(1.0);
    left_motor.Stop();
    right_motor.Stop();
    servo.SetDegree(90);
    Sleep(1.0);
    //old values -50 -15 1.8
    //can lower to -50 -14 1.8
    gradualTurn(-50,-14,1.8);
    driveInches(-50,10);
    //final step for luggage over

    driveInches(50,9);
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
    driveInches(-50,correctLever*2.0);
    //up agaisnt wall
    //driveInches(-50,12.0);
    gradualTurn(-50,-15,2);
    //driveInches(50,10);
    servo.SetDegree(2);
    Sleep(1.0);
    //driving up ramp
    driveInches(-50,15.5);
    gradualTurn(-50,-6,.56);
    //done with gradual
    driveInches(-50,15);
    Sleep(1.0);
    float val = moveWhileSensing(-50,30);
    LCD.WriteLine(val);
    Sleep(3.0);
    driveInches(50,20);
    Sleep(1.0);
    gradualTurn(-50,-4,.5);
    Sleep(1.0);
    //was 20
    driveInches(-50,27);
    Sleep(1.0);
    //test default >.7 = blue
    //int val=.8;
    LCD.WriteLine(val);

    //deafults to blue
    val = .8;
    if (val<.7)
    {
        //drive to far button which is red
        LCD.SetFontColor(RED);
        LCD.WriteLine(": Red");
        LCD.SetFontColor(BLACK);
        Sleep(1.0);
        driveInches(50,10);
        
    }
    else
    {
        LCD.SetFontColor(BLUE);
        LCD.WriteLine(": Blue");
        LCD.SetFontColor(BLACK);
        Sleep(1.0);
        //was 5
        driveInches(50,4);
    }
    Sleep(2.0);
    //was 1.0
    gradualTurn(-7,50,1.2);
    Sleep(1.0);
    //was 15
    driveInches(60,20);
    Sleep(1.0);
    gradualTurn(7,-50,1.5);
    Sleep(1.0);
    driveInches(-50,15);
    Sleep(1.0);
    //now for passport
    //was 29 50 1.8
    gradualTurn(25,50,1.8);
    Sleep(.5);
    driveInches(50,10);
    //right_motor.SetPercent(-50);
    //Sleep(.5);
    //right_motor.Stop();
    driveInches(-50,6);
    servo.SetDegree(179);
    Sleep(2.0);
    driveInches(50,2);
    Sleep(1.0);
    driveInches(-50,.4);
    servo.SetDegree(140);
    Sleep(2.0);
    //was -50
    right_motor.SetPercent(-75);
    Sleep(1.0);
    right_motor.Stop();
    servo.SetDegree(110);
    Sleep(1.0);
    right_motor.SetPercent(-50);
    left_motor.SetPercent(10);
    Sleep(1.0);
    left_motor.Stop();
    right_motor.Stop();
    Sleep(1.0);
    servo.SetDegree(80);
    Sleep(1.0);
    driveInches(50,1);
    Sleep(1.0);
    driveInches(-50,5);
    servo.SetDegree(2);
    //now get back to start
    //gradualTurn(50,30,2);
    left_motor.SetPercent(30);
    Sleep(1.7);
    left_motor.Stop();
    gradualTurn(30,50,3);
    driveInches(50,25);
    Sleep(1.0);
    //was -50
    //right_motor.SetPercent(-80);
    //Sleep(1.0);
    //right_motor.Stop();
    Sleep(1.0);
    driveInches(-50,.2);
    gradualTurn(7,-50,1);
    Sleep(1.0);
    driveInches(-50,4);
    Sleep(1.0);
    driveInches(60,38);
    right_motor.SetPercent(-75);
    Sleep(2.0);
    right_motor.Stop();
    Sleep(1.0);
    right_motor.SetPercent(60);
    Sleep(1.0);
    driveInches(60,10);
    while (true)
    {
        driveInches(-50,3);
        gradualTurn(50,45,1);
        driveInches(60,3);
        right_motor.SetPercent(-75);
        Sleep(1.0);
        right_motor.Stop();
    }

}

