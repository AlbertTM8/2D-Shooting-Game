#include "Bullets.h"
Serial pcb(USBTX, USBRX);
//constructor defines x, y and the direction of travel
Bullets::Bullets(int ex, int wy, int d)
{
    //direction defaults to zero if the joystick hasn't been used yet
    _dir = 0;
    _x = ex;
    _y = wy;
    _dir = d;
    //pcb.printf("Direction = %d", _dir);
}

Bullets::~Bullets()
{

}
//draws a 1-by-1 rectangle at x and y
void Bullets::draw(N5110 &lcd)
{   

    lcd.drawRect(_x,_y,1,1,FILL_BLACK);
}
//bullets travels in a straight line in a direction
void Bullets::update()
{
    if(_dir == 0){
    _y = _y-3;
    }
    else if (_dir == 1){
    _x = _x+3;
    }
    else if (_dir == 2){
    _y = _y+3;
    }
    else if (_dir == 3){
    _x = _x-3;
    }
}
//accessors
int Bullets::get_x(){
    return _x;
    }
    
int Bullets::get_y(){
    return _y;
    }
//shots that "die" go offscreen (where main them deletes them in line 224)
//Avoids vector becoming zero, which causes errors in the GameRun loop
void Bullets::dead(){
       _x = 1000;
       _y = 1000;
}
    