#ifndef BALL_H
#define BALL_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"

/** Ball Class
@author Dr Craig A. Evans, University of Leeds
@brief Controls the ball in the Pong game 
@date Febraury 2017
*/ 
class Bullets
{

public:
    Bullets(int ex, int wy, int d);
    ~Bullets();
    void draw(N5110 &lcd);
    void update();
    int get_y();
    int get_x();
    void dead();
private:
    int _x;
    int _y;
    int _dir;
};
#endif