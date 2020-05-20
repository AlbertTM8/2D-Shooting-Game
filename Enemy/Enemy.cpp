#include "Enemy.h"
#include <Bitmap.h>
// nothing doing in the constructor and destructor
Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::init()
{
    _x = 84;
    _y = 48;
}

void Enemy::draw(N5110 &lcd)
{
    lcd.drawRect(_x,_y,3,3,FILL_TRANSPARENT);
}

void Enemy::update(int player_x, int player_y)
{
    if(_x<player_x){
        _x++;
        }
    else if(_x > player_x){
        _x--;
        } 
        
    if(_y < player_y){
        _y++;
        }
    else if (_y > player_y){
        _y--;
        }     
}
int Enemy::get_x()
{   
    return _x;
    
}
int Enemy::get_y()
{   
    return _y;
    
}
