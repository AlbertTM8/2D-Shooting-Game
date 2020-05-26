/*
ELEC2645 Embedded Systems Project
School of Electronic & Electrical Engineering
University of Leeds
2019/20

Name: Albert Tan Mulligan
Username: el18ajst
Student ID Number:201241153
Date:10/4/2020
*/

// includes
#include "mbed.h"
#include "Gamepad.h"
#include "N5110.h"
#include "Character.h"
#include "Bullets.h"
#include "Enemy.h"
#include "Bitmaps.h"
#include <vector>
#include <Bitmap.h>

//my computer had issues with CoolTerm but setting up serial communication fixed it
Serial pc(USBTX, USBRX);
//STRUCTS
struct State {
    int number;
    int next_state[2];
};

//OBJECTS
Gamepad pad;
N5110 lcd;
Character p1;
vector<Bullets> shots;
vector<Enemy> enemies;

//Variables
int melody[46] = {277, 330, 311, 277, 330, 311, 330, 311, 370, 330, 415, 330, 277, 330, 311, 277, 330, 311, 330, 311, 370, 330, 311, 247, 277, 330, 311, 277, 330, 311, 330, 311, 370, 330, 415, 440, 330, 415, 440, 494, 554, 415, 440, 494, 622, 659}; // #c = 277, d# =311, f# = 370, g# = 415
int durations[46] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
int dead_music[6] = {0, 0, 220, 220, 220, 175};
int dead_duration[6] = {8,8,8,8,8,1};
State fsm[4] = {
    {0,{1,1}},
    {1,{2,3}},
    {2,{1,1}},
    {3,{0,0}}
};
volatile int Current_State = 0;
volatile int timer = 0;
volatile int score = 0;
volatile int holder;

//PROTOTYPES
void menu();
void GameRun();
void Pause();
void AddEnemy();
void PlayerDead();
void animation();
void reset();
void collisions();
void draws();
void updates(Direction dir);
void DeathAnimation();

//FUNCTIONS
int main()      //pc.printf("Main Open");
{
    pad.init();      //initial setups
    lcd.init();
    lcd.clear();
    //pc.printf("CoolTerm is Connected\n");
    lcd.setContrast(0.3 + (pad.read_pot1() / 5.0));
    while(1) {
        //pc.printf("CurrentState = %d\n", Current_State);
        if(Current_State == 0) {
            menu();
        } else if (Current_State == 1) {
            GameRun();
        } else if (Current_State == 2) {
            Pause();
        } else if (Current_State == 3) {
            PlayerDead();
            reset();
        }
    }
}

void menu()
{
    //pc.printf("Menu Open");
    lcd.clear();
    Bitmap sprite(Title, 17, 60);
        sprite.render(lcd, 10, 6);
    lcd.printString("  Press A to  ",0,3);
    lcd.printString("Start",27,4);
    lcd.refresh();
    while (1) {
        //pc.printf("While Loop + %d", g_pad.Button_A_flag);
        if (pad.A_held()) {
            //pc.printf("Button_A");
            animation();
            Current_State = fsm[0].next_state[0];
            enemies.push_back(Enemy(timer));
            timer = 0;
            p1.init(40,22);
            return;
        }
        sleep();
    }
}
//Runs actual game code
void GameRun()
{
    //pc.printf("Game Open/n");
    Direction dir = pad.get_direction();
    pad.play_melody(46, melody,durations,180.0,true);
    while(1) {  //main game run loop
        //pc.printf("Direction = %d/n", dir);
        timer++;    //Timer to seed random functions/spawn enemies intermittently
        if(timer % 100 == 0) {
            AddEnemy();
        }
        dir = pad.get_direction();

        if(dir == N) {
            holder = 0;    //setting a placeholder for last direction, because bullets should travel in facing direction if the joystick is in neutral position
        } else if(dir == S) {
            holder = 2;
        } else if(dir == E) {
            holder = 1;
        } else if(dir == W) {
            holder = 3;
        }
        //pc.printf("Direction holder = %d\n", holder);

        if (pad.A_pressed()) {        //shoot if A is pressed
            shots.push_back( Bullets( p1.get_x() + 2, p1.get_y() + 2, holder));
        }

        //lcd updates, collsions and draws
        updates(dir);
        draws();
        collisions();

        if (pad.start_held()) {
            //pc.printf("Start Held/n");
            Current_State = fsm[Current_State].next_state[0];   //Brings up pause menu if you press start
            return;
        }

        if(Current_State != 1) {
            return;    //Collisions could change current state to 3 (Dead state)
        }
        //setting frames
        wait(1.0f/10.0f);
    }
}

void Pause()    //Creates the Pause menu
{
    //pc.printf("Pause Menu/n");
    lcd.printString("    PAUSED",0,1);
    lcd.refresh();
    lcd.printString("  Press B to",0,4);
    lcd.printString("Unpause",21,5);
    if (pad.B_held()) {             //B goes back to main game run code
        //pc.printf("B Held/n");
        Current_State = fsm[Current_State].next_state[0];
        lcd.clear();
        return;
    }
    sleep();    //sleep function for interrupt
}

void AddEnemy()     //Add enemies
{
    //pc.printf("Add Enemy/n");
    enemies.push_back(Enemy(timer));    //adds an enemy to the vector of enemy class
    return;
}

void PlayerDead() //Called When Player Dies, B returns the game to menu
{   
    //pc.printf("Player Dead/n");
    pad.play_melody(6, dead_music, dead_duration,90.0,false);
    DeathAnimation();
    lcd.inverseMode();
    lcd.clear();
    lcd.printString("YOU DIED",18,1);
    char buffer[14];
    if(score > 10) {  //score displays differently if below 10 to make sure the text is centered
        sprintf(buffer, "   Score:%d", score);
        lcd.printString(buffer,0,2);
    } else {
        sprintf(buffer, "Score:0%d", score);
        lcd.printString(buffer,18,2);
    }
    lcd.printString("  Press B to  ",0,4);
    lcd.printString("Restart",21,5);
    lcd.refresh();
    while(1) {
        if(pad.B_held()) {
            Current_State =fsm[Current_State].next_state[1];
            lcd.normalMode();
            return;
        }
        sleep();
    }
}

void animation()    //Small animation for transition between main menus
{

    for(int i = 1; i <= 42; i++) {
        lcd.drawRect(0, 0, i, 48, FILL_BLACK);
        lcd.drawRect(84-i, 0, i, 48, FILL_BLACK);
        lcd.refresh();
        wait(0.005);
    }

    for(int i = 1; i <= 42; i++) {
        lcd.drawRect(0, 0, i, 48, FILL_WHITE);
        lcd.drawRect(84-i, 0, i, 48, FILL_WHITE);
        lcd.refresh();
        wait(0.005);
    }

    return;
}

void reset() //Resets the game variables when you die
{
    //pc.printf("Game Reset/n");
    score = 0;
    while(enemies.size() >= 1) {
        enemies.pop_back();
    }

    while(shots.size() >= 1) {
        shots.pop_back();
    }

    p1.reset();
    holder = 0;
    return;
}

void collisions() //all the collisions
{
    //pc.printf("Collisions/n");
    for(int i = 1; i < shots.size(); i++) { //Checking all shots to make sure they are erased if they leave the screen
        if(shots.at(i).get_x()>84 | shots.at(i).get_x()<1 | shots.at(i).get_y()>48 | shots.at(i).get_y()<1) {
            shots.erase(shots.begin()+i);
        }
    }

    for(int i = 0; i < enemies.size(); i++) {   //Checking all enemy-player collisions and enemy-bullet collisions
        enemies.at(i).update(p1.get_x()+1, p1.get_y()+1);   //update enemies to save processing time of having another large for loop in updates function

        //enemy player collisions
        if(enemies.at(i).get_x() >= p1.get_x()-1 & enemies.at(i).get_x() <= p1.get_x()+3 & enemies.at(i).get_y() >= p1.get_y()-1 & enemies.at(i).get_y() <= p1.get_y()+3) {
            Current_State = fsm[Current_State].next_state[1];
            return;
        }

        for(int j = 0; j < shots.size(); j++) { //enemy bullet collisions
            if(shots.at(j).get_x() >= enemies.at(i).get_x() & shots.at(j).get_x() <= enemies.at(i).get_x()+3 & shots.at(j).get_y() >= enemies.at(i).get_y() & shots.at(j).get_y() <= enemies.at(i).get_y()+3) {
                shots.at(j).dead();
                enemies.at(i).reset(timer, lcd);
                score++;
            }
        }
    }
}

void draws()    //All the draw functions w/ clear and refresh
{
    //pc.printf("Draw Functions/n");
    lcd.clear();
    for(int i = 0; i < enemies.size(); i++) {
        enemies.at(i).draw(lcd);
    }
    for(int i = 0; i < shots.size(); i++) {
        shots.at(i).draw(lcd);
    }
    p1.draw(lcd);
    lcd.refresh();
}

void updates(Direction dir)     //Updates objects after every frame(enemy update is in collisions)
{
    //pc.printf("Update Functions/n");
    p1.update(dir);
    for(int i = 0; i < shots.size(); i++) {
        shots.at(i).update();
    }
}

void DeathAnimation()      //falling blood animation on death
{
    for(int i = 1; i <= 65; i++) {
        Bitmap sprite(falling_blood, 17, 84);
        sprite.render(lcd, 0, i-17);
        lcd.drawRect(84, i, 0, 0, FILL_BLACK);
        lcd.refresh();
        wait(0.03);
    }
}