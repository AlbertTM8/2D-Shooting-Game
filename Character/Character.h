#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"
/** Character Class
@author Albert Tan Muligan, Student at University of Leeds
@brief Controls the Character in a shooter game 
@date May 2020


*/ 
class Character
{
public:
    Character();
    ~Character();
    /** 
  * @brief Sets inital x and y
  * @param x and y  @details x and y in pixels (max x = 84, y = 48)
  */
    void init(int x,int y);
    void draw(N5110 &lcd);
    void update(Direction d);
    /** 
  * @brief Resets x, y and direction to original(center, facing up) values
  */
    void reset();
    ///Direction accessor
    int get_direction();
    ///X value accessor
    int get_x();
    ///X value accessor
    int get_y();

private:

    int _x;
    int _y;
    int _speed;
    int _dir;
};