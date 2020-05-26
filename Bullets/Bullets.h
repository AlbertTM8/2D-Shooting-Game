/** Bullets Class
@author Albert Tan Muligan, Student at University of Leeds
@brief Controls the Character in a shooter game 
@date May 2020


*/ 

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