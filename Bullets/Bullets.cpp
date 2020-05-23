#include "Bullets.h"
Serial pcb(USBTX, USBRX);
Bullets::Bullets(int ex, int wy, Direction d)
{
    _dir = 0;
    _x = ex;
    _y = wy;
    if (d == N) {
        _dir = 0;
    } 
    else if (d == E) {
        _dir = 1;
    }
    else if (d == S) {
        _dir = 2;
    }
    else if (d == W) {
        _dir = 3;
    }
    //pcb.printf("Direction = %d", _dir);
}

Bullets::~Bullets()
{

}

void Bullets::draw(N5110 &lcd)
{   

    lcd.drawRect(_x,_y,1,1,FILL_BLACK);
}

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
int Bullets::get_x(){
    return _x;
    }
    
int Bullets::get_y(){
    return _y;
    }
    
void Bullets::dead(){
       _x = 2147483647;
       _y = 1000;
}
    