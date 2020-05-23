#include "Enemy.h"
#include <Bitmap.h>
Serial pce(USBTX, USBRX);
// nothing doing in the constructor and destructor
Enemy::Enemy(int seed)
{
    _dead = false;
    srand(seed);
    four = (rand()%4)+1;
    //pce.printf("%d",four);
    if(four == 1){
        _x = 0;
        _y = rand()%48;
        }
    else if(four == 2){
        _x = 84;
        _y = rand()%48;
        }
    else if(four == 3){
        _y = 0;
        _x = rand()%84;
        }
    else if(four == 4){
        _y = 48;
        _x = rand()%84;
        }
}

Enemy::~Enemy()
{

}

void Enemy::init()
{
}

void Enemy::draw(N5110 &lcd)
{
    lcd.drawRect(_x,_y,3,3,FILL_TRANSPARENT);
}

void Enemy::update(int player_x, int player_y)
{
    if(_dead == false){
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
}
int Enemy::get_x()
{   
    return _x;
    
}
int Enemy::get_y()
{   
    return _y;
    
}
void Enemy::dead(){
       _dead  = true;
       _x = 2147483647;
}
void Enemy::reset(int seed){
    _dead = false;
    srand(seed);
    four = (rand()%4)+1;
    //pce.printf("%d",four);
    if(four == 1){
        _x = 0;
        _y = rand()%48;
        }
    else if(four == 2){
        _x = 84;
        _y = rand()%48;
        }
    else if(four == 3){
        _y = 0;
        _x = rand()%84;
        }
    else if(four == 4){
        _y = 48;
        _x = rand()%84;
        } 
}
