/* 
ELEC2645 Embedded Systems Project
School of Electronic & Electrical Engineering
University of Leeds
2019/20

Name: Albert Tan Mulligan
Username: el18ajst
Student ID Number:201241153
Date:10/4/2020
*/

// includes
#include "mbed.h"
#include "Gamepad.h"
#include "N5110.h"
#include "Character.h"
#include "Bullets.h"
Serial pc(USBTX, USBRX);
//STRUCTS
struct State {
int number;
int next_state[2];
};

//OBJECTS
Gamepad pad;
N5110 lcd;
Character p1;
Bullets shot;

//Variables
State fsm[3] = {
        {0,{1,1}},
        {1,{2,2}},
        {2,{1,0}}
        };
volatile int Current_State = 0;
//PROTOTYPES
void menu();
void GameRun();
void Pause();
void LevelUp();
//FUNCTIONS
int main()
    {    
    //pc.printf("Main Open");
     //SETUPS
    pad.init();
    lcd.init();
    lcd.clear();
    //testing CoolTerm
    //pc.printf("CoolTerm is Connected\n");
    //pc.printf("CurrentState = %d\n", Current_State);
    
    while(1){
    if(Current_State == 0){
        menu();
        }
    else if (Current_State == 1){
        GameRun();
        }
    else if (Current_State ==2){
        Pause();
        }
}
}

void menu(){    
    //pc.printf("Menu Open");
    lcd.printString("    BoxHead    ",0,1);  
    lcd.printString("  Choose your",0,3);
    lcd.printString("     Weapon",0,4);
    lcd.refresh();
        while (1) {
        //pc.printf("While Loop + %d", g_pad.Button_A_flag);
            if (pad.A_pressed()) {
                //pc.printf("Button_A");
                Current_State = fsm[0].next_state[0];
                p1.init(40,22);     
                return;
            }
            if (pad.X_pressed()) {
                //pc.printf("Button_X");
                Current_State = fsm[0].next_state[0];
                p1.init(40,22); 
                return;
            }
            if (pad.Y_pressed()) {
                //pc.printf("Button_Y");
                Current_State = fsm[0].next_state[0];
                p1.init(40,22); 
                return;
            }
        sleep();
        }
}
                                                                         
void GameRun(){
    //pc.printf("Game Open");
    Direction dir = pad.get_direction();
    float mag = pad.get_mag();
    while(1){
        //movement code
        dir = pad.get_direction();
        mag = pad.get_mag();
        
        if (pad.A_pressed()) {
                shot.init(p1.get_x()+2, p1.get_y()+2, 1);
        }
        else if (pad.B_pressed()) {
                shot.init(p1.get_x()+2, p1.get_y()+2, 2);
        }        
        else if (pad.Y_pressed()) {
                shot.init(p1.get_x()+2, p1.get_y()+2, 3);
        }
        else if (pad.X_pressed()) {
                shot.init(p1.get_x()+2, p1.get_y()+2, 0);
        }
        p1.update(dir, mag);
        shot.update(p1.get_x()+2, p1.get_y()+2);
        lcd.clear(); 
        p1.draw(lcd);
        shot.draw(lcd);
        lcd.refresh();
        wait(1.0f/10.0f);
        if (pad.start_pressed()) {
            Current_State = fsm[Current_State].next_state[0];
            return;

        }
    }
}
void Pause(){
        lcd.printString("    PAUSED     ",0,1);  
        lcd.refresh();  
                // put the MCU to sleep until an interrupt wakes it up
        if (pad.start_pressed()) {
            Current_State = fsm[Current_State].next_state[0];
            return;

        }
        if (pad.B_pressed()) {
            Current_State = fsm[Current_State].next_state[1];
            lcd.clear();
            return;

        }
        sleep();
    }
void LevelUp(){
    
    lcd.printString("     NEXT LEVEL    ",0,1);  
    wait(2.0);
    lcd.refresh();
    p1.level_up();
    Current_State = fsm[Current_State].next_state[0];
    return;
    }
