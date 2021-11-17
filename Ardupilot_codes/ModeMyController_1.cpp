#include "Copter.h"
#include <AP_HAL/AP_HAL.h>
#include <stdlib.h>
#include <math.h>
#include <AP_Baro/AP_Baro.h>
#include <AP_HAL/AP_HAL.h>
#include <AP_Motors/AP_Motors_Class.h>
#include "mycontroller_usercode.h"
#define ESC_HZ 490

// For yaw commands
int yaw_flag_start = 0;

float current_time  = 0.0;
float H_roll        = 0.0;
float H_pitch       = 0.0;
float H_yaw_rate    = 0.0;
float H_throttle    = 0.0;
float H_yaw         = 0.0;

float H_roll_dot    = 0.0;
float H_pitch_dot   = 0.0;

float H_pitch_prev  = 0.0;
float H_roll_prev   = 0.0;

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


int pwm__thrust_measurement = 1000;

int flag_thrust_measurement = 0;

float fil_z     = 0.0;
float fil_z_1   = 0.0;
float fil_z_2   = 0.0;
float fil_z_3   = 0.0;
float fil_z_4   = 0.0;
float fil_z_5   = 0.0;
float fil_z_6   = 0.0;
float fil_z_7   = 0.0;
float fil_z_8   = 0.0;
float fil_z_9   = 0.0;
float fil_z_10  = 0.0;
float fil_z_11  = 0.0;

float fil_ph    = 0.0;
float fil_ph_1  = 0.0;
float fil_ph_2  = 0.0;
float fil_ph_3  = 0.0;
float fil_ph_4  = 0.0;
float fil_ph_5  = 0.0;

float fil_th    = 0.0;
float fil_th_1  = 0.0;
float fil_th_2  = 0.0;
float fil_th_3  = 0.0;
float fil_th_4  = 0.0;
float fil_th_5  = 0.0;

float e_phi_prev    = 0.0;
float e_theta_prev  = 0.0;
float e_psi_prev    = 0.0;

float tstart = 0.0;
float time_thrust_mea = 0.0;

float z_des  = 0.0;
float z_des_init = 0.0;

/*
 * Init and run calls for mycontroller flight mode
 */
bool ModeMyController::init(bool)
{
        hal.rcout->set_freq( 15, 490); //0xFF  0x0F->b'00001111'
        hal.rcout->enable_ch(0);
        hal.rcout->enable_ch(1);
        hal.rcout->enable_ch(2);
        hal.rcout->enable_ch(3);

        H_yaw   = 360.0-(ahrs.yaw_sensor)   / 100.0;     // degrees ;
        tstart  = AP_HAL::millis();
        z_des_init     = inertial_nav.get_altitude()/100.0;
        return true;
    
}

void ModeMyController::custom_PID_controller(float des_phi, float des_theta, float des_psi,float des_phi_dot, float des_theta_dot, float des_psi_dot, float des_z, float des_z_dot){

    float mass = 0.75;
    float arm_length = 0.161;
    float FM_devided_FF ;
    if (battvolt >= 11.5 ){
         FM_devided_FF = 0.24;
    }else{
         FM_devided_FF = 0.31;
    }

    float Kp_phi      = 0.020;   // 0.02 (best) // 0.09 (out 1) // 0.09 (testbed)
    float Kp_theta    = 0.025;   // 0.025 (best)    //0.15 (out 1) // 0.15 (testbed)
    float Kp_psi      = 0.1;    // 0.3 (out 1)  // 0.3  (testbed)

    float Kd_phi      = 0.4;    // 0.4 (best) // 0.25 (lab 1) // 2.0 (out 1) 1.6 (testbed)
    float Kd_theta    = 0.3;    // 0.4 (best) // 0.5 (lab 1) // // 3.0 (out 1) 2.5 (testbed)
    float Kd_psi      = 0.3;    // // 6.0 (testbed)

    float Ki_phi      = 0.0;   // 0.0005 (lab 1)
    float Ki_theta    = 0.0;   // 0.0005 (lab 1)
    float Ki_psi      = 0.0;    // 0.0005 (lab 1)

    float e_phi       = des_phi   - imu_roll;
    float e_theta     = des_theta - imu_pitch;

    float e_phi_sum   = e_phi + e_phi_prev;
    float Mb1 = Kp_phi    * saturation_for_roll_pitch_angle_error(e_phi)    + Kd_phi    * (des_phi_dot - imu_roll_dot) + Ki_phi * sat_I_gain_ph_th(e_phi_sum);
    // float Mb1 = Kp_phi    * saturation_for_roll_pitch_angle_error(e_phi)    + Kd_phi    * (des_phi_dot - imu_roll_dot) - Ki_phi * sat_I_gain_ph_th(e_phi_sum);
    e_phi_prev = e_phi;

    float e_theta_sum   = e_theta + e_theta_prev;
    float Mb2 = Kp_theta  * saturation_for_roll_pitch_angle_error(e_theta)  + Kd_theta  * (des_theta_dot - imu_pitch_dot) + Ki_theta * sat_I_gain_ph_th(e_theta_sum);
    // float Mb2 = Kp_theta  * saturation_for_roll_pitch_angle_error(e_theta)  + Kd_theta  * (des_theta_dot - imu_pitch_dot) - Ki_theta * sat_I_gain_ph_th(e_theta_sum);
    e_theta_prev = e_theta;


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

    // hal.console->printf("Yaw__ %f H_yaw ->  %f e_psi %f \n",imu_yaw,H_yaw, e_psi);
    float e_psi_sum     = e_psi + e_psi_prev;
    float Mb3           = -(Kp_psi  * saturation_for_yaw_angle_error(e_psi)  + Kd_psi  * (des_psi_dot - imu_yaw_dot)) + Ki_psi * sat_I_gain_psi(e_psi_sum);;    
    e_psi_prev          = e_psi;
    // Mb3 = 0.0;
    ////// Yaw controller customized //////


///////////////////// Altitude controller /////////////////////


    // fil_z_4 = fil_z_3;
    // fil_z_3 = fil_z_2;
    // fil_z_2 = fil_z_1;
    // fil_z_1 = copter.barometer.get_altitude() * 100.0f;
    // float baro_alt = 0.5*fil_z_1 + 0.2*fil_z_2 + 0.2*fil_z_3 + 0.1*fil_z_4;

    copter.barometer.update();
    // float z           = AP::baro().get_altitude();
    // float z_dot       = AP::baro().get_climb_rate();
    float z = inertial_nav.get_altitude()/100.0 - z_des_init;
    // float z = AP::baro().get_altitude_difference();
    float z_dot = inertial_nav.get_velocity().z/100.0 ;

    fil_z_11 = fil_z_10;
    fil_z_10 = fil_z_9;
    fil_z_9 = fil_z_8;
    fil_z_8 = fil_z_7;
    fil_z_7 = fil_z_6;
    fil_z_6 = fil_z_5;
    fil_z_5 = fil_z_4;
    fil_z_4 = fil_z_3;
    fil_z_3 = fil_z_2;
    fil_z_2 = fil_z_1;
    fil_z_1 = z;
    fil_z = (fil_z_1+ fil_z_2+ fil_z_3+ fil_z_4+ fil_z_5+ fil_z_6+ fil_z_7+ fil_z_8+ fil_z_9+ fil_z_10+ fil_z_11)/11.0;

    float e_z   = z_des - z;
    // float F = 0.0;
    // if (H_throttle > -400.0){
    //     F = 0.0;
    // }else{
    float Kp_z        = 2.0;
    float Kd_z        = 0.5;
    float F     =  mass * GRAVITY_MSS + Kp_z * (e_z) + Kd_z * (des_z_dot - z_dot);
    // }

    // if (F > 25.0){
    //     F = 25.0;
    // }
    
    if (F > 20.0){
        F = 20.0;
    }
    
    if (F < 0.0){
        F =  0.0;
    }
    // F = 7.2;

    hal.console->printf("z: %f , z_dot %f ,z_des: %f ,F: %f , Th %f \n", fil_z,z_dot, z_des, F, H_throttle);

    // F = H_throttle/50.0;
    // hal.console->printf(" %f \n ", F);
    // hal.console->printf(" Altitude %f \n ", baro_alt);

///////////////////// Prioritizing the control commands [start] /////////////////////

    // float step_1 = 10.0;
    // float step_2 = 20.0;
    // float step_3 = 30.0;
    // float step_4 = 40.0;

    // if ( abs(e_phi) < step_1 && abs(e_theta) < step_1){
    //     // Mb2 = Mb2;
    //     Mb3 = Mb3;
    //     // F   = F;
    // }else if ( step_1 < abs(e_phi) && abs(e_phi) < step_2 && step_1 < abs(e_theta) && abs(e_theta) < step_2){
    //     // Mb2 = 0.9*Mb2;
    //     Mb3 = 0.9*Mb3;
    //     // F   = 0.8*F;
    // }else if ( step_2 < abs(e_phi) && abs(e_phi) < step_3 && step_2 < abs(e_theta) && abs(e_theta) < step_3){
    //     // Mb2 = 0.8*Mb2;
    //     Mb3 = 0.8*Mb3;
    //     // F   = 0.8*F;
    // }else if ( step_3 < abs(e_phi) && abs(e_phi) < step_4 && step_3 < abs(e_theta) && abs(e_theta) < step_4){
    //     // Mb2 = 0.7*Mb2;
    //     Mb3 = 0.7*Mb3;
    //     F   = F;
    // }else{
    //     // Mb2 = Mb2;
    //     Mb3 = Mb3;
    //     // F   = F;
    // }

///////////////////// Prioritizing the control commands [end] /////////////////////
    // Mb1=0.0;
    // Mb2=0.0;
    // Mb3=0.0;
    
    float function_F1 = F/4.0 + Mb1 / (4.0 * arm_length) - Mb2 / (4.0 * arm_length) -  Mb3 / (4.0 * FM_devided_FF);
    float function_F2 = F/4.0 - Mb1 / (4.0 * arm_length) - Mb2 / (4.0 * arm_length) +  Mb3 / (4.0 * FM_devided_FF);
    float function_F3 = F/4.0 + Mb1 / (4.0 * arm_length) + Mb2 / (4.0 * arm_length) +  Mb3 / (4.0 * FM_devided_FF);
    float function_F4 = F/4.0 - Mb1 / (4.0 * arm_length) + Mb2 / (4.0 * arm_length) -  Mb3 / (4.0 * FM_devided_FF);

    // hal.console->printf("F1-> %f, F2-> %f, F3-> %f, F4-> %f  \n", function_F1, function_F2, function_F3, function_F4);

    PWM1 = Inverse_thrust_function(function_F1);
    PWM2 = Inverse_thrust_function(function_F2);
    PWM3 = Inverse_thrust_function(function_F3);
    PWM4 = Inverse_thrust_function(function_F4);

    // hal.console->printf("F -> %f Roll ->  %f Pitch ->  %f Sum_phi %f Sum_theta %f \n",F, imu_roll, imu_pitch, sat_I_gain_psi(e_phi_sum), sat_I_gain_psi(e_theta_sum));
    // hal.console->printf("Yaw__ ->  %f H_yaw ->  %f, Mb3 -> %f\n",imu_yaw,H_yaw, Mb3);
    // hal.console->printf("PWM %d,%d,%d,%d \n", PWM1, PWM2, PWM3, PWM4);
    // hal.console->printf("Each N %f,%f,%f,%f \n", function_F1, function_F2, function_F3, function_F4);

}

// float ModeMyController::Traj_plan_roll(float t1, float x, float x_dot, float xd){
//     // float t2 = t1 + dt;
//     // float AA = -(2*x - 2*xd - t1*x_dot + t2*x_dot)/(t1 - t2)^3;
//     // float BB = -(t1*(3*xd - 3*x + t2*x_dot) - 3*t2*x + 3*t2*xd + t1^2*x_dot - 2*t2^2*x_dot)/(t1 - t2)^3;
//     // float CC = -(t2^3*x_dot - 2*t1^2*t2*x_dot + t1*t2*(6*x - 6*xd + t2*x_dot))/(t1 - t2)^3;
//     // float DD = -(- xd*t1^3 + x_dot*t1^2*t2^2 + 3*xd*t1^2*t2 - x_dot*t1*t2^3 - 3*x*t1*t2^2 + x*t2^3)/(t1 - t2)^3;

//     // float x_des = AA * + BB * + CC * + DD;


// }


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

float ModeMyController::sat_I_gain_ph_th(float sum){

    float lim = 20.0;

    if (sum > lim){
        sum = lim;
    }else if (sum < -lim){
        sum = -lim;
    }else {
        sum = sum;
    }
    return sum;
}

float ModeMyController::sat_I_gain_psi(float sum){

    float lim = 20.0;

    if (sum > lim){
        sum = lim;
    }else if (sum < -lim){
        sum = -lim;
    }else {
        sum = sum;
    }
    return sum;
}

int ModeMyController::Inverse_thrust_function(float Force){
    int PWM = 1200;

/////////////////////////// From the quadcopter motors  ///////////////////////////

    if (battvolt >= 11.5 ){
        
        PWM = 1000 * (0.9206 + (sqrtf(12.8953 + 30.3264*Force)/(15.1632)));
    }else{

        PWM = 1000 * (0.6021 + (sqrtf(33.2341 + 19.418*Force)/(9.5740)));
    }

    if (PWM > 2000){PWM = 2000;}

    if (PWM < 1000){PWM = 1000;}


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

    // if (Force >=0 && Force < 1.87)
    // {
    //     PWM = 1000 * ( (37641.0/37480.0) +  (25/1874.0)*sqrtf( (3748.0*Force/25.0) + (11607241.0/250000.0) )  );
    // }else if (Force >=1.87 && Force < 8.53)
    // {
    //     PWM = 1000 * ( (10017.0/6310.0)  -  (5.0/631.0)*sqrtf( (5690289.0/2500.0) - (1262.0*Force/5.0) )  );
    //     hal.console->printf("In second");
    // }else if (Force > 8.53)
    // {
    //     PWM  = 1500;
    //     hal.console->printf("In third");
    // }else 
    // {
    //     PWM  = 1000;
    // }

/////////////////////////// modified one  ///////////////////////////
    // if (Force >=0 && Force < 1.87)
    // {
    //     PWM = 1000 * ( (37641.0/37480.0) +  (25/1874.0)*sqrtf( (3748.0*Force/25.0) + (11607241.0/250000.0) )  );
    // }
    // else if (Force >=1.87)
    // {
    //     PWM = 1000 * ( (10017.0/6310.0)  -  (5.0/631.0)*sqrtf( (5690289.0/2500.0) - (1262.0*Force/5.0) )  );
    // }

    // PWM = 1000 * (1.0 + (Force/10.0) );

    // if (PWM > 2000){
    //     PWM = 2000;
    // }

    // if (PWM < 1000){
    //         PWM = 1000;
    // }

    return PWM;
}

void ModeMyController::custom_pwm_code(){

    // if (RC_Channels::get_radio_in(CH_6) > 1600){

        copter.init_rc_out();
        SRV_Channels::cork();
        hal.rcout->write(0,PWM1);
        hal.rcout->write(1,PWM2);
        hal.rcout->write(2,PWM3);
        hal.rcout->write(3,PWM4);
        SRV_Channels::push();

        // hal.console->printf("PWM1-> %d, PWM2-> %d, PWM3-> %d, PWM4-> %d  \n", PWM1, PWM2, PWM3, PWM4);

    // }
}

void ModeMyController::channel_arming(){
    if (RC_Channels::get_radio_in(CH_6) < 1200){
        copter.motors->armed(false);
        motors->set_desired_spool_state(AP_Motors::DesiredSpoolState::SHUT_DOWN);  // AP_Motors class.cpp libraries
        // stop_logging();
        z_des = 0.0;
    }
    else if (RC_Channels::get_radio_in(CH_6) > 1400 && RC_Channels::get_radio_in(CH_6) < 1600 ){
        copter.motors->armed(true);
        motors->set_desired_spool_state(AP_Motors::DesiredSpoolState::GROUND_IDLE);
        // start_logging();
        // logging_checks();
            // AP::logger().Write("TEST", "TimeUS, pratik",
            //        "sm", // units: seconds, meters
            //        "FB", // mult: 1e-6, 1e-2
            //        "Qf", // format: uint64_t, float
            //        AP_HAL::micros64(),
            //        (double)10.0);
        z_des = 0.0;
    }
}

void ModeMyController::pilot_input(){

    // fil_ph_5    = fil_ph_4;    
    // fil_ph_4    = fil_ph_3;
    // fil_ph_3    = fil_ph_2;
    // fil_ph_2    = fil_ph_1;
    // fil_ph_1    = fil_ph;
    // fil_ph      = (double)(channel_roll->get_control_in())/100.0;
    // H_roll      = (fil_ph + fil_ph_1+fil_ph_2+fil_ph_3+fil_ph_4+fil_ph_5)/6.0;
    H_roll      = (double)(channel_roll->get_control_in())/100.0;
    H_roll_dot  = (H_roll - H_roll_prev)/400.0;
    H_roll_prev = H_roll;

    // fil_th_5    = fil_th_4;    
    // fil_th_4    = fil_th_3;
    // fil_th_3    = fil_th_2;
    // fil_th_2    = fil_th_1;
    // fil_th_1    = fil_th;
    // fil_th      = (double)(channel_pitch->get_control_in())/100.0;
    // H_pitch     = (fil_th + fil_th_1+fil_th_2+fil_th_3+fil_th_4+fil_th_5)/6.0;
    H_pitch     = (double)(channel_pitch->get_control_in())/100.0;
    H_pitch_dot = (H_pitch - H_pitch_prev)/400.0;
    H_pitch_prev= H_pitch;

    H_yaw_rate  = -(double)(channel_yaw->get_control_in()) / 100.0;
    H_throttle  = (double)channel_throttle->get_control_in()-500.0;

    float dt_yaw = 1.0/100.0;
    H_yaw = wrap_360(H_yaw + H_yaw_rate*dt_yaw);

    if (H_throttle > -20 && H_throttle < 20){
        H_throttle = 0.0;
    }

    float dt_z = 1.0/15000.0;
    z_des       =  z_des + H_throttle * dt_z;

    if (z_des > 5.0){
        z_des = 5.0;
    }
    if (z_des < 0.0){
        z_des = 0.0;
    }
    // z_des = 200.0;
    // hal.serial(1)->begin(57600);
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
}

void ModeMyController::thrust_measurement_code(){

    time_thrust_mea = (AP_HAL::millis() - tstart)/1000.0;

    if (time_thrust_mea < 5.0){
        pwm__thrust_measurement = 1000;
    }else if (time_thrust_mea > 5.0 && time_thrust_mea < 7.0){
        pwm__thrust_measurement = 1050;
    }else if (time_thrust_mea > 7.0 && time_thrust_mea < 9.0){
        pwm__thrust_measurement = 1100;
    }else if (time_thrust_mea > 9.0 && time_thrust_mea < 11.0){
        pwm__thrust_measurement = 1150;
    }else if (time_thrust_mea > 11.0 && time_thrust_mea < 13.0){
        pwm__thrust_measurement = 1200;
    }else if (time_thrust_mea > 13.0 && time_thrust_mea < 15.0){
        pwm__thrust_measurement = 1250;
    }else if (time_thrust_mea > 15.0 && time_thrust_mea < 17.0){
        pwm__thrust_measurement = 1300;
    }else if (time_thrust_mea > 17.0 && time_thrust_mea < 19.0){
        pwm__thrust_measurement = 1350;
    }else if (time_thrust_mea > 19.0 && time_thrust_mea < 21.0){
        pwm__thrust_measurement = 1400;
    }else if (time_thrust_mea > 21.0 && time_thrust_mea < 23.0){
        pwm__thrust_measurement = 1450;
    }else if (time_thrust_mea > 23.0 && time_thrust_mea < 25.0){
        pwm__thrust_measurement = 1500;
    }else if (time_thrust_mea > 25.0 && time_thrust_mea < 27.0){
        pwm__thrust_measurement = 1550;
    }else if (time_thrust_mea > 27.0 && time_thrust_mea < 29.0){
        pwm__thrust_measurement = 1600;
    }else if (time_thrust_mea > 29.0 && time_thrust_mea < 31.0){
        pwm__thrust_measurement = 1650;
    }else if (time_thrust_mea > 31.0 && time_thrust_mea < 33.0){
        pwm__thrust_measurement = 1700;
    }else if (time_thrust_mea > 33.0 && time_thrust_mea < 35.0){
        pwm__thrust_measurement = 1750;
    }else if (time_thrust_mea > 35.0 && time_thrust_mea < 37.0){
        pwm__thrust_measurement = 1800;
    }else if (time_thrust_mea > 37.0 && time_thrust_mea < 39.0){
        pwm__thrust_measurement = 1850;
    }else if (time_thrust_mea > 39.0 && time_thrust_mea < 41.0){
        pwm__thrust_measurement = 1900;
    }else if (time_thrust_mea > 41.0 && time_thrust_mea < 43.0){
        pwm__thrust_measurement = 1950;
    }else if (time_thrust_mea > 43.0 && time_thrust_mea < 45.0){
        pwm__thrust_measurement = 2000;
    }else if (time_thrust_mea > 45.0){
        pwm__thrust_measurement = 1000;
    }

    hal.console->printf("time %f, PWM - %d\n",time_thrust_mea,pwm__thrust_measurement); 

    copter.init_rc_out();
    SRV_Channels::cork();
    hal.rcout->write(0,pwm__thrust_measurement);
    hal.rcout->write(1,1000);
    hal.rcout->write(2,1000);
    hal.rcout->write(3,1000);
    SRV_Channels::push();

}
void ModeMyController::run()
{

// convert pilot input to lean angles
    float target_roll, target_pitch;
    get_pilot_desired_lean_angles(target_roll, target_pitch, copter.aparm.angle_max, copter.aparm.angle_max); // from mode.cpp file angle in centi-degree

    // get pilot's desired yaw rate
    // float target_yaw_rate = get_pilot_desired_yaw_rate(channel_yaw->norm_input_dz()); // from mode.cpp file angle in centi-degree per sec

    if (!motors->armed()) {
        // Motors should be Stopped
        motors->set_desired_spool_state(AP_Motors::DesiredSpoolState::SHUT_DOWN);  // AP_Motors class.cpp libraries
    } else if (copter.ap.throttle_zero) {
        // Attempting to Land
        motors->set_desired_spool_state(AP_Motors::DesiredSpoolState::GROUND_IDLE);
    } else {
        motors->set_desired_spool_state(AP_Motors::DesiredSpoolState::THROTTLE_UNLIMITED);
    }

    switch (motors->get_spool_state()) {
    case AP_Motors::SpoolState::SHUT_DOWN:
        // Motors Stopped
        attitude_control->reset_yaw_target_and_rate();
        attitude_control->reset_rate_controller_I_terms();
        break;

    case AP_Motors::SpoolState::GROUND_IDLE:
        // Landed
        attitude_control->reset_yaw_target_and_rate();
        attitude_control->reset_rate_controller_I_terms_smoothly();
        break;

    case AP_Motors::SpoolState::THROTTLE_UNLIMITED:
        // clear landing flag above zero throttle
        if (!motors->limit.throttle_lower) {
            set_land_complete(false);
            // copter.motors->rc_write(1, 2000);
        }
        break;

    case AP_Motors::SpoolState::SPOOLING_UP:
    case AP_Motors::SpoolState::SPOOLING_DOWN:
        // do nothing
        break;
    }


















    // apply simple mode transform to pilot input
    channel_arming();
    battery_check();
    pilot_input();
    imu_read();

    if (RC_Channels::get_radio_in(CH_6) > 1600){
    custom_pwm_code();
    custom_PID_controller(H_roll, H_pitch, H_yaw, H_roll_dot ,H_pitch_dot, 0.0, 0.0 ,0.0);
    // custom_PD_controller(H_roll, H_pitch, H_yaw, 0.0 , 0.0, 0.0, 0.0 ,0.0);
    // thrust_measurement_code();
    }
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
