#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"


class Enemy
{
public:
    Enemy(int seed);
    ~Enemy();
    void draw(N5110 &lcd);
    void update(int player_x, int player_y);
    int get_x();
    int get_y();
    void reset(int seed, N5110 &lcd);
private:
    void DeathAnimation(N5110 &lcd);
    int _x;
    int _y;
    int _speed;
    int _four;
};