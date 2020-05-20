#include "Bullets.h"
Serial pcb(USBTX, USBRX);
Bullets::Bullets()
{

}

Bullets::~Bullets()
{

}

void Bullets::init(int ex, int wy, int d)
{
    _size = 1;
    _speed = 4;
    _x = ex;
    _y = wy;

    if (d == 0) {
        _dir = 0;
    } else if (d == 1) {
        _dir = 1;
    } else if (d == 2) {
        _dir = 2;
    } else {
        _dir = 3;
    }
    //pcb.printf("Direction = %d", _dir);
}

void Bullets::draw(N5110 &lcd)
{   

    if(_dir == 0){
    lcd.drawLine(_x,_y, _x, 0,1);
    }
    else if (_dir == 1){
    lcd.drawLine(_x,_y, WIDTH, _y,1);
    }
    else if (_dir == 2){
    lcd.drawLine(_x,_y, _x, HEIGHT,1);
    }
    else if (_dir == 3){
    lcd.drawLine(_x,_y, 0, _y,1);
    }
}

void Bullets::update(int ex, int wy)
{
    _x = ex;
    _y = wy;
}