#include "mbed.h"
#include "N5110.h"
#include "Gamepad.h"


class Character
{
public:
    Character();
    ~Character();
    void init(int x,int y);
    void draw(N5110 &lcd);
    void update(Direction d,float mag);
    void level_up();
    int get_level();
    int get_direction();
    int get_x();
    int get_y();

private:

    int _x;
    int _y;
    int _speed;
    int _level;
    int _dir;
};