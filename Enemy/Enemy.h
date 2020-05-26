#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"

/** Enemy Class:
@brief  - Defines instances of Enemies for a 2D shooter game

@author Albert Tan Muligan, Student at University of Leeds
@date May 2020


*/ 

class Enemy
{
public:
    /** 
  * @brief Constructor that sets inital x and y with random seet
  * @param x and y  @details x and y in pixels (places enemy somewhere on the edge of a 84x48 board)
  */
    Enemy(int seed);
    ~Enemy();
    void draw(N5110 &lcd);
    void update(int player_x, int player_y);
    ///X value accessor
    int get_x();
    ///Y value accessor
    int get_y();
     /** 
  * @brief Reset enemy to edge with random seed, and creates an animation on death
  * @param seed, lcd  @details integer seed and pointer to lcd
  */
    void reset(int seed, N5110 &lcd);
private:
    ///small animation on death
    void death_animation(N5110 &lcd);
    int _x;
    int _y;
    int _speed;
    int _four;
};