#include  "mbed.h"
#include "C12832.h"
#include "MMA7660.h"                                      
#include "QEI.h"


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

MMA7660 MMA(D14,D15);

int delta = 0, intermediate = 1;
volatile int paused = 1;
volatile int state = 0;

float PrevPot1, PrevPot2;        // Used to measure changes in Pot
float Pot1val;
float Pot2val;

float RightMotor;
float LeftMotor;

float Directionval;

float vr, vl;

float turn = 0;

int ErrR = 0, ErrL = 0;
 
float K;

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

    PB_Right.rise(&pause);
    PB_Up.rise(&stateChange);                                       // Interrupt at the rising edge of push button being pressed
    Enable = 0;                             
    BipolarLeft = 0;
    BipolarRight = 0;
    DirectionLeft = 0;
    DirectionRight = 0;
   
    Right.period(0.000055);
     Left.period(0.000055);
    
    int RightPulses = 0;
    int RightPrevPulses = 0;
    int RightPPS = 0;                                            // Pulses per second
    
    int LeftPulses = 0;
    int LeftPrevPulses = 0;
    int LeftPPS = 0;
    
    PrevPot1 = 0;                                           // Measures changes in Pot1 & Pot 2
    PrevPot2 = 0;
    
    while (1) {
        
        Pot1val = Pot1.read();
        Pot2val = Pot2.read();
        
        RightPulses = RightEncoder.getPulses();              //     
        LeftPulses = LeftEncoder.getPulses(); 
        
        if (paused == 1) {
            Enable = 0; 
            lcd.locate(0,0);                                     // While loop entered when program is paused
            lcd.printf("Paused");
            while (paused == 1) {
            }                                               
            lcd.cls();
        }
        

    if (state == 0) {
        Enable = 0;
        lcd.locate(0,0);
        lcd.printf("Resting State");
        while (state == 0 && paused == 0) {
            }
        lcd.cls();
        }
        
        
    
    if (state == 1) {                               // State for showing task 6 
        Enable = 1;
        
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("Go!");                         
                            
            while (state == 1 && paused == 0) {       
                                  
                    while (1){  
                                            
                        RightPulses = RightEncoder.getPulses();              //     
                        LeftPulses = LeftEncoder.getPulses();
                    
                        LeftPPS = (LeftPulses - LeftPrevPulses);
                        lcd.cls();
                        lcd.locate(0,0);
                        lcd.printf("%.3f   %.3f", 1-vl, 1-vr);
                        LeftPrevPulses = LeftPulses;

                        RightPPS = (RightPulses - RightPrevPulses);
                        lcd.locate(0,20);
                        pc.printf("\t%.3f", 1-vr);
                        RightPrevPulses = RightPulses;

                    
                        pc.printf("\t%.3f", ErrL);
                        pc.printf("\t%.3f\n\r", ErrR);
                    
                        turn = 0; //500*(Pot1.read() - 0.5f);
                    
                        K = Pot2.read();
                    
                        ErrR = (100 - turn) - RightPPS;
                        ErrL = (90 + turn) - LeftPPS;
                    
                        vr = 0.5f - 0.001f*(K*ErrR);  //right output
                        vl = 0.5f - 0.001f*(K*ErrL);  //left output
                    
                    
                        if(vr < 0.1f){     //max and min output
                            vr = 0.1f;
                            }
                        if(vr > 0.8f){
                            vr = 0.8f;
                            }
                        if(vl < 0.1f){
                            vl = 0.1f;
                            }
                        if(vl > 0.8f){
                            vl = 0.8f;
                            }
                        
                            Right.write(vr);            
                            Left.write(vl);                                        
                    }
            
                                    
                        ErrR = 0;
                        ErrL = 0; 
                                
                        Enable = 0;
                        paused = 1;
                            
                        lcd.cls();
                        lcd.locate(0,0);
                        lcd.printf("First State:"); 
                        lcd.locate(0,20);
                        lcd.printf("STOP"); 
                        wait(1.5);
                            
                        state = 0;
        }
       }
            lcd.cls();                        
}
}
