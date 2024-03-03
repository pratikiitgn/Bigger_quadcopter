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

#include <uORB/uORB.h>
#include <uORB/topics/vehicle_acceleration.h>
#include <uORB/topics/vehicle_attitude.h>

#include <uORB/topics/cam_device.h>
#include <uORB/topics/pamd_device.h>

__EXPORT int px4_simple_app_main(int argc, char *argv[]);

int px4_simple_app_main(int argc, char *argv[])
{
	PX4_INFO("Hello Sky!");

	/* subscribe to vehicle_acceleration topic */
	int sensor_sub_fd = orb_subscribe(ORB_ID(vehicle_acceleration));
	/* limit the update rate to 5 Hz */
	orb_set_interval(sensor_sub_fd, 200);

	// /* advertise attitude topic */
	struct vehicle_attitude_s att;
	memset(&att, 0, sizeof(att));
	orb_advert_t att_pub = orb_advertise(ORB_ID(vehicle_attitude), &att);

/////////////////////////////////////////////////////
	/* advertise cable attitude topic */
	struct cam_device_s cam_data;
	memset(&cam_data, 0, sizeof(cam_data));
	orb_advert_t cam_data_pub = orb_advertise(ORB_ID(cam_device), &cam_data);

	/* advertise payload attitude topic */
	struct pamd_device_s pamd_data;
	memset(&pamd_data, 0, sizeof(pamd_data));
	orb_advert_t pamd_data_pub = orb_advertise(ORB_ID(pamd_device), &pamd_data);
/////////////////////////////////////////////////////

	while (true) {

		uint64_t timestamp_us = hrt_absolute_time();

		struct vehicle_acceleration_s accel;
		/* copy sensors raw data into local buffer */
		orb_copy(ORB_ID(vehicle_acceleration), sensor_sub_fd, &accel);
		PX4_INFO("Accelerometer: - >Pratik (1) \t%3.2f\t%3.2f\t%3.2f",
				(double)accel.xyz[0],
				(double)accel.xyz[1],
				(double)accel.xyz[2]);

		/* set att and publish this information for other apps
			the following does not have any meaning, it's just an example
		*/
		att.q[0] = accel.xyz[0];
		att.q[1] = accel.xyz[1];
		att.q[2] = accel.xyz[2];
		att.timestamp = timestamp_us;
		orb_publish(ORB_ID(vehicle_attitude), att_pub, &att);

/////////////////////////////////////////////////////
		cam_data.phi 	= accel.xyz[0];
		cam_data.theta 	= accel.xyz[1];
		cam_data.timestamp = timestamp_us;
		orb_publish(ORB_ID(cam_device), cam_data_pub, &cam_data);

		pamd_data.qp1 	= 4.4;
		pamd_data.qp2 	= 5.5;
		pamd_data.qp3 	= 8.8;
		pamd_data.timestamp = timestamp_us;
		orb_publish(ORB_ID(pamd_device), pamd_data_pub, &pamd_data);

		px4_usleep(10000);
/////////////////////////////////////////////////////

	}

	// PX4_INFO("exiting");

	return 0;
}
