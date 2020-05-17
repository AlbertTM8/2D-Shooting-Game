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
Serial pc(USBTX, USBRX);
// objects
Gamepad pad;
N5110 lcd;
Character p1;
int main()
{// need to initialise LCD and Gamepad 
    lcd.init();
    pad.init();
    
    //Joystick intitilisation
    Direction dir = pad.get_direction();
    float mag = pad.get_mag();
    
    //character intitilisation
    p1.init(40,22);
    
    //testing CoolTerm
    pc.printf("CoolTerm is Connected\n");
    
    while(1){
        ///movement code
        dir = pad.get_direction();
        mag = pad.get_mag();
        p1.update(dir, mag);
        lcd.clear();  
        p1.draw(lcd);
        lcd.refresh();
        wait(1.0f/10.0);
    }
}

