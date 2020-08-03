// DC_ctrl_H_
#ifndef DC_CTRL_H_
#define DC_CTRL_H_

enum M_Locate{LEFT, RIGHT};             //Robot's Location of Motor; LEFT or RIGHT? 

extern const int  WHEELSIZE;       // Wheel Size
extern const int  WHEELBASE;      // Wheel to wheel distance
extern const int  ENCODER_RESOLUTION;      // Pulse Per Round (31gear * 13)402 Pulse/CH x 4 
extern const int  CONTROL_FREQUENCY;        // [ms]

extern const byte R_MOTOR_ENCOD_A;
extern const byte R_MOTOR_ENCOD_B;
extern const byte R_MOTOR_PWM;
extern const byte R_MOTOR_DIR;

extern const byte L_MOTOR_ENCOD_A;
extern const byte L_MOTOR_ENCOD_B;
extern const byte L_MOTOR_PWM;
extern const byte L_MOTOR_DIR;
 

/***************************************
*             DC Motor Class           *
***************************************/

class DCMotor{
private:
    //Pin Setup
    int MOTR_ENCOD_A_;
    int MOTR_ENCOD_B_;
    int MOTR_DIR_;
    int MOTR_PWM_;
    M_Locate M_Locate_;

    //PID Setup
    float P_gain_;
    float I_gain_;
    float D_gain_;
    
    //Encoder Position
    long Encoder_;
    long Position_prev_;
    long Position_current_;
    long Position_diff_;

    float  err_prev_;
    float  err_sum_;
    double Spd_target_;

    int  PWM_current_;

    double Velocity_; //current speed

    void WritePWM_(bool Dir, int PWM);  // Writing PWM on PWN Pin.
    double Trans_Spd2Encod_(double spd);   // Transforming Speed to encoder val 
    double Trans_Encod2Spd_(double encod); // Transforming Encoder val to Speed

public:
    bool Direction_current;
    
    DCMotor(int pin_encod_A, int pin_encod_B, int pin_direct, int pin_PWM, M_Locate M_LOC);
    ~DCMotor();

    void PIDgainSet(float P_gain, float I_gain, float D_gain);
    int EncoderPos();                     // Returning current encoder's position.
    void CalcEncodDiff();                 // Calulating Diff of Encoder.

    void MotorControl(float TargetSpd);   // Controlling MotorSpd using PID_Ctrl.
    void SetUpSpd(double TargetSpd);      // Setting up Target Speed.

    // double Trans_Spd2Encod(double spd);   // Transforming Speed to encoder val 
    // double Trans_Encod2Spd(double encod); // Transforming Encoder val to Speed

    void EncodDiff();
    void CalcSpd();
    void PID_Update();

    void callbackEncod_A();
    void callbackEncod_B();

    long  ShowEncoder();
    float ShowSpeed();
    float showDebug(int options);
};



#endif //DC_CTRL_H_