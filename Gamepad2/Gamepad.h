#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <bitset>

// Forward declaration of the classes that we use from the mbed library
// This avoids the need for us to include the huge mbed.h header inside our
// own library API
namespace mbed
{
class AnalogIn;
class InterruptIn;
class PwmOut;
class AnalogOut;
class Ticker;
class Timeout;
}

#define TOL 0.1f
#define RAD2DEG 57.2957795131f
#define PI 3.14159265359


/** Enum for direction */
enum Direction {
    CENTRE,  /**< joystick centred */
    N,       /**< pushed North (0)*/
    NE,      /**< pushed North-East (45) */
    E,       /**< pushed East (90) */
    SE,      /**< pushed South-East (135) */
    S,       /**< pushed South (180) */
    SW,      /**< pushed South-West (225) */
    W,       /**< pushed West (270) */
    NW       /**< pushed North-West (315) */
};

/** Vector 2D struct */
struct Vector2D {
    float x; /**< float for x value */
    float y; /**< float for y value */
};

/** Polar coordinate struct */
struct Polar {
    float mag;  /**< float for magnitude */
    float angle; /**< float for angle (in degrees) */
};

/** Gamepad Class
 * @brief Library for interfacing with ELEC2645 Gamepad PCB, University of Leeds
 * @author Dr Craig A. Evans
 * @author Dr Alex Valavanis
 * @author Joshua Davy
 */
class Gamepad
{

private:
    mbed::PwmOut *_led1;
    mbed::PwmOut *_led2;
    mbed::PwmOut *_led3;
    mbed::PwmOut *_led4;
    mbed::PwmOut *_led5;
    mbed::PwmOut *_led6;

    mbed::InterruptIn *_button_A;
    mbed::InterruptIn *_button_B;
    mbed::InterruptIn *_button_X;
    mbed::InterruptIn *_button_Y;
    mbed::InterruptIn *_button_start;

    mbed::AnalogIn *_vert;
    mbed::AnalogIn *_horiz;

    mbed::AnalogIn *_pot1;
    mbed::AnalogIn *_pot2;
    
    mbed::AnalogOut *dac;
    mbed::Ticker *ticker;
    mbed::Timeout *timeout;
    mbed::Timeout *note_timeout;

    // centred x,y values
    float _x0;
    float _y0;
    
    float *_sample_array;
    const int *_notes;
    const int *_durations;
    
    int _n;
    int _melody_length;
    volatile unsigned int _sample;
    volatile unsigned int _note;
    float _bpm;
    bool _repeat;
    

public:
    /** Constructor */
    Gamepad();

    /** Initialise all peripherals and configure interrupts */
    void init();

    /** Turn all LEDs on */
    void leds_on();

    /** Turn all LEDs off */
    void leds_off();

    /** Set all LEDs to duty-cycle
    *@param value in range 0.0 to 1.0
    */
    void leds(float val) const;

    /** Set LED to duty-cycle
    *@param led number (0 to 5)
    *@param value in range 0.0 to 1.0
    */
    void led(int n,float val) const;

    /** Read potentiometer 1 value
    *@returns potentiometer value in range 0.0 to 1.0
    */
    float read_pot1() const;

    /** Read potentiometer 2 value
    *@returns potentiometer value in range 0.0 to 1.0
    */
    float read_pot2() const;

    /** Get magnitude of joystick movement
    * @returns value in range 0.0 to 1.0
    */
    float get_mag();

    /** Get angle of joystick movement
    * @returns value in range 0.0 to 359.9. 0.0 corresponds to N, 180.0 to S. -1.0 is central
    */
    float get_angle();

    /** Gets joystick direction
    * @returns an enum: CENTRE, N, NE, E, SE, S, SW, W, NW,
    */
    Direction get_direction();    // N,NE,E,SE etc.

    /** Gets raw cartesian co-ordinates of joystick
    * @returns a struct with x,y members, each in the range 0.0 to 1.0
    */
    Vector2D get_coord();         // cartesian co-ordinates x,y

    /** Gets cartesian coordinates mapped to circular grid
    * @returns a struct with x,y members, each in the range 0.0 to 1.0
    */
    Vector2D get_mapped_coord();  // x,y mapped to circle

    /** Gets polar coordinates of the joystick
    * @returns a struct contains mag and angle
    */
    Polar get_polar();            // mag and angle in struct form


    /** Resets all button states. Useful for calling inbetween scenes
    *   where you do not want button presses from the previous scene effecting
    *   the current scene
    */
    void reset_buttons();

    /** Returns true if A has been pressed
    * @returns a bool corresponding to A being pressed
    */

    bool A_pressed();

    /** Returns true if A is held
    * @returns a bool corresponding to A being held
    *
    */
    bool A_held();
    

    /** Returns true if B has been pressed
    * @returns a bool corresponding to B being pressed
    */
    bool B_pressed();

    /** Returns true if B is held
    * @returns a bool corresponding to B being held
    *
    */
    bool B_held();

    /** Returns true if B has been pressed
    * @returns a bool corresponding to B being pressed
    */
    bool X_pressed();

    /** Returns true if X is held
    * @returns a bool corresponding to X being held
    *
    */
    bool X_held();    

    /** Returns true if Y has been pressed
    * @returns a bool corresponding to Y being pressed
    */
    bool Y_pressed();

    /** Returns true if Y is held
    * @returns a bool corresponding to Y being held
    *
    */
    bool Y_held();


    /** Returns true if start has been pressed
    * @returns a bool corresponding to start being pressed
    */
    bool start_pressed();

    /** Returns true if start is held
    * @returns a bool corresponding to start being held
    *
    */
    bool start_held();
    
    /** Play a single tone for the specifed duration
    *@param note frequency (in Hz)
    *@param duration (in s)
    */
    void tone(const float frequency,const float duration);
    
    /** Play a melody
    *@param length of note array
    *@param array of note frequencies (in Hz) - 0 treated as a rest
    *@param array of note durations (4 corresponds to 1/4, 8 is 1/8 etc.)
    *@param beats per minute
    *@param whether to repeat or play just once
    */
    void play_melody(int length,const int *notes,const int *durations,float bpm,bool repeat);
    
    /** Set the BPM of the melody
    *@param beats per minute
    */
    void set_bpm(float bpm);
    
    /** Write an analog voltage to the speaker
    *@param voltage in range 0.0 to 1.0 (corresponds 0.0 to 3.3 V)
    */
    void write_dac(float val);




private:

    volatile bool A_fall;
    void A_fall_interrupt();

    volatile bool B_fall;
    void B_fall_interrupt();
    
    volatile bool X_fall;
    void X_fall_interrupt();

    volatile bool Y_fall;
    void Y_fall_interrupt();

    volatile bool start_fall;
    void start_fall_interrupt();

   // Tone functions  
    void ticker_isr();    
    void timeout_isr();
    void note_timeout_isr();  
    void play_next_note();



};

// Note definitions from Arduino.cc
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#endif