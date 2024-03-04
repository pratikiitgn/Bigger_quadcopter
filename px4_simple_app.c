/****************************************************************************
 *
 *   Copyright (c) 2012-2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file px4_simple_app.c
 * Minimal application example for PX4 autopilot
 *
 * @author Example User <mail@example.com>
 */

#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

#include <uORB/uORB.h>
#include <uORB/topics/vehicle_acceleration.h>
#include <uORB/topics/vehicle_attitude.h>

#include <uORB/topics/cam_device.h>
#include <uORB/topics/pamd_device.h>
// #include "common.h"
#include <termios.h>
#include <systemlib/err.h>
#include <drivers/drv_hrt.h>
// #include "sPort_data.h"
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/getopt.h>

// char attitude[]         = "50000_50000";
char phi_char[]    	= "50000";
char theta_char[]  	= "50000";
char startChar[]	= "11";
char endChar[]		= "11";

__EXPORT int px4_simple_app_main(int argc, char *argv[]);

int px4_simple_app_main(int argc, char *argv[])
{

//////////////////////////// For UART communication  ////////////////////////////
	// char *uart_name = "/dev/ttyS1"; // S1 = TELEM2
	char *uart_PAMD = "/dev/ttyS5"; // S5 = GPS2

	// baudRate 	= "115200";

	// int test_uart = open(uart_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
	int test_uart_PAMD = open(uart_PAMD, O_RDWR | O_NOCTTY | O_NONBLOCK);

	// if (test_uart < 0) {
	// 	PX4_ERR("Error opening port: ");
	// 	return -1;
	// }else{
	// 	PX4_INFO("Wooohoo");
	// }

	if (test_uart_PAMD < 0) {
		PX4_ERR("Error opening port: ");
		return -1;
	}else{
		PX4_INFO("Wooohoo");
	}

	// struct termios uart2_config;
	// int termios_state = 0;
	// // int ret = 0;
	// if ((termios_state = tcgetattr(test_uart, &uart2_config)) < 0) {
	// printf("ERROR getting termios config for UART2: %d\n", termios_state);
	// // ret = termios_state;
	// }
	// if (cfsetispeed(&uart2_config, B115200) < 0 ||
	// cfsetospeed(&uart2_config, B115200) < 0) {
	// printf("ERROR setting termios config for UART2: %d\n", termios_state);
	// }
	// // uart2_config.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
	// if ((termios_state = tcsetattr(test_uart, TCSANOW, &uart2_config)) < 0) {
	// printf("ERROR setting termios config for UART2\n");
	// }

	// fcntl(test_uart, F_SETFL, 0);
	// uart2_config.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
	// uart2_config.c_iflag = IGNPAR | ICRNL;
	// tcflush(test_uart, TCIFLUSH);
	// tcsetattr(test_uart, TCSANOW, &uart2_config);



	struct termios uart2_config_PAMD;
	int termios_state_PAMD = 0;
	// int ret = 0;
	if ((termios_state_PAMD = tcgetattr(test_uart_PAMD, &uart2_config_PAMD)) < 0) {
	printf("ERROR getting termios config for UART2: %d\n", termios_state_PAMD);
	// ret = termios_state;
	}
	if (cfsetispeed(&uart2_config_PAMD, B2000000) < 0 ||
	cfsetospeed(&uart2_config_PAMD, B2000000) < 0) {
	printf("ERROR setting termios config for UART2: %d\n", termios_state_PAMD);
	}
	// uart2_config.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
	if ((termios_state_PAMD = tcsetattr(test_uart_PAMD, TCSANOW, &uart2_config_PAMD)) < 0) {
	printf("ERROR setting termios config for UART2\n");
	}

	fcntl(test_uart_PAMD, F_SETFL, 0);
	uart2_config_PAMD.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
	uart2_config_PAMD.c_iflag = IGNPAR | ICRNL;
	tcflush(test_uart_PAMD, TCIFLUSH);
	tcsetattr(test_uart_PAMD, TCSANOW, &uart2_config_PAMD);







	// int readval 	= 0;
	// int count 	= 0;
	// char readbuf[14];
	char readbuf_PAMD[23];

	struct cam_device_s cam_data;
	memset(&cam_data, 0, sizeof(cam_data));
	orb_advert_t cam_data_pub = orb_advertise(ORB_ID(cam_device), &cam_data);

	/* advertise payload attitude topic */
	struct pamd_device_s pamd_data;
	memset(&pamd_data, 0, sizeof(pamd_data));
	orb_advert_t pamd_data_pub = orb_advertise(ORB_ID(pamd_device), &pamd_data);

	// hal.console->printf("serial data -> %c\n",hal.serial(4)->read());
while (true) {

	// do {
		// usleep(1);
		// px4_usleep(1000);
		// int nbytes =
		// read(test_uart, &readbuf, sizeof(readbuf));
		read(test_uart_PAMD, &readbuf_PAMD, sizeof(readbuf_PAMD));

		// PX4_INFO("Attitude data -> %d ", nbytes);
		// PX4_INFO("%s ", readbuf);
		PX4_INFO("%s ", readbuf_PAMD);

				// if (temp != endChar)
			// 		{
			// 			attitude[index] = temp;
			// 			index++;
			// 		}

		// 	startChar[0] 	= *&readbuf[0];
		// 	endChar[0] 	= *&readbuf[12];

		// 	if (startChar[0] == ',' && endChar[0] == '/')
		// 	{
		// 		for (int i= 1 ; i<6 ; i++)
		// 		{
		// 			// PX4_INFO("%c ",
		// 			phi_char[i-1] 	= *&readbuf[i];
		// 			theta_char[i-1] = *&readbuf[i+6];
		// 		}
		// 	}

		// 	// PX4_INFO("%s", phi_char);
		// 	// PX4_INFO("%s", theta_char);

		// int encoder_roll_int      = atoi(phi_char);
		// int encoder_pitch_int     = atoi(theta_char);

		// double encoder_roll_feedback  = (double)((encoder_roll_int  - 50000.0) / 100.0);
		// double encoder_pitch_feedback = (double)((encoder_pitch_int - 50000.0) / 100.0);

		// if (encoder_roll_feedback > 60.0){
		// 	encoder_roll_feedback = 60.0;
		// }
		// if (encoder_roll_feedback < -60.0){
		// 	encoder_roll_feedback = -60.0;
		// }

		// if (encoder_pitch_feedback > 60.0){
		// 	encoder_pitch_feedback = 60.0;
		// }
		// if (encoder_pitch_feedback < -60.0){
		// 	encoder_pitch_feedback = -60.0;
		// }

		// PX4_INFO("%3.2f, %3.2f", encoder_roll_feedback, encoder_pitch_feedback);

		// count++;

		uint64_t timestamp_us = hrt_absolute_time();

		// struct vehicle_acceleration_s accel;
		/* copy sensors raw data into local buffer */
		// orb_copy(ORB_ID(vehicle_acceleration), sensor_sub_fd, &accel);
		// PX4_INFO("Accelerometer: - >Pratik (1) \t%3.2f\t%3.2f\t%3.2f",
		// 		(double)accel.xyz[0],
		// 		(double)accel.xyz[1],
		// 		(double)accel.xyz[2]);

		/* set att and publish this information for other apps
			the following does not have any meaning, it's just an example
		*/
		// att.q[0] = accel.xyz[0];
		// att.q[1] = accel.xyz[1];
		// att.q[2] = accel.xyz[2];
		// att.timestamp = timestamp_us;
		// orb_publish(ORB_ID(vehicle_attitude), att_pub, &att);

/////////////////////////////////////////////////////
		cam_data.phi 	= 0.0;
		cam_data.theta 	= 1.1;
		cam_data.timestamp = timestamp_us;
		orb_publish(ORB_ID(cam_device), cam_data_pub, &cam_data);

		pamd_data.qp1 	= 4.4;
		pamd_data.qp2 	= 5.5;
		pamd_data.qp3 	= 8.8;
		pamd_data.timestamp = timestamp_us;
		orb_publish(ORB_ID(pamd_device), pamd_data_pub, &pamd_data);

		px4_usleep(10000);
}
/////////////////////////////////////////////////////

	// }
	// while (count < 500);

	// close(test_uart);
	close(test_uart_PAMD);

	// PX4_INFO("Accelerometer: - >Pratik (1) \t%3.2f\t%3.2f\t%3.2f",
		// 		(double)accel.xyz[0],
		// 		(double)accel.xyz[1],
		// 		(double)accel.xyz[2]);

	/* Open UART */
	// struct termios uart_config_original;
	// struct termios uart_config;
	// const int uart = sPort_open_uart(device_name, &uart_config, &uart_config_original);

//////////////////////////// For UART communication  ////////////////////////////


	// // PX4_INFO("Hello Sky!");

	// /* subscribe to vehicle_acceleration topic */
	// int sensor_sub_fd = orb_subscribe(ORB_ID(vehicle_acceleration));
	// /* limit the update rate to 5 Hz */
	// orb_set_interval(sensor_sub_fd, 200);

	// // /* advertise attitude topic */
	// struct vehicle_attitude_s att;
	// memset(&att, 0, sizeof(att));
	// orb_advert_t att_pub = orb_advertise(ORB_ID(vehicle_attitude), &att);

/////////////////////////////////////////////////////
	/* advertise cable attitude topic */

/////////////////////////////////////////////////////

	// PX4_INFO("exiting");

	return 0;
}
