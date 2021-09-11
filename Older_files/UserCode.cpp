#include "Copter.h"
#include <AP_HAL/AP_HAL.h>
#include <AP_AHRS/AP_AHRS.h>
#include <AP_Logger/LogStructure.h>
#include <AP_Logger/AP_Logger.h>  
#include <stdlib.h>
#include <string.h>
#include "UserCode.h"
#include <math.h>
#include <AP_GPS/AP_GPS.h>
// #include "ardupilot/Tracker.h"

#if HAL_OS_POSIX_IO
#include <stdio.h>
#endif


int temp;

#define PI 3.14159265

int encoder_control_roll_int = 0;
int encoder_control_pitch_int = 0;

float CAC_roll = 0.0;
float CAC_pitch = 0.0;
 
float H_roll = 0.0;
float H_pitch = 0.0;
float H_yaw = 0.0;
float H_throttle = 0.0;

float encoder_roll_feedback = 0.0;
float encoder_pitch_feedback = 0.0;

float encoder_control_roll_input  = 0.0;
float encoder_control_pitch_input = 0.0;

float kp_theta = 0.0;
float kd_theta = 0.0;
float kp_phi = 0.0;
float kd_phi = 0.0;

float kp_x = 0.0;
float kp_y = 0.0;
float kp_z = 0.0;
float kd_x = 0.0;
float kd_y = 0.0;
float kd_z = 0.0;

float Z_des = 0.0;

float roll_in_inertial_frame = 0.0;
float pitch_in_inertial_frame = 0.0;

float last_roll_in_inertial_frame = 0.0;
float last_pitch_in_inertial_frame = 0.0;

float phi_dot = 0.0;
float theta_dot = 0.0;

// For cable attitude
float q1 = 0.0;
float q2 = 0.0;

float q1_dot = 0.0;
float q2_dot = 0.0;

float q1_old = 0.0;
float q2_old = 0.0;
// string test_;

int flag1 = 0 ;
int flag2 = 0 ;
int flag3 = 0 ;

float q_roll  = 0.0;
float q_pitch = 0.0;
float q_yaw   = 0.0;

// For GPS data
float latitude  = 0.0;
float longitude = 0.0;
float altitude  = 0.0;

float quad_x  = 0.0;
float quad_y  = 0.0;
float quad_z  = 0.0;

float quad_z_vel = 0.0;

float altitude_1 = 0.0;
float altitude_2 = 0.0;
float altitude_3 = 0.0;
float altitude_4 = 0.0;
float altitude_5 = 0.0;

float altitude_fil = 0.0;

// For logging Human commands
float Human_phi         = 0.0;
float Human_theta       = 0.0;
float Human_yaw_rate    = 0.0;
float Human_throttle    = 0.0;


// For checking the mode on off
float control_mode_on_off = 0.0;

#ifdef USERHOOK_INIT
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
    hal.uartC->begin(38400);
    // hal.uartA->begin(57600);
    // hal.uartD->begin(38400);
    hal.uartE->begin(115200);
    // hal.uartE->begin(9600);
    // hal.uartE->begin(115200);
    // hal.gpio->attach_interrupt(54,enc1_phaseApin_isr,AP_HAL::GPIO::INTERRUPT_RISING);

    // hal.uartA->printf("Init_completed\n");
    // hal.uartC->printf("Init_completed\n");
    // hal.uartD->printf("Init_completed\n");
    // AP::logger().Write("TEST", "TimeUS",
    //                "sm", // units: seconds, meters
    //                "FB", // mult: 1e-6, 1e-2
    //                "Qf", // format: uint64_t, float
    //             12);
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here

    // fetch human roll and pitch inputs in terms of degree

    H_roll      = (channel_roll->get_control_in())/100.0;
    H_pitch     = (channel_pitch->get_control_in())/100.0;
    H_yaw       = (channel_yaw->get_control_in()) / 100.0;
    H_throttle  = (double)channel_throttle->get_control_in();

    const struct Location &loc_ = gps.location();
    
    // AP_Baro &baro = AP::baro();
    latitude    = (double)((loc_.lat)*1.0e-7f);
    longitude   = (double)((loc_.lng)*1.0e-7f);

    //// Filtering the altitude of the quadcopter

    altitude    = (double)((baro_alt)*1.0e-2f);
    altitude_1  = altitude;
    altitude_2  = altitude_1;
    altitude_3  = altitude_2;
    altitude_4  = altitude_3;
    altitude_5  = altitude_4;

    float fil_c   = 1/6.0;
    float fil_c_1 = 1/6.0;
    float fil_c_2 = 1/6.0;
    float fil_c_3 = 1/6.0;
    float fil_c_4 = 1/6.0;
    float fil_c_5 = 1/6.0;

    altitude_fil = (fil_c*altitude + fil_c_1*altitude_1 + fil_c_2 * altitude_2 + fil_c_3*altitude_3+fil_c_4 * altitude_4 + fil_c_5*altitude_5);

    // const AP_GPS &gps_user_define = AP::gps();

    // hal.uartC->printf("E");
    // hal.uartC->printf("%0.5f",(double)(error_X_des));
    // hal.uartC->printf(",");
    // hal.uartC->printf("%0.5f",(double)(error_Y_des));
    // hal.uartC->printf(",");
    // hal.uartC->printf("%0.5f",(double)(error_Z_des));
    // // hal.uartC->printf(",");
    // // hal.uartC->printf("%0.2f",(double)(variable_x_velocity));
    // // hal.uartC->printf(",");
    // // hal.uartC->printf("%0.2f",(double)(variable_y_velocity));
    // // hal.uartC->printf(",");
    // // hal.uartC->printf("%0.2f",(double)(variable_z_velocity));
    // // hal.uartC->printf(",");
    // // hal.uartC->printf("%0.2f",(double)(variable_quad_yaw));
    // // hal.uartC->printf(",");
    // // hal.uartC->printf("%0.5f",(double)(inertial_nav.get_velocity_z()));
    // // hal.uartC->printf("%f",(double)(kp_z));
    // // hal.uartC->printf(",");
    // // hal.uartC->printf("%f",(double)(kd_z));
    // // hal.uartC->printf(",");
    // // hal.uartC->printf("%f",(double)(Z_des));
    // hal.uartC->printf("\n");


    // (double)((loc_.alt)*1.0e-2f);

    // To change the MODE of the CAC controller

    // CAC_geometric();
    CAC_Euler_angle();
    switching_type_mode();
    debug_code_function();

    GPS_coordinate_transform();
    // To send it in stabilize mode
    CAC_roll    = encoder_control_roll_input;
    CAC_pitch   = encoder_control_pitch_input;

    getEncoderData();
    Log_Write_Test();
    Log_Write_Test_2();
    Log_Write_Test_3();
    Log_Write_Test_4();
    

}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
    // hal.uartD->printf("Yaw %.3f\n", (double)(ahrs.yaw_sensor));
    //hal.uartD->printf("%.1f\n", (double)(ahrs.yaw_sensor));
    // hal.uartD->printf("%.3f,%.3f,%.3f\n", (double)(ahrs.pitch_sensor),(double)(ahrs.roll_sensor),(double)(ahrs.yaw_sensor));
    //double arm = bool AP_Arming::disarm()
    //hal.uartD->printf("Arm ? %.3\n", (double)(arm));

    // int lenb = hal.uartD->available();
    // hal.uartD->printf("%d\n",(int)lenb);
    // hal.uartC->printf("E");
    // hal.uartC->printf("%f",(double)(error_X_des));
    // hal.uartC->printf(",");
    // hal.uartC->printf("%f",(double)(variable_x_velocity));
    // hal.uartC->printf(",");
    // hal.uartC->printf("%f",(double)(variable_z_velocity));
    // hal.uartC->printf("\n");
    // hal.uartC->printf("E");
    // hal.uartC->printf("%f",(double)(kp_theta));
    // hal.uartC->printf(",");
    // hal.uartC->printf("%f",(double)(kd_theta));
    // hal.uartC->printf(",");
    // hal.uartC->printf("%f",(double)(error_X_des));
    // hal.uartC->printf(",");
    // hal.uartC->printf("%f",(double)(error_Y_des));
    // hal.uartC->printf("\n");


}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
    // temp = hal.uartD->read();
    // static int pos;
    // attitude[pos++] = temp;
    // if (pos>=11)
    // {pos = 0;}
    // hal.uartD->printf(attitude);
    //  hal.uartD->printf("A \n");
    // getEncoderData();
    // hal.uartC->printf("Pratik\n");

    
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
        
    // hal.uartE->printf("_");
    // hal.uartE->printf("784");
    // hal.uartE->printf("_");
    // hal.uartE->printf("986");
    // hal.uartE->printf("/");
    // hal.uartC->printf("\n");
    // put your 1Hz code here
    // if (copter.motors->armed()) {
        // hal.uartD->printf("Yaw %.3f \n", (double)(ahrs.yaw_sensor));
    // }else
    // {
    //     hal.uartD->printf("max value - %d \n", channel_roll->get_control_in());
    // }
    // if (copter.motors->disarmed()){
        // hal.uartD->printf("Yaw -> in disarm mode %.3f \n", (double)(ahrs.yaw_sensor));
    // }

    // abcd = hal.uartD->read();
    // hal.uartA->printf(" From UART A \n");
    // hal.uartD->printf(" From UART D \n");
    // abcd = hal.uartD->read();
    // hal.uartD->printf("%d\n",(abcd));
    // hal.uartE->printf("%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f\n",(double)(encoder_roll_feedback),(double)(encoder_pitch_feedback),(double)(kp_phi),(double)(kd_phi),(double)(kd_theta),(double)(kd_theta),(double)(kp_x),(double)(kp_y),(double)(kp_z),(double)(kd_x),(double)(kd_y),(double)(kd_z),(double)(Z_des));
}
#endif

#ifdef USERHOOK_AUXSWITCH
void Copter::userhook_auxSwitch1(uint8_t ch_flag)
{
    // put your aux switch #1 handler here (CHx_OPT = 47)
}

void Copter::userhook_auxSwitch2(uint8_t ch_flag)
{
    // put your aux switch #2 handler here (CHx_OPT = 48)
}

void Copter::userhook_auxSwitch3(uint8_t ch_flag)
{
    // put your aux switch #3 handler here (CHx_OPT = 49)
}
#endif

void Copter::GPS_coordinate_transform(){

    // Constant parameter based on earth's shape
    float a_gps = 6378137.0;
    // float f_gps = 1/298.257223563;
    // float b_gps = a_gps*(1-f_gps);
    float e_gps = sqrtf(6.6943799014/1000.0);

    // Reference ECEF location

    // float lamda_r = 72.685987;
    // float phi_r   = 23.213985;
    // float altitude_r = 100.0;
    // float N_r     = a_gps/(sqrtf(1.0 - powf(e_gps,2) * powf(sinf(phi_r * PI/180),2)));
    
    // float ECEF_x_r= (N_r + altitude_r) * cosf(phi_r * PI/180) * cosf(lamda_r * PI/180);
    // float ECEF_y_r= (N_r + altitude_r) * cosf(phi_r * PI/180) * sinf(lamda_r * PI/180);
    // float ECEF_z_r= (N_r * (1.0 - powf(e_gps,2)) + altitude_r) * sinf(phi_r * PI/180);

    float ECEF_x_r= 0.0;
    float ECEF_y_r= 0.0;
    float ECEF_z_r= 0.0;

    // Current ECEF location

    // float lamda_r = 72.685987;
    // float phi_r   = 23.213985;
    // float altitude_r = 100.0;
    float N_C     = a_gps/(sqrtf(1.0 - powf(e_gps,2) * powf(sinf(latitude * PI/180),2)));
    
    float ECEF_x  = ((N_C + altitude) * cosf(latitude * PI/180) * cosf(longitude * PI/180)) - ECEF_x_r ;
    float ECEF_y  = ((N_C + altitude) * cosf(latitude * PI/180) * sinf(longitude * PI/180)) - ECEF_y_r;
    float ECEF_z  = ((N_C * (1.0 - powf(e_gps,2)) + altitude) * sinf(latitude * PI/180)) - ECEF_z_r;

    float ENU_x   = -sinf(longitude)*ECEF_x + cosf(longitude) * ECEF_y;
    float ENU_y   = -cosf(longitude)*sinf(latitude)*ECEF_x  - sinf(longitude)*sinf(latitude)*ECEF_y + cosf(latitude) * ECEF_z;
    // float ENU_z   =  cosf(latitude)*cosf(longitude)*ECEF_x + sinf(longitude)*cosf(latitude)*ECEF_y + sinf(latitude)*ECEF_z;
    float ENU_z = altitude;
    // float GPS_sensor_offset_x =  0.0;
    // float GPS_sensor_offset_y = -15.0;
    // float GPS_sensor_offset_z =   0.0;

    // quad_x = -ENU_x + GPS_sensor_offset_x;
    // quad_y =  ENU_y + GPS_sensor_offset_y;
    // quad_z =  ENU_z + GPS_sensor_offset_z;

    quad_x = -ENU_x ;
    quad_y =  ENU_y ;
    quad_z =  ENU_z ;

}

void Copter::debug_code_function(){

    // hal.uartE->printf("%0.3f\n",(double)(H_pitch_mimic));
    // hal.uartE->printf("%0.3f,%0.3f\n",(double)(encoder_roll_feedback),(double)(encoder_pitch_feedback));
    // hal.uartE->printf("%0.3f\n",(double)(q_yaw));
    // hal.uartE->printf("%0.3f,%0.3f,%0.3f\n",(double)(quad_x),(double)(quad_y),(double)(quad_z));
    // hal.uartE->printf("%0.3f\n",(double)(baro_alt));
    // hal.uartE->printf("%0.3f,%0.3f,%0.3f,%0.3f\n",(double)(kp_phi),(double)(kd_phi),(double)(kp_theta),(double)(kd_theta));
    // Code for the CAM device calbration with hand movement
    // hal.uartE->printf("%0.3f\n",(double)(H_pitch_mimic));
    // char roll_[]        = "11111";
    // char pitch_[]       = "11111";
    // char yaw_[]         = "11111";
    // char q_x []         = "11111";
    // char q_y []         = "11111";
    // char q_z []         = "11111";
 
    // int q_roll_int      = (int)(100*q_roll  + 50000);
    // int q_pitch_int     = (int)(100*q_pitch + 50000);
    // int q_yaw_int       = (int)(100*q_yaw   + 50000);
    // int q_x_int         = (int)(100*quad_x  + 50000);
    // int q_y_int         = (int)(100*quad_y  + 50000);
    // int q_z_int         = (int)(100*quad_z  + 50000);

    // itoa(q_roll_int,roll_,10);
    // itoa(q_pitch_int,pitch_,10);
    // itoa(q_yaw_int,yaw_,10);
    // itoa(q_baro_alt_int,baro_alt_,10);

    // hal.uartE->printf(",");
    // hal.uartE->printf(roll_);
    // hal.uartE->printf("_");
    // hal.uartE->printf(pitch_);
    // hal.uartE->printf("_");
    // hal.uartE->printf(yaw_);
    // hal.uartE->printf("_");
    // hal.uartE->printf(baro_alt_);
    // hal.uartE->printf("/");
    // hal.uartE->printf("\n");
        

    // Convert the position from meters to centimeters

    // error_X_des = error_X_des;
    // error_Y_des = error_Y_des;
    // error_Z_des = error_Z_des;



    // hal.uartC->printf(",");
    // hal.uartC->printf("P");
    // hal.uartC->printf("_");
    // hal.uartC->printf("784");
    // hal.uartC->printf("_");
    // hal.uartC->printf("986");
    // hal.uartC->printf("/");
    // hal.uartC->printf("\n");

    // hal.uartC->printf("Pratik\n");

}

void Copter::Log_Write_Test()
{
    struct log_Test pkt = {
        LOG_PACKET_HEADER_INIT(LOG_TEST_MSG),
        time_us     : AP_HAL::micros64(),
        p_roll      : control_roll_input_final,
        p_pitch     : control_pitch_input_final,
        en_r        : encoder_roll_feedback,
        en_p        : encoder_pitch_feedback,
        q_r         : q_roll,
        q_p         : q_pitch,
        q_y         : q_yaw,
        h_r         : H_roll,
        h_p         : H_pitch,
        h_t         : H_throttle,
        o_o         : ch5_in,
        };
    logger.WriteBlock(&pkt, sizeof(pkt));
}

void Copter::Log_Write_Test_2()
{
    struct log_Test_2 pkt = {
        LOG_PACKET_HEADER_INIT(LOG_TEST_MSG_2),
        time_us     : AP_HAL::micros64(),
        alt         : altitude,
        lat         : latitude,
        longi       : longitude,
        quad_xx     : error_X_des,
        quad_yy     : error_Y_des,
        quad_zz     : error_Z_des,
        H_phi       : control_roll_input_final,
        H_theta     : control_pitch_input_final,
        h_c_Th      : controlled_throttle,
        h_Th        : human_commands_throttle,
        };
    logger.WriteBlock(&pkt, sizeof(pkt));
}

void Copter::Log_Write_Test_3()
{
    struct log_Test_3 pkt = {
        LOG_PACKET_HEADER_INIT(LOG_TEST_MSG_3),
        time_us     : AP_HAL::micros64(),
        Q_X         : error_X_des,
        Q_Y         : error_Y_des,
        Q_Z         : error_Z_des,
        Q_V_X       : quad_fil_vel_x,
        Q_V_Y       : quad_fil_vel_y,
        Q_V_Z       : variable_z_velocity,
        Q_yaw       : variable_quad_yaw,
        Q_lat       : variable_latitude,
        Q_lon       : variable_longitude,
    };
    logger.WriteBlock(&pkt, sizeof(pkt));
}

void Copter::Log_Write_Test_4(){
    // Human_phi = 55;
    struct log_Test_4 pkt = {
        LOG_PACKET_HEADER_INIT(LOG_TEST_MSG_4),
        time_us     : AP_HAL::micros64(),
        H_phi       : Human_phi,
        H_theta     : Human_theta,
        H_yaw_rate  : Human_yaw_rate,
        H_throttle  : Human_throttle,
        Mode_        : ch5_in,
    };
    logger.WriteBlock(&pkt, sizeof(pkt));
}

void Copter::getEncoderData()
{
    // int ind = 0;
    // int bufflen = hal.uartE->available();
    // for(int i = bufflen - 1; i >= 0; i--)
    // {
    //     attitude[ind] = hal.uartE->read();
    // }
    // hal.uartE->printf("%d\n",int(bufflen));

    bool receiving_data = false;
    int index = 0;
    char startChar = ',';
    char endChar = '/';
    // char temp;
    bool new_data = false;
    // ,55555_55555_55555_55555_55555_55555_55555_55555_55555_55555_55555_55555_55555/
    // 65+14+1 = 80

    char attitude[] = "50000_50000_500_500_500_500_500_500_500_500_500_500_500_500";
    while (hal.uartE->available()>0 && new_data == false)
        {
            temp = hal.uartE->read();
            if (receiving_data == true)
            {
                if (temp != endChar)
                {   
                    attitude[index] = temp;
                    index++;
                }
                else
                {
                    attitude[index] = '\0';
                    receiving_data = false;
                    new_data = false;
                    index = 0;
                    flag1 = 0;
                    flag2 = 0;
                    flag3 = 0;
                }
            }
            else if (temp == startChar)
            {
                receiving_data = true;
                index = 0; 
            }
        }
        // hal.uartE->printf("Attitude->");
        // hal.uartE->printf(attitude);
        

        // char attitude_split [sizeof(attitude)] = {'0','0','0','0','0','0','0','0','0','0'};
        // strcpy(attitude_split,attitude);
        // char * pch;
        // pch = strtok(attitude_split,"_");
        // while (pch != NULL)
        // {
        //     pch = strtok(NULL,"_");
        // }
        
        // for (int j = 0; j++ ; j < 3){
        //     char roll[4] = {'0','0','0','0'};
        //     roll [j] = attitude[j];
        // }
        // pitch_char[5] = '\0';
        
        // pitch_char[6] = '\0';
        // strcpy(attitude, "5555555555555555555555555555555555555555555555555555555555555555555555555555555");
        
        // hal.uartE->printf(attitude);
        // hal.uartE->printf('\n');

        char roll_char[]        = "11111";
        char pitch_char[]       = "11111";

        char kp_theta_char[]    = "111";
        char kd_theta_char[]    = "111";
        char kp_phi_char[]      = "111";
        char kd_phi_char[]      = "111";

        char kp_x_char[]        = "111";
        char kp_y_char[]        = "111";
        char kp_z_char[]        = "111";
        char kd_x_char[]        = "111";
        char kd_y_char[]        = "111";
        char kd_z_char[]        = "111";

        char Z_des_char[]       = "111";

        // char control_roll_char[] = "11111";
        // char control_pitch_char[] = "11111";

        // hal.uartE->printf("Attitude->");
        // hal.uartE->printf(attitude);
        // hal.uartE->printf("____\n");
        int actual_size = 0;
        for (int i = 0; i < 55 ; i++)
        {
            if (attitude[i] =='\0'){
                
            } else
            {
                actual_size = actual_size + 1;
            }
        }

        // if(actual_size == 55){ 

        for (int i = 0; i < 55; i++)
        {
            if (i < 5)  
            {
                roll_char[i]                = attitude[i];
            } else if (i >= 6 && i < 11 )
            {
                pitch_char[i - 6]           = attitude[i];
            } else if (i >= 12 && i < 15 )
            {
                kp_phi_char[i - 12]         = attitude[i];
            } else if (i >= 16 && i < 19)
            {
                kd_phi_char[i - 16]         = attitude[i];
            } else if (i >= 20 && i < 23 )
            {
                kp_theta_char[i - 20]       = attitude[i];
            } else if (i >= 24 && i < 27)
            {
                kd_theta_char[i - 24]       = attitude[i];
            } else if (i >= 28 && i < 31)
            {
                kp_z_char[i - 28]           = attitude[i];
            } else if (i >= 32 && i < 35)
            {
                kd_z_char[i - 32]           = attitude[i];
            } else if (i >= 36 && i < 39)
            {
                Z_des_char[i - 36]           = attitude[i];
            } else if (i >= 40 && i < 43)
            {
                kp_x_char[i - 40]           = attitude[i];
            } else if (i >= 44 && i < 47)
            {
                kd_x_char[i - 44]           = attitude[i];
            } else if (i >= 48 && i < 51)
            {
                kp_y_char[i - 48]           = attitude[i];
            } else if (i >= 52 && i < 55)
            {
                kd_y_char[i - 52]           = attitude[i];
            }
        }

        // hal.uartE->printf(roll_char);
        // hal.uartE->printf(",");
        // hal.uartE->printf(pitch_char);
        // hal.uartE->printf(",");
        // hal.uartE->printf(control_roll_char);
        // hal.uartE->printf(",");
        // hal.uartE->printf(control_pitch_char);
        // hal.uartE->printf("\n");
        // hal.uartE->printf("Pitch-> ");
        // hal.uartE->printf(pitch_char);
        // hal.uartE->printf("  Roll->");
        // hal.uartE->printf(roll_char);
        
        // hal.uartE->printf("---%d,%d",(int)sizeof(pitch_char),(int)sizeof(roll_char));
        int encoder_roll_int  = 0;
        int encoder_pitch_int = 0;

        encoder_roll_int      = atoi(roll_char);
        encoder_pitch_int     = atoi(pitch_char);
        
        float kp_theta_int    = atoi(kp_theta_char);
        float kd_theta_int    = atoi(kd_theta_char);
        float kp_phi_int      = atoi(kp_phi_char);
        float kd_phi_int      = atoi(kd_phi_char);
        float kp_x_int        = atoi(kp_x_char);
        float kp_y_int        = atoi(kp_y_char);
        float kp_z_int        = atoi(kp_z_char);
        float kd_x_int        = atoi(kd_x_char);
        float kd_y_int        = atoi(kd_y_char);
        float kd_z_int        = atoi(kd_z_char);

        float Z_des_int       = atoi(Z_des_char);

        // hal.uartE->printf(roll_char,pitch_char,kp_theta_char,kd_theta_char,kp_phi_char,kd_phi_char,kp_x_char,kp_y_char,kp_z_char,kd_x_char,kd_y_char,kd_z_char,Z_des_char);       

        // encoder_control_roll_int = atoi(control_roll_char);
        // encoder_control_pitch_int = atoi(control_pitch_char);
        // hal.uartE->printf("   integer--> %d , %d ",(int)sizeof(encoder_roll_int),(int)sizeof(encoder_pitch_int));
        // hal.uartE->printf("   integer--> %d , %d ",(int)(encoder_roll_int),(int)(encoder_pitch_int));

        encoder_roll_feedback  = (float)((encoder_roll_int  - 50000.0) / 100.0);
        encoder_pitch_feedback = (float)((encoder_pitch_int - 50000.0) / 100.0);

        // encoder_control_roll_input     = (float)((encoder_control_roll_int  - 50000.0) / 100.0);
        // encoder_control_pitch_input    = (float)((encoder_control_pitch_int  - 50000.0) / 100.0);

        kp_phi      = (float) ((kp_phi_int    - 500.0) / 10.0);
        kd_phi      = (float) ((kd_phi_int    - 500.0) / 10.0);
        kp_theta    = (float) ((kp_theta_int  - 500.0) / 10.0);
        kd_theta    = (float) ((kd_theta_int  - 500.0) / 10.0);

        kp_x        = (float) ((kp_x_int      - 500.0) / 10.0);
        kp_y        = (float) ((kp_y_int      - 500.0) / 10.0);
        kp_z        = (float) ((kp_z_int      - 500.0) / 10.0);
        kd_x        = (float) ((kd_x_int      - 500.0) / 10.0);
        kd_y        = (float) ((kd_y_int      - 500.0) / 10.0);
        kd_z        = (float) ((kd_z_int      - 500.0) / 10.0);

        Z_des       = (float) ((Z_des_int     - 500.0) / 10.0);

        // hal.uartE->printf("Starting__\n");
        // hal.uartE->printf("%0.3f,%0.3f,",(double)(kp_phi),(double)(kd_phi));
        // hal.uartE->printf("%0.3f,%0.3f\n",(double)(kp_theta),(double)(kd_theta));
        // hal.uartE->printf("%0.3f,%0.3f,",(double)(kp_x),(double)(kd_x));
        // hal.uartE->printf("%0.3f,%0.3f,",(double)(kp_y),(double)(kd_y));
        // hal.uartE->printf("%0.3f,%0.3f\n",(double)(kp_z),(double)(kd_z));
        // hal.uartE->printf("%0.3f,",(double)(user_throttle));
        // hal.uartE->printf("%0.1f,%0.1f,",(double)(kp_z),(double)(kd_z));
        // hal.uartE->printf("%0.1f\n",(double)(Z_des));
        
        // hal.uartE->printf("%c",attitude[57]);
        // hal.uartE->printf("%c,%c,%c",attitude[55],attitude[56],attitude[57]);
        // hal.uartE->printf("\n");
        // hal.uartE->printf("%d\n",actual_size);
        // hal.uartE->printf(attitude);

        // quad_z_vel = inertial_nav.get_velocity_z();
        // hal.uartE->printf("%0.3f\n",double(quad_z_vel));

        if (encoder_roll_feedback > 90){
            encoder_roll_feedback = 90;
        }
        if (encoder_roll_feedback < -90){
            encoder_roll_feedback = -90;
        }

        if (encoder_pitch_feedback > 90){
            encoder_pitch_feedback = 90;
        }
        if (encoder_pitch_feedback < -90){
            encoder_pitch_feedback = -90;
        }

        q_roll  =  (ahrs.roll_sensor)  / 100.0;
        q_pitch = -(ahrs.pitch_sensor) / 100.0;
        q_yaw   =  (ahrs.yaw_sensor)   / 100.0;

        roll_in_inertial_frame  = encoder_roll_feedback + q_roll;
        pitch_in_inertial_frame = encoder_pitch_feedback + q_pitch;
        }
        // current_time = (time_us - starting_time)/10000000;

        //Final code for debug
        // hal.uartE->printf("%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f\n",(double)(encoder_roll_feedback),(double)(encoder_pitch_feedback),(double)(q_roll),(double)(q_pitch),(double)(kp_phi),(double)(kd_phi),(double)(kp_theta),(double)(kd_theta));
        // hal.uartE->printf("%0.3f,%0.3f\n",(double)(H_roll),(double)(H_pitch));

        // hal.uartE->printf("%0.3f,%0.3f,%0.3f\n",(double)(encoder_pitch_feedback),(double)(q_pitch),(double)(pitch_in_inertial_frame));
        // hal.uartE->printf("%0.3f,%0.3f,%0.3f\n",(double)(encoder_roll_feedback),(double)(q_roll),(double)(roll_in_inertial_frame));
        // hal.uartE->printf("%0.3f,%0.3f\n",(double)(roll_in_inertial_frame),(double)(pitch_in_inertial_frame));
// }

void Copter::switching_type_mode(){
            // For checking the control mode
        if (RC_Channels::get_radio_in(CH_5) > 1400){
            control_mode_on_off = 1.0;
        }else if (RC_Channels::get_radio_in(CH_5) > 1600){
            control_mode_on_off = 2.0;
        }else{
            control_mode_on_off = 0.0;
        }
}

void Copter::CAC_Euler_angle(){

        phi_dot   = (roll_in_inertial_frame  - last_roll_in_inertial_frame);
        theta_dot = (pitch_in_inertial_frame - last_pitch_in_inertial_frame);

        float des_phi = 0.0;
        float des_theta = 0.0;
        float des_phi_dot = 0.0;
        float des_theta_dot = 0.0;

        // kp_phi      = 0.4;
        // kd_phi      = 0.5;

        kp_theta    = 0.4;
        kd_theta    = 0.0;

        encoder_control_roll_input  = -(0.2)*(des_phi   - roll_in_inertial_frame ) + (1)*(des_phi_dot   - phi_dot);
        encoder_control_pitch_input =  (0.2)*(des_theta - pitch_in_inertial_frame) + (1)*(des_theta_dot - theta_dot);

        last_roll_in_inertial_frame  = roll_in_inertial_frame;
        last_pitch_in_inertial_frame = pitch_in_inertial_frame;

        encoder_control_roll_input   = saturation_fun(encoder_control_roll_input);
        encoder_control_pitch_input  = saturation_fun(encoder_control_pitch_input);

}

// void Copter::CAC_geometric()
// {
//         q1 = (- cosf(q_pitch*PI/180) * sinf(encoder_pitch_feedback*PI/180) - sinf(q_pitch*PI/180) * cosf(encoder_roll_feedback*PI/180) * cosf(encoder_pitch_feedback*PI/180));
//         q2 = (- sinf(encoder_pitch_feedback*PI/180) * sinf(q_roll*PI/180) * sinf(q_pitch)  + cosf(q_roll*PI/180) * sinf(encoder_roll_feedback*PI/180) * cosf(encoder_pitch_feedback*PI/180) + sinf(q_roll*PI/180) * cosf(q_pitch*PI/180) * cosf(encoder_pitch_feedback*PI/180) * cosf(encoder_roll_feedback*PI/180));

//         if (q1 > 1){q1 = 1;}
//         if (q1 < -1){q1 = -1;}

//         if (q2 > 1){q2 = 1;}
//         if (q2 < -1){q2 = -1;}

//         // q1 = sinf(0);
//         // q2 = sinf(PI/4);

//         q1_dot = (q1 - q1_old);
//         q2_dot = (q2 - q2_old);

//         q1_old = q1;
//         q2_old = q2;

//         encoder_control_roll_input  =  (kp_phi * q2  + kd_phi * q2_dot);
//         encoder_control_pitch_input =   kp_theta * q1  + kd_theta * q1_dot;

//         // Saturation
//         if (encoder_control_roll_input > 45){
//             encoder_control_roll_input = 45;
//         }
//         if (encoder_control_roll_input < -45){
//             encoder_control_roll_input = -45;
//         }

//         if (encoder_control_pitch_input > 45){
//             encoder_control_pitch_input = 45;
//         }
//         if (encoder_control_pitch_input < -45){
//             encoder_control_pitch_input = -45;
//         }
// }
float Copter::saturation_fun(float data){
        float output = 0.0;
        float max_lim_cac  =  30.0;
        float min_lim_cac  = -30.0;

        if (data > max_lim_cac) {
            output = max_lim_cac;
        }else if (data < min_lim_cac) {
            output = min_lim_cac;
        }else{
            output = data;
        }

        return output;
}

// int charArrayTointArray ( char [] data , int start , int end ) throws NumberFormatException
