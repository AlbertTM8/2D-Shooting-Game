#ifndef BALL_H
#define BALL_H

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"

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