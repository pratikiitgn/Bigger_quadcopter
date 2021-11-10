#include "Copter.h"
#include <AP_HAL/AP_HAL.h>
#include <stdlib.h>
// #include <string.h>
#include <math.h>
// #include <tuple>


#include <AP_HAL/AP_HAL.h>
#include <AP_Motors/AP_Motors_Class.h>
// #include <AP_Vehicle/AP_Vehicle.h>


// #if CONFIG_HAL_BOARD == HAL_BOARD_CHIBIOS
// #include <AP_BoardConfig/AP_BoardConfig.h>
// #include <AP_IOMCU/AP_IOMCU.h>
// AP_BoardConfig BoardConfig;
// #endif



#define ESC_HZ 490

// #if CONFIG_HAL_BOARD == HAL_BOARD_CHIBIOS
// BoardConfig.init();
// #endif

// For yaw commands

int yaw_flag_start = 0;

float current_time  = 0.0;
float H_roll        = 0.0;
float H_pitch       = 0.0;
float H_yaw_rate    = 0.0;
float H_throttle    = 0.0;
float H_yaw         = 0.0;

float imu_roll      = 0.0;
float imu_pitch     = 0.0;
float imu_yaw       = 0.0;

float imu_roll_dot  = 0.0;
float imu_pitch_dot = 0.0;
float imu_yaw_dot   = 0.0;
float battvolt      = 0.0;

int PWM1 = 1000;
int PWM2 = 1000;
int PWM3 = 1000;
int PWM4 = 1000;

// uint32_t tstart = AP_HAL::millis();

/*
 * Init and run calls for mycontroller flight mode
 */
bool ModeMyController::init(bool)
{ 
    // hal.uartE->begin(115200);

    // #if CONFIG_HAL_BOARD == HAL_BOARD_CHIBIOS
    // BoardConfig.init();
    // #endif

    // for (uint8_t i = 0; i< 14; i++) {
    //     hal.rcout->enable_ch(i);
    //     hal.console->printf("Enable Channel %u \n", i);
    // }

    /////////////
    // #if CONFIG_HAL_BOARD == HAL_BOARD_CHIBIOS
    // //Change parameters for board
    // float SET_PWM_PARAMETER = 14;
    // AP_Param::set_object_value(&BoardConfig, BoardConfig.var_info, "PWM_COUNT", SET_PWM_PARAMETER);
    // AP_Param::set_object_value(&BoardConfig, BoardConfig.var_info, "SAFETYENABLE", DISABLE);
    // AP_Param::set_object_value(&BoardConfig, BoardConfig.var_info, "SAFETYOPTION", 0);// 0 1 2
    // AP_Param::set_object_value(&BoardConfig, BoardConfig.var_info, "SAFETY_MASK", 8191);
    // #endif

    // hal.rcout->set_freq(0, 490); //// set output frequency
    // hal.rcout->enable_ch(0); //ADDED
    // hal.rcout->write(0, 200); //ONLY CHANNEL 2; (uint8_t)2
//////////////////////////////// For PWM test ////////////////////////////////


    hal.rcout->set_freq( 15, 490); //0xFF  0x0F->b'00001111'
    hal.rcout->enable_ch(0);
    hal.rcout->enable_ch(1);
    hal.rcout->enable_ch(2);
    hal.rcout->enable_ch(3);

    H_yaw = 360.0-(ahrs.yaw_sensor)   / 100.0;     // degrees ;
    return true;
}

void ModeMyController::run()
{
    // apply simple mode transform to pilot input
    channel_arming();
    battery_check();
    pilot_input();
    imu_read();
    custom_pwm_code();
    custom_PID_controller(H_roll, H_pitch, H_yaw, 0.0 , 0.0, 0.0, 0.0 ,0.0);
}

void ModeMyController::custom_PID_controller(float des_phi, float des_theta, float des_psi,float des_phi_dot, float des_theta_dot, float des_psi_dot, float des_z, float des_z_dot){

    float mass = 1.5;
    float arm_length = 0.161;
    float FM_devided_FF ;
    if (battvolt >= 11.5 ){
         FM_devided_FF = 0.24;
    }else{
         FM_devided_FF = 0.31;
    }

    float Kp_phi      = 0.09;   // 0.09 (testbed)
    float Kp_theta    = 0.15;   // 0.15 (testbed)
    float Kp_psi      = 0.3;    // 0.3  (testbed)

    float Kd_phi      = 2.2;    // 1.6 (testbed)
    float Kd_theta    = 3.0;    // 2.5 (testbed)
    float Kd_psi      = 6.0;    // 6.0 (testbed)

    float Kp_z        = 0.0;
    float Kd_z        = 0.0;

    float z           = 0.0;
    float z_dot       = 0.0;

    float e_phi       = des_phi   - imu_roll;
    float e_theta     = des_theta - imu_pitch;
    float e_z         = des_z - z;

    float Mb1 = Kp_phi    * saturation_for_roll_pitch_angle_error(e_phi)    + Kd_phi    * (des_phi_dot - imu_roll_dot);
    float Mb2 = Kp_theta  * saturation_for_roll_pitch_angle_error(e_theta)  + Kd_theta  * (des_theta_dot - imu_pitch_dot);
    float F  =  mass * GRAVITY_MSS + Kp_z * (e_z)                           + Kd_z * (des_z_dot - z_dot);

    // ////// Yaw controller with (1 - cos ) function //////
    // float Mb3 = 0.0;
    // if ( imu_yaw >= 0 && imu_yaw < 180.0 ){
    //     Mb3 = -Kp_psi * ( (1 - cosf(des_psi)) - (1 - cosf(imu_yaw))  )  + Kd_psi    * (des_psi_dot - imu_yaw_dot);
    // }
    // if (imu_yaw >= 180.0 && imu_yaw < 360.0 ){
    //     Mb3 =  Kp_psi * ( (1 - cosf(des_psi)) - (1 - cosf(imu_yaw))  )  + Kd_psi    * (des_psi_dot - imu_yaw_dot);
    // }

    ////// Simple yaw rate controller //////
    // float Mb3 = Kd_psi    * (des_psi_dot - imu_yaw_dot);
    ////// Simple yaw control law /////


    ////// Yaw controller with R2 //////

    // e_psi = acosf( cosf(des_psi)*cosf(imu_yaw)  + sinf(des_psi)*sinf(imu_yaw)  );

    // if (sinf(des_psi) < 0 ){
    //     e_psi = -e_psi;
    // }

    // float Mb3 = Kp_psi    * saturation_for_yaw_angle_error(e_psi)           + Kd_psi    * (des_phi_dot - imu_yaw_dot);
    

    ////// Yaw controller customized //////
    float e_psi     = des_psi   - imu_yaw;

    if (e_psi > 0.0){

        if ( e_psi > 180.0 ){
            e_psi = -(360.0 - e_psi);
        }

    }else if ( e_psi < 0.0 ) {
        if ( -e_psi < 180.0 ){
            e_psi = e_psi;
        }else{
            e_psi = 360.0 + e_psi;
        }
    }

    hal.console->printf("Yaw__ %f H_yaw ->  %f e_psi %f \n",imu_yaw,H_yaw, e_psi);


    float Mb3       = Kp_psi  * saturation_for_yaw_angle_error(e_psi)  + Kd_psi  * (des_psi_dot - imu_yaw_dot);    
    Mb3 = -Mb3;
    Mb3 = 0.0;
    ////// Yaw controller customized //////

///////////////////// Prioritizing the control commands [start] /////////////////////

    float step_1 = 10.0;
    float step_2 = 20.0;
    float step_3 = 30.0;
    float step_4 = 40.0;

    if ( abs(e_phi) < step_1 && abs(e_theta) < step_1){
        // Mb2 = Mb2;
        Mb3 = Mb3;
        // F   = F;
    }else if ( step_1 < abs(e_phi) && abs(e_phi) < step_2 && step_1 < abs(e_theta) && abs(e_theta) < step_2){
        // Mb2 = 0.9*Mb2;
        Mb3 = 0.9*Mb3;
        // F   = 0.8*F;
    }else if ( step_2 < abs(e_phi) && abs(e_phi) < step_3 && step_2 < abs(e_theta) && abs(e_theta) < step_3){
        // Mb2 = 0.8*Mb2;
        Mb3 = 0.8*Mb3;
        // F   = 0.8*F;
    }else if ( step_3 < abs(e_phi) && abs(e_phi) < step_4 && step_3 < abs(e_theta) && abs(e_theta) < step_4){
        // Mb2 = 0.7*Mb2;
        Mb3 = 0.7*Mb3;
        F   = F;
    }else{
        // Mb2 = Mb2;
        Mb3 = Mb3;
        // F   = F;
    }

///////////////////// Prioritizing the control commands [end] /////////////////////

    float function_F1 = F/4 + Mb1 / (4 * arm_length) - Mb2 / (4 * arm_length) -  Mb3 / (4 * FM_devided_FF);
    float function_F2 = F/4 - Mb1 / (4 * arm_length) - Mb2 / (4 * arm_length) +  Mb3 / (4 * FM_devided_FF);
    float function_F3 = F/4 + Mb1 / (4 * arm_length) + Mb2 / (4 * arm_length) +  Mb3 / (4 * FM_devided_FF);
    float function_F4 = F/4 - Mb1 / (4 * arm_length) + Mb2 / (4 * arm_length) -  Mb3 / (4 * FM_devided_FF);

    // hal.console->printf("F1-> %f, F2-> %f, F3-> %f, F4-> %f  \n", function_F1, function_F2, function_F3, function_F4);

    PWM1 = Inverse_thrust_function(function_F1);
    PWM2 = Inverse_thrust_function(function_F2);
    PWM3 = Inverse_thrust_function(function_F3);
    PWM4 = Inverse_thrust_function(function_F4);

    // hal.console->printf("Yaw__ ->  %f H_yaw ->  %f, Mb3 -> %f\n",imu_yaw,H_yaw, Mb3);
    // hal.console->printf("PWM1-> %d, PWM2-> %d, PWM3-> %d, PWM4-> %d  \n", PWM1, PWM2, PWM3, PWM4);

}

float ModeMyController::saturation_for_yaw_angle_error(float error){

    float lim = 30.0;

    if (error > lim){
        error = lim;
    }else if (error < -lim){
        error = -lim;
    }else {
        error = error;
    }
    return error;
}

float ModeMyController::saturation_for_roll_pitch_angle_error(float error){

    float lim = 45.0;

    if (error > lim){
        error = lim;
    }else if (error < -lim){
        error = -lim;
    }else {
        error = error;
    }
    return error;
}

float ModeMyController::saturation_for_altitude_error(float z){

    float lim = 45.0;

    if (z > lim){
        z = lim;
    }else if (z < -lim){
        z = -lim;
    }else {
        z = z;
    }
    return z;
}


int ModeMyController::Inverse_thrust_function(float Force){
    int PWM = 1000;
    // hal.console->printf("From Inverse function \n");

    // if (battvolt >= 11.5 ){
    //     if (Force >=0 && Force < 1.87)
    //     {
    //         PWM = 1000 * ( (37641.0/37480.0) +  (25/1874.0)*sqrtf( (3748.0*Force/25.0) + (11607241.0/250000.0) )  );
    //     }else if (Force >=1.87 && Force < 8.53)
    //     {
    //         PWM = 1000 * ( (10017.0/6310.0)  -  (5.0/631.0)*sqrtf( (5690289.0/2500.0) - (1262.0*Force/5.0) )  );
    //     }else if (Force > 8.53)
    //     {
    //         PWM  = 1500;
    //     }else 
    //     {
    //         PWM  = 1000;
    //     }
    // } else if (battvolt < 11.5)
    // {
    //     if (Force >=0 && Force < 1.34)
    //     {
    //         PWM = 1000 * ( (6871.0/6223.0) -  (50/6223.0)*sqrtf( (6223.0*Force/25.0) + (113499.0/25000.0) )  );
    //     }else if (Force >=1.34 && Force < 6.1)
    //     {
    //         PWM = 1000 * ( (40395.0/32878.0)  +  (250.0/49317.0)*sqrtf( (49317.0/125.0) - (5160903.0*Force/10000.0) )  );
    //     }else if (Force > 8.53)
    //     {
    //         PWM  = 1500;
    //     }else 
    //     {
    //         PWM  = 1000;
    //     }
    // }



    if (Force >=0 && Force < 1.87)
    {
        PWM = 1000 * ( (37641.0/37480.0) +  (25/1874.0)*sqrtf( (3748.0*Force/25.0) + (11607241.0/250000.0) )  );
    }else if (Force >=1.87 && Force < 8.53)
    {
        PWM = 1000 * ( (10017.0/6310.0)  -  (5.0/631.0)*sqrtf( (5690289.0/2500.0) - (1262.0*Force/5.0) )  );
    }else if (Force > 8.53)
    {
        PWM  = 1500;
    }else 
    {
        PWM  = 1000;
    }








    return PWM;
}

void ModeMyController::custom_pwm_code(){

    if (RC_Channels::get_radio_in(CH_6) > 1600){

        copter.init_rc_out();
        // int pwm__ = H_throttle + 1000;

        SRV_Channels::cork();

        hal.rcout->write(0,PWM1);
        hal.rcout->write(1,PWM2);
        hal.rcout->write(2,PWM3);
        hal.rcout->write(3,PWM4);

        SRV_Channels::push();

        // hal.console->printf("PWM1-> %d, PWM2-> %d, PWM3-> %d, PWM4-> %d  \n", PWM1, PWM2, PWM3, PWM4);

    }
}

void ModeMyController::channel_arming(){
    if (RC_Channels::get_radio_in(CH_6) < 1200){
        copter.motors->armed(false);
    }
    else if (RC_Channels::get_radio_in(CH_6) > 1400 && RC_Channels::get_radio_in(CH_6) < 1600 ){
        // motors->set_desired_spool_state(AP_Motors::DesiredSpoolState::GROUND_IDLE);
        copter.motors->armed(true);
    }
}

void ModeMyController::pilot_input(){

    H_roll      = (double)(channel_roll->get_control_in())/100.0;
    H_pitch     = (double)(channel_pitch->get_control_in())/100.0;
    H_yaw_rate  = (double)(channel_yaw->get_control_in()) / 100.0;
    H_throttle  = (double)channel_throttle->get_control_in();

    float dt = 1.0/200.0;
    H_yaw = wrap_360(H_yaw + H_yaw_rate*dt);


    // hal.serial(1)->begin(57600);
    // hal.console->printf("pitch: %f ,roll: %f ,yaw: %f ,throttle: %f \n",H_pitch,H_roll,H_yaw,H_throttle);
}

void ModeMyController::imu_read(){
    
    imu_roll        =  (ahrs.roll_sensor)  / 100.0;     // degrees 
    imu_pitch       = -(ahrs.pitch_sensor) / 100.0;     // degrees 
    imu_yaw         = 360.0-(ahrs.yaw_sensor)   / 100.0;     // degrees 
    imu_roll_dot    =  (ahrs.get_gyro().x);             // degrees/second
    imu_pitch_dot   =  -(ahrs.get_gyro().y);             // degrees/second    
    imu_yaw_dot     = -(ahrs.get_gyro().z);             // degrees/second

    // hal.console->printf("Hi..\n");

    //hal.console->printf("roll_dot: %f pitch_dot %f yaw_dot %f",imu_roll_dot,imu_pitch_dot,imu_yaw_dot);
    // hal.serial(1)->begin(57600);
    // hal.console->printf("roll: %f ,pitch: %f ,yaw: %f \n",imu_roll,imu_pitch,imu_yaw);
    // hal.console->printf("R: %f ,Hr: %f ,P: %f ,Hp: %f \n",imu_roll,H_roll,imu_pitch,H_pitch);

}

void ModeMyController::battery_check(){
    battvolt=copter.battery_volt();

    // battvolt=copter.battery.voltage();
    // hal.serial(1)->begin(57600);
    // hal.serial(1)->printf("%f",battvolt);
    // hal.console->printf("%f\n",battvolt);
    // hal.console->printf("Hi..\n");
}

// void ModeMyController::getEncoderData()
// {
//     bool receiving_data = false;
//     int index = 0;
//     char startChar = ',';
//     char endChar = '/';
//     bool new_data = false;

//     char attitude[] = "50000_50000";
//     while (hal.uartE->available()>0 && new_data == false)
//         {
//             char temp = hal.uartE->read();
//             if (receiving_data == true)
//             {
//                 if (temp != endChar)
//                 {   
//                     attitude[index] = temp;
//                     index++;
//                 }
//                 else
//                 {    
//                     attitude[index] = '\0';
//                     receiving_data = false;
//                     new_data = false;
//                     index = 0;
//                 }
//             }
//             else if (temp == startChar)
//             {
//                 receiving_data = true;
//                 index = 0; 
//             }
//         }
//         // hal.uartE->printf("%s\n",attitude);

//         char roll_char[]        = "11111";
//         char pitch_char[]       = "11111";

//         for (int i = 0; i < 11; i++)
//         {
//             if (i < 5)  
//             {
//                 roll_char[i]                = attitude[i];
//             } else if (i >= 6 && i < 11 )
//             {
//                 pitch_char[i - 6]           = attitude[i];
//             }
//         }

//         int encoder_roll_int      = atoi(roll_char);
//         int encoder_pitch_int     = atoi(pitch_char);

//         encoder_roll_feedback  = (float)((encoder_roll_int  - 50000.0) / 100.0);
//         encoder_pitch_feedback = (float)((encoder_pitch_int - 50000.0) / 100.0);

//         if (encoder_roll_feedback > 60.0){
//             encoder_roll_feedback = 60.0;
//         }
//         if (encoder_roll_feedback < -60.0){
//             encoder_roll_feedback = -60.0;
//         }

//         if (encoder_pitch_feedback > 60.0){
//             encoder_pitch_feedback = 60.0;
//         }
//         if (encoder_pitch_feedback < -60.0){
//             encoder_pitch_feedback = -60.0;
//         }

//         hal.uartE->printf("%0.3f,", encoder_roll_feedback);
//         hal.uartE->printf("%0.3f\n", encoder_pitch_feedback);
// }
