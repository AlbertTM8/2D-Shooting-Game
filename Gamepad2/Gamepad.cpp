#include "Gamepad.h"

#include "mbed.h"

//////////// constructor/destructor ////////////
Gamepad::Gamepad()
    :
    _led1(new PwmOut(PTA2)),
    _led2(new PwmOut(PTC2)),
    _led3(new PwmOut(PTC3)),
    _led4(new PwmOut(PTA1)),
    _led5(new PwmOut(PTC10)),
    _led6(new PwmOut(PTC11)),

    _button_A(new InterruptIn(PTC7)),
    _button_B(new InterruptIn(PTC9)),
    _button_X(new InterruptIn(PTC5)),
    _button_Y(new InterruptIn(PTC0)),
    _button_start(new InterruptIn(PTC8)),

    _vert(new AnalogIn(PTB11)),
    _horiz(new AnalogIn(PTB10)),

    _pot1(new AnalogIn(PTB2)),
    _pot2(new AnalogIn(PTB3)),

    dac(new AnalogOut(DAC0_OUT)),
    ticker(new Ticker),
    timeout(new Timeout),
    note_timeout(new Timeout),

    _x0(0),
    _y0(0)
{}


///////////////// public methods /////////////////

void Gamepad::init()
{
    leds_off();

    // read centred values of joystick
    _x0 = _horiz->read();
    _y0 = _vert->read();

    // Set all buttons to PullUp
    _button_A->mode(PullUp);
    _button_B->mode(PullUp);
    _button_X->mode(PullUp);
    _button_Y->mode(PullUp);
    _button_start->mode(PullUp);

    // Set up interrupts for the fall of buttons
    _button_A->fall(callback(this,&Gamepad::A_fall_interrupt));
    _button_B->fall(callback(this,&Gamepad::B_fall_interrupt));
    _button_X->fall(callback(this,&Gamepad::X_fall_interrupt));
    _button_Y->fall(callback(this,&Gamepad::Y_fall_interrupt));
    _button_start->fall(callback(this,&Gamepad::start_fall_interrupt));

    // initalise button flags
    reset_buttons();

    // number of samples
    _n = 16;
    _sample_array = new float[_n];

    // create sample array for one period between 0.0 and 1.0
    for (int i = 0; i < _n ; i++) {
        _sample_array[i] = 0.5f + 0.5f*sin(i*2*PI/_n);
        //printf("y[%i] = %f\n",i,_sample_array[i]);
    }

}

void Gamepad::leds_off()
{
    leds(0.0);
}

void Gamepad::leds_on()
{
    leds(1.0);
}

void Gamepad::leds(float val) const
{
    if (val < 0.0f) {
        val = 0.0f;
    }
    if (val > 1.0f) {
        val = 1.0f;
    }

    // leds are active-low, so subtract from 1.0
    // 0.0 corresponds to fully-off, 1.0 to fully-on
    val = 1.0f - val;

    _led1->write(val);
    _led2->write(val);
    _led3->write(val);
    _led4->write(val);
    _led5->write(val);
    _led6->write(val);
}

void Gamepad::led(int n,float val) const
{
    // ensure they are within valid range
    if (val < 0.0f) {
        val = 0.0f;
    }
    if (val > 1.0f) {
        val = 1.0f;
    }

    switch (n) {

        // check for valid LED number and set value

        case 1:
            _led1->write(1.0f-val);   // active-low so subtract from 1
            break;
        case 2:
            _led2->write(1.0f-val);   // active-low so subtract from 1
            break;
        case 3:
            _led3->write(1.0f-val);   // active-low so subtract from 1
            break;
        case 4:
            _led4->write(1.0f-val);   // active-low so subtract from 1
            break;
        case 5:
            _led5->write(1.0f-val);   // active-low so subtract from 1
            break;
        case 6:
            _led6->write(1.0f-val);   // active-low so subtract from 1
            break;

    }
}

float Gamepad::read_pot1() const
{
    return _pot1->read();
}

float Gamepad::read_pot2() const
{
    return _pot2->read();
}


// this method gets the magnitude of the joystick movement
float Gamepad::get_mag()
{
    Polar p = get_polar();
    return p.mag;
}

// this method gets the angle of joystick movement (0 to 360, 0 North)
float Gamepad::get_angle()
{
    Polar p = get_polar();
    return p.angle;
}

Direction Gamepad::get_direction()
{
    float angle = get_angle();  // 0 to 360, -1 for centred

    Direction d;
    // partition 360 into segments and check which segment the angle is in
    if (angle < 0.0f) {
        d = CENTRE;   // check for -1.0 angle
    } else if (angle < 22.5f) {  // then keep going in 45 degree increments
        d = N;
    } else if (angle < 67.5f) {
        d = NE;
    } else if (angle < 112.5f) {
        d = E;
    } else if (angle < 157.5f) {
        d = SE;
    } else if (angle < 202.5f) {
        d = S;
    } else if (angle < 247.5f) {
        d = SW;
    } else if (angle < 292.5f) {
        d = W;
    } else if (angle < 337.5f) {
        d = NW;
    } else {
        d = N;
    }

    return d;
}

void Gamepad::reset_buttons()
{
    A_fall = B_fall = X_fall = Y_fall = start_fall = false;
}

bool Gamepad::A_pressed()
{
    if (A_fall) {
        A_fall = false;
        return true;
    } else {
        return false;
    }
}

bool Gamepad::B_pressed()
{
    if (B_fall) {
        B_fall = false;
        return true;
    } else {
        return false;
    }
}

bool Gamepad::X_pressed()
{
    if (X_fall) {
        X_fall = false;
        return true;
    } else {
        return false;
    }
}

bool Gamepad::Y_pressed()
{
    if (Y_fall) {
        Y_fall = false;
        return true;
    } else {
        return false;
    }
}

bool Gamepad::start_pressed()
{
    if (start_fall) {
        start_fall = false;
        return true;
    } else {
        return false;
    }
}

bool Gamepad::A_held()
{
    // Buttons are configured as PullUp hence the not
    return !_button_A->read();
}

bool Gamepad::B_held()
{
    return !_button_B->read();
}

bool Gamepad::X_held()
{
    return !_button_X->read();
}

bool Gamepad::Y_held()
{
    return !_button_Y->read();
}

bool Gamepad::start_held()
{
    return !_button_start->read();
}

///////////////////// private methods ////////////////////////

// get raw joystick coordinate in range -1 to 1
// Direction (x,y)
// North     (0,1)
// East      (1,0)
// South     (0,-1)
// West      (-1,0)
Vector2D Gamepad::get_coord()
{
    // read() returns value in range 0.0 to 1.0 so is scaled and centre value
    // substracted to get values in the range -1.0 to 1.0
    float x = 2.0f*( _horiz->read() - _x0 );
    float y = 2.0f*( _vert->read()  - _y0 );

    // Note: the y value here is inverted to ensure the positive y is up

    Vector2D coord = {x,-y};
    return coord;
}

// This maps the raw x,y coord onto a circular grid.
// See:  http://mathproofs.blogspot.co.uk/2005/07/mapping-square-to-circle.html
Vector2D Gamepad::get_mapped_coord()
{
    Vector2D coord = get_coord();

    // do the transformation
    float x = coord.x*sqrt(1.0f-pow(coord.y,2.0f)/2.0f);
    float y = coord.y*sqrt(1.0f-pow(coord.x,2.0f)/2.0f);

    Vector2D mapped_coord = {x,y};
    return mapped_coord;
}

// this function converts the mapped coordinates into polar form
Polar Gamepad::get_polar()
{
    // get the mapped coordinate
    Vector2D coord = get_mapped_coord();

    // at this point, 0 degrees (i.e. x-axis) will be defined to the East.
    // We want 0 degrees to correspond to North and increase clockwise to 359
    // like a compass heading, so we need to swap the axis and invert y
    float x = coord.y;
    float y = coord.x;

    float mag = sqrt(x*x+y*y);  // pythagoras
    float angle = RAD2DEG*atan2(y,x);
    // angle will be in range -180 to 180, so add 360 to negative angles to
    // move to 0 to 360 range
    if (angle < 0.0f) {
        angle+=360.0f;
    }

    // the noise on the ADC causes the values of x and y to fluctuate slightly
    // around the centred values. This causes the random angle values to get
    // calculated when the joystick is centred and untouched. This is also when
    // the magnitude is very small, so we can check for a small magnitude and then
    // set the angle to -1. This will inform us when the angle is invalid and the
    // joystick is centred

    if (mag < TOL) {
        mag = 0.0f;
        angle = -1.0f;
    }

    Polar p = {mag,angle};
    return p;
}

// ISRs for buttons
void Gamepad::A_fall_interrupt()
{
    A_fall = true;
}
void Gamepad::B_fall_interrupt()
{
    B_fall = true;
}
void Gamepad::X_fall_interrupt()
{
    X_fall = true;
}
void Gamepad::Y_fall_interrupt()
{
    Y_fall = true;
}
void Gamepad::start_fall_interrupt()
{
    start_fall = true;
}

void Gamepad::set_bpm(float bpm)
{
    _bpm = bpm;
}

void Gamepad::tone(float frequency,float duration)
{
    // calculate time step between samples
    float dt = 1.0f/(frequency*_n);
    // start from beginning of LUT
    _sample = 0;

    // setup ticker and timeout to stop ticker

    // the ticker repeats every dt to plat each sample in turn
    ticker->attach(callback(this, &Gamepad::ticker_isr), dt);
    // the timeout stops the ticker after the required duration
    timeout->attach(callback(this, &Gamepad::timeout_isr), duration );
}

void Gamepad::play_melody(int length,const int *notes,const int *durations,float bpm,bool repeat)
{
    // copy arguments to member variables
    _bpm = bpm;
    _notes = notes;  // pointer for array
    _durations = durations;  // pointer for array
    _melody_length = length;
    _repeat = repeat;

    _note = 0;  // start from first note

    play_next_note(); // play the next note in the melody
}

void Gamepad::write_dac(float val)
{
    if (val < 0.0f) {
        val = 0.0f;
    } else if (val > 1.0f) {
        val = 1.0f;
    }
    dac->write(val);
}


void Gamepad::play_next_note()
{
    // _note is the note index to play

    // calculate the duration and frequency of the note
    float duration = 60.0f/(_bpm*_durations[_note]);
    float frequency = float(_notes[_note]);
    //printf("[%i] f = %f d = %f\n",_note,frequency,duration);

    // check if the note is not a space and if not then play the note
    if (frequency > 0) {
        tone(frequency,duration);
    }

    // the timeout goes to the next note in the melody
    // double the duration to leave a bit of a gap in between notes to be better
    // able to distinguish them
    note_timeout->attach(callback(this, &Gamepad::note_timeout_isr), duration*2.0f );
}

// called when the next note needs playing
void Gamepad::note_timeout_isr()
{
    _note++; // go onto next note

    // if in repeat mode then reset the note counter when get to end of melody
    if (_repeat && _note == _melody_length) {
        _note=0;
    }

    // check if note is within the melody
    if (_note < _melody_length) {
        play_next_note();
    }
}

void Gamepad::ticker_isr()
{
    dac->write(_sample_array[_sample%_n]);  // use modulo to get index to play
    _sample++;  // increment the sample ready for next time
}

void Gamepad::timeout_isr()
{
    // stops the ticker to end the note
    ticker->detach();
}
