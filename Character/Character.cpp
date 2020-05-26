#include "Character.h"
#include <Bitmap.h>
Serial pcc(USBTX, USBRX);
// Contrcutor and Deconstructor are empty
Character::Character()
{

}

Character::~Character()
{

}
//Initlializer that sets x and y coordinates
void Character::init(int x,int y)
{
    _x = x;  // initial x
    _y = y;  // initial y
    _speed = 1;  // default speed
}
//draws and renders sprite, accounting for direction
void Character::draw(N5110 &lcd)
{ //sprite is facing left
  static int sprite_data[] = {
  0,1,1,1,1,
  1,1,1,1,1,
  1,1,1,1,1,
  1,1,1,1,1,
  0,1,1,1,1
  };
  //turning the sprite 
    for(int i = 0; i < 25; i++){
            sprite_data[i] = 1;
            }
     if (_dir == 0) {
        sprite_data[0] = 0;
        sprite_data[4] = 0;
    } else if (_dir == 1) {
        sprite_data[4] = 0;
        sprite_data[24] = 0;
    } else if (_dir == 2) {
        sprite_data[20] = 0;
        sprite_data[24] = 0;
    } else if (_dir == 3) {
        sprite_data[0] = 0;
        sprite_data[20] = 0;
    }
 //Intantiate Bitmap
 Bitmap sprite(sprite_data, 5, 5);
 
 //Render Bitmap at x & y
 sprite.render(lcd, _x, _y);
}
//Update direction and movement of Character
void Character::update(Direction d)
{
    _speed = 1; //scale of speed
    //pcc.printf("speed = %d \n", _speed);
    // update x and y value depending on direction of movement
    // Set direction and speed according to north south directions
    if (d == N) {
        _y-=_speed;
        _dir = 0;
    } else if (d == S) {
        _y+=_speed;
        _dir = 2;
    } else if (d == E) {
        _x+=_speed;
        _dir = 1;
    } else if (d == W) {
        _x-=_speed;
        _dir = 3;
    }
    //testing _x and _y
    //pcc.printf("x = %d \n", _x);
    //pcc.printf("y = %d \n", _y);
    if (_x < 1) { _x = 1; }// check the x and y position] to ensure that the paddle doesn't go off screen
    if (_y < 1) { _y = 1; }
    if (_x > 79){ _x = 79; }
    if (_y > 43){ _y = 43; }
    }
//Accessor Functions
int Character::get_x()
{   
    return _x;
    
}
int Character::get_y()
{   
    return _y;
    
}
int Character::get_direction(){
    return _dir;
    }
//Resets Character to original position and direction
void Character::reset(){
    _x = 40;
    _y = 22;
    _dir = 0;
    }
