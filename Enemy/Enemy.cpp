#include "Enemy.h"
#include <Bitmap.h>
Serial pce(USBTX, USBRX);
//Constructor with seed, to spawn enemies at random places on edge of screen
Enemy::Enemy(int seed)
{
    srand(seed);
    _four = (rand()%4)+1;
    //pce.printf("%d",_four);
    if(_four == 1){
        _x = 0;
        _y = rand()%48;
        }
    else if(_four == 2){
        _x = 84;
        _y = rand()%48;
        }
    else if(_four == 3){
        _y = 0;
        _x = rand()%84;
        }
    else if(_four == 4){
        _y = 48;
        _x = rand()%84;
        }
}

Enemy::~Enemy()
{

}
void Enemy::draw(N5110 &lcd)
{
    lcd.drawRect(_x,_y,3,3,FILL_TRANSPARENT);
}

//updates enemy with player x and y so it can walk towards the player
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
//accessor functions
int Enemy::get_x()
{   
    return _x;
    
}
int Enemy::get_y()
{   
    return _y;
    
}
//resets enemies each time it gets shot
void Enemy::reset(int seed, N5110 &lcd){
    DeathAnimation(lcd);
    srand(seed);
    _four = (rand()%4)+1;
    //pce.printf("%d", _four);
    if(_four == 1){
        _x = 0;
        _y = rand()%48;
        }
    else if(_four == 2){
        _x = 84;
        _y = rand()%48;
        }
    else if(_four == 3){
        _y = 0;
        _x = rand()%84;
        }
    else if(_four == 4){
        _y = 48;
        _x = rand()%84;
        } 
}
//Eenmy Death Animation
void Enemy::DeathAnimation(N5110 &lcd){
    int sprite_data[] = {
    1,0,1,0,1,
    0,1,1,1,0,
    1,1,0,1,1,
    0,1,1,1,0,
    1,0,1,0,1
    };
    
    
    Bitmap death(sprite_data, 5, 5);

    //Render Bitmap at x & y
    death.render(lcd, _x - 1, _y - 1);
    lcd.refresh();
    wait(0.01);
    }
