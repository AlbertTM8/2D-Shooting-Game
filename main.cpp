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
#include <vector>
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

//FUNCTIONS
int main() {//pc.printf("Main Open");
     //SETUPS
    pad.init();
    lcd.init();
    lcd.setContrast(0.4 + (pad.read_pot1() / 2.0));
    lcd.clear();
    //pc.printf("CoolTerm is Connected\n");
    while(1){
    //pc.printf("CurrentState = %d\n", Current_State);
    if(Current_State == 0){
        menu();
        }
    else if (Current_State == 1){
        GameRun();
        }
    else if (Current_State == 2){
        Pause();
        }
    else if (Current_State == 3){
        PlayerDead();
        reset();
        }
}
}

void menu(){    
    //pc.printf("Menu Open");
    lcd.clear();
    lcd.printString("BoxHead",21,1);  
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
            p1.init(40,22);
            return;
            }
        sleep();
        }
}
//Runs actual game code                                                                
void GameRun(){
    //pc.printf("Game Open");
    Direction dir = pad.get_direction();
    pad.play_melody(46, melody,durations,180.0,true);
    //main game run loop
    while(1){
        //Timer to seed random functions/spawn enemies intermittently
        timer++;
        if(timer%100==0){ AddEnemy(); }
        dir = pad.get_direction();
        //setting a placeholder for last direction, because bullets should travel in facing direction if the joystick is in neutral position
        if(dir == N){ holder = 0; }
        else if(dir == S){ holder = 2; }
        else if(dir == E){ holder = 1; }
        else if(dir == W){ holder = 3; }   
        //pc.printf("Direction holder = %d\n", holder);
        //shoot if A is pressed
        if (pad.A_pressed()) {
            shots.push_back(Bullets(p1.get_x()+2, p1.get_y()+2, holder)); }
        //lcd updates, collsions and draws 
        updates(dir);
        draws();
        collisions();
        //Brings up pause menu if you press start
        if (pad.start_held()) {
            Current_State = fsm[Current_State].next_state[0];
            return; }
        //Collisions could change current state to 3 (Dead state)
        if(Current_State!= 1){ return; }
        //setting frames
        wait(1.0f/10.0f);
    }
}

void Pause(){
        lcd.printString("    PAUSED",0,1);  
        lcd.refresh();  
        lcd.printString("  Press B to",0,4); 
        lcd.printString("Unpause",21,5); 
        //B goes back to main game run code
        if (pad.B_held()) {
            Current_State = fsm[Current_State].next_state[0];
            lcd.clear();
            return;
        }
        //sleep function for interrupt
        sleep();
    }
//Add enemies
void AddEnemy(){
    //adds an enemy to the vector of enemy class
    enemies.push_back(Enemy(timer));
    return;
    }
//Called When Player Dies, B returns the game to menu
void PlayerDead(){
    lcd.clear();
    lcd.printString("YOU DIED",18,1);  
    char buffer[14];
    //score displays differently if below 10 to make sure the text is centered
    if(score>10){
    sprintf(buffer, "   Score:%d", score);
    lcd.printString(buffer,0,2); 
    }else{
    sprintf(buffer, "Score:0%d", score);
    lcd.printString(buffer,18,2); 
        }
    lcd.printString("  Press B to  ",0,4); 
    lcd.printString("Restart",21,5); 
    lcd.refresh(); 
    while(1){
        if(pad.B_held()){
        animation();
        Current_State =fsm[Current_State].next_state[1];
        return;
        }
        sleep();
        }
    }
//Small animation for transition between main menus
void animation(){
    
    for(int i = 1; i<=42; i++){
    lcd.drawRect(0, 0, i, 48, FILL_BLACK);
    lcd.drawRect(84-i, 0, i, 48, FILL_BLACK);
    lcd.refresh();
    wait(0.005);
    }
    
    for(int i = 1; i<=42; i++){
        lcd.drawRect(0, 0, i, 48, FILL_WHITE);
        lcd.drawRect(84-i, 0, i, 48, FILL_WHITE);
        lcd.refresh();
        wait(0.005);
    }
    
    return;
    }
//Resets the game variables when you die
void reset(){
    score = 0;
    while(enemies.size() >= 1){
        enemies.pop_back();
    }
    
    while(shots.size() >= 1){
        shots.pop_back();
    }
    
    p1.reset();
    holder = 0;
    return;
    }
//all the collisions and 
void collisions(){
    
    //Checking all shots to make sure they are erased if they leave the screen 
    for(int i = 1; i < shots.size(); i++){
            if(shots.at(i).get_x()>84 | shots.at(i).get_x()<1 | shots.at(i).get_y()>48 | shots.at(i).get_y()<1){
                shots.erase(shots.begin()+i);
                }
            }
            
    //Checking all enemy-player collisions and enemy-bullet collisions
    for(int i = 0; i < enemies.size(); i++){
        
        //update enemies to save processing time of having another large for loop in updates function
        enemies.at(i).update(p1.get_x()+1, p1.get_y()+1);
        
        //enemy player collisions
        if(enemies.at(i).get_x()>= p1.get_x()-1 & enemies.at(i).get_x()<= p1.get_x()+3 & enemies.at(i).get_y()>= p1.get_y()-1 & enemies.at(i).get_y()<= p1.get_y()+3){
            Current_State = fsm[Current_State].next_state[1];
            return;
            }
            
        //enemy bullet collisions
        for(int j = 0; j < shots.size(); j++){
            if(shots.at(j).get_x() >= enemies.at(i).get_x() & shots.at(j).get_x()<= enemies.at(i).get_x()+3 & shots.at(j).get_y() >= enemies.at(i).get_y() & shots.at(j).get_y()<= enemies.at(i).get_y()+3){
                shots.at(j).dead();
                enemies.at(i).reset(timer, lcd);
                score++;
                }   
            }
        }
    }
//All the draw functions w/ clear and refresh
void draws(){
        lcd.clear(); 
        for(int i = 0; i<enemies.size(); i++){
            enemies.at(i).draw(lcd);
            }
        for(int i = 0; i<shots.size(); i++){
            shots.at(i).draw(lcd);
            }
        p1.draw(lcd);
        lcd.refresh();
    }
//Updates objects after every frame(enemy update is in collisions)
void updates(Direction dir){
    p1.update(dir);
    for(int i = 0; i<shots.size(); i++){
        shots.at(i).update();
    }
}