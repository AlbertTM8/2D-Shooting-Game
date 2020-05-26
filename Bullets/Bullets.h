/** Bullets Class
@brief  - Defines instances of bullets for a 2D shooter game

@author Albert Tan Muligan, Student at University of Leeds
@date May 2020


*/ 

#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
class Bullets
{

public:
    /** 
  * @brief Constructor that sets inital x and y
  * @param x and y  @details x and y in pixels (max x = 84, y = 48)
  */
    Bullets(int ex, int wy, int d);
    ~Bullets();
    void draw(N5110 &lcd);
    void update();
    ///Y value accessor
    int get_y();
    ///X value accessor
    int get_x();
    ///"Dead" function puts the x and y at 1000 so it's off-screen.
    void dead();
private:
    int _x;
    int _y;
    int _dir;
};