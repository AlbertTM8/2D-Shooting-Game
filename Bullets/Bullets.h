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
    Bullets();
    ~Bullets();
    void init(int ex, int wy, int d);
    void draw(N5110 &lcd);
    void update(int ex, int wy);
    /// accessors and mutators
    Vector2D get_pos();
    void set_pos(Vector2D p);
    
private:

    int _x;
    int _y;
    int _size;
    int _dir;
    int _speed;
};
#endif