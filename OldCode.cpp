#include  "mbed.h"
#include "C12832.h"
#include "MMA7660.h"                                      
#include "QEI.h"

const float ratio = 0.75;
float compRatio = (1-ratio);

// Using Arduino pin notation
C12832 lcd (D11, D13, D12, D7, D10);
AnalogIn Pot1 (A0);
AnalogIn Pot2 (A1);
Serial pc(USBTX, USBRX);

InterruptIn PB_Center (D4);
InterruptIn PB_Right (A5);
InterruptIn PB_Up (A2);                                   // Interrupt for joystick up

DigitalOut BipolarLeft (PA_13);                           // Note: PA_13 and PA_14 are used for debug in keil
DigitalOut DirectionLeft (PA_14);
PwmOut Left (PA_15);

DigitalOut BipolarRight (PC_13);
DigitalOut DirectionRight(PC_14);
PwmOut Right(PB_7);

DigitalOut Enable (PC_15);

QEI RightEncoder(PC_11,PC_12,NC,624,QEI::X2_ENCODING);                       // The number (624) doesn't seem to have any effect as far as I can tell
QEI LeftEncoder(PC_6,PC_8,NC,624 ,QEI::X2_ENCODING);                        

Timer t1, t2, t3;

//MMA7660 MMA(D14,D15);

int delta = 0, intermediate = 1;
volatile int paused = 0;
volatile int state = 0;

float PrevPot1; // Used to measure changes in Pot
float PrevPot2;
float Pot1val;
float Pot2val;

float RightMotor;
float LeftMotor;

float Directionval;

void pause() {                                                          // Pauses and unpauses program
            paused = !paused;
            Enable=!Enable;   
            }

void stateChange()
{
    switch(state) {
        case 0 :
            state = 1;
            break;

        case 1 :
            state = 2;
            break;

        case 2:
            state = 0;
            break;

        default :
            state = 0;
    }
}            
//void ChangeBipolar_Right {} { }
  
               
////////////////////////// MAIN ////////////////////////
int main()
{

    PB_Center.rise(&pause);
    PB_Up.rise(&stateChange);                                       // Interrupt at the rising edge of push button being pressed
    Enable = 0;                             
    BipolarLeft = 0;
    BipolarRight = 0;
    DirectionLeft = 1;
    DirectionRight = 1;
    Right.period(0.000055);
    Left.period(0.000055);
    
    int RightPulses = 0;
    int RightPrevPulses = 0;
    int RightPPS = 0;                                            // Pulses per second
    
    int LeftPulses = 0;
    int LeftPrevPulses = 0;
    int LeftPPS = 0;
    
    PrevPot1 = 0;                                           // Used to measure changes in Pot1 & Pot 2
    PrevPot2 = 0;

    while (1) {
        if (paused == 1) { 
            lcd.locate(0,0);                                     // While loop entered when program is paused
            lcd.printf("Paused");
            while (paused == 1) {
            }
            lcd.cls();
        }

    if (state == 0) {
        lcd.locate(0,0);
        lcd.printf("Resting State");
        while (state == 0 && paused == 0) {
        }
        lcd.cls();
    }
    if (state == 1) {                                            // State for showing tasks 1 & 2, oscilloscope has to be connected to pwm channels
        lcd.locate(0,0);
        lcd.printf("First State");
        while (state == 1&& paused == 0) {
            // put the potentiometer code here
            RightMotor = 1 - Pot2.read();
            Right.write(RightMotor);                             // For some reason the motor is 100% at 0 and 0% at 1 e.g write(0) = 100% duty cycle
            LeftMotor = 1 - Pot1.read();
            Left.write(LeftMotor);   
            lcd.locate(0,9);                                       
            lcd.printf("Left: %3.0f%%", 100*(LeftMotor));
            lcd.locate(0,20);
            lcd.printf("Right: %3.0f%%", 100*(RightMotor));
        }
        lcd.cls();
        if (state == 2) {                                       // State for showing task 6
            lcd.locate(0,0);
            lcd.printf("Second State");
            while (state == 2&& paused == 0) {
                // put the forward, left right code here
                /*          t3.start();
                            while (t3.read() < 3){                // go forward for three seconds or something
                                forward code e.g:
                                if leftPPS > Right PPS ---> increase voltage on left motor...
                            }
                            t3.reset();
                            t3.start();
                            while (t3.read() < 3){
                                left code
                            }
                            t3.reset();
                            t3.start();
                            while (t3.read() < 3){
                                right code
                            }
                            t3.reset();
                            Enable = 0;  */
            }
        }

        /*t1.start();                                             // start the timer
        t2.start();
        
        Pot1val = Pot1.read();
        //printf("Pot1val = %f\r\n", Pot1val);              //Check if variable is reading from Potentiometer
        Pot2val = Pot2.read();
    
        RightPulses = RightEncoder.getPulses();              //     
        LeftPulses = LeftEncoder.getPulses();   
    
        if (paused == 1){                                       // While loop entered when program is paused                                           
            printf("Paused\r\n");
                while (1){
                    if (paused == 0){
                        printf("Unpaused\r\n");
                    break;
                    }
                }
            }
            
        if (PB_Right == 1) {
             (BipolarRight = !BipolarRight);
             }
                    
        
        //Speed
        RightMotor = Pot1val*ratio;                                         //Measurement from Potentiometer times "ratio" (0 to 1)
        LeftMotor = Pot1val*ratio;      
        
        // Direction
        Directionval = ((Pot2val-0.5) * compRatio);                      // Voltage in Centre of Pot changed from 0.5V to 0.0V and times complementary ratio (1 - ratio)
        RightMotor = RightMotor - Directionval;
        LeftMotor = LeftMotor + Directionval;  
        
        //printf("Right Motor = %f\t", RightMotor);            //Read Value going into Right Motor
        //printf("Left Motor = %f\r\n", LeftMotor);            //Read Value going into Left Motor
        
        Right.write(1-RightMotor);                              // For some reason the motor is 100% at 0 and 0% at 1 e.g write(0) = 100% duty cycle
        Left.write(1-LeftMotor);*/
        
        /*lcd.locate(0,3);                                    //Printing to lcd slows the process a bit
        lcd.printf("Left: %f", LeftMotor);
        lcd.locate(0,15);
        lcd.printf("Right: %f", RightMotor);
        lcd.locate(85,3);*/
        //pc.printf("PPS: %d\n\r", RightPPS);                  // Gets pulses From Right Encoder
        
        /*LeftPPS = (LeftPulses - LeftPrevPulses)/t1.read();
        lcd.locate(0,3);
        void cls();
        lcd.printf("Left PPS = %d", LeftPPS);
        LeftPrevPulses = LeftPulses;
        t1.reset();
        
        RightPPS = (RightPulses - RightPrevPulses)/t2.read();
        lcd.locate(0,20);
        lcd.printf("Right PPS = %d", RightPPS);
        RightPrevPulses = RightPulses;      
        t2.reset();*/ 

        }
    }
    }