#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist
import math
from vicon_bridge.msg import Markers
import time

global point_A
global point_B
global point_C
global point_D
global theta
global phi

global flag_header
global quad_attached_marker
global payload_marker
global position_vector

global start_time
global current_time

start_time = time.time()
current_time = 0.0

point_A = [ 0.0, 0.0, 0.0 ]
point_B = [ 0.0, 0.0, 0.0 ]
point_C = [ 0.0, 0.0, 0.0 ]
point_D = [ 0.0, 0.0, 0.0 ]

theta = 0.0
phi = 0.0

flag_header = 0.0

quad_attached_marker = [ 0.0, 0.0, 0.0]
payload_marker       = [ 0.0, 0.0, 0.0]
position_vector 	 = [ 0.0, 0.0, 0.0]

def callback(data):
	global point_A
	global point_B
	global point_C
	global point_D
	global theta
	global phi
	global quad_attached_marker
	global flag_header
	global start_time, current_time

	current_time = time.time() - start_time

	L = 660.0
	
	markerX_offset = 0.0
	markerY_offset = 0.0
	markerZ_offset = 50.0

	quad_attached_marker[0] = int(data.markers[4].translation.x) - markerX_offset
	quad_attached_marker[1] = int(data.markers[4].translation.y) - markerY_offset
	quad_attached_marker[2] = int(data.markers[4].translation.z) - markerZ_offset

	payload_marker[0] = int(data.markers[6].translation.x)
	payload_marker[1] = int(data.markers[6].translation.y)
	payload_marker[2] = int(data.markers[6].translation.z)

	position_vector[0] = payload_marker[0] - quad_attached_marker[0]
	position_vector[1] = payload_marker[1] - quad_attached_marker[1]
	position_vector[2] = payload_marker[2] - quad_attached_marker[2]

	# theta_1 = 180/math.pi*math.asin((point_B[0] - point_A[0])/ math.sqrt( (point_B[0] - point_A[0])*(point_B[0] - point_A[0]) + (point_B[1] - point_A[1])*(point_B[1] - point_A[1]) + (point_B[2] - point_A[2])*(point_B[2] - point_A[2]) ) )
	theta = math.asin(-position_vector[0]/L)
	phi = 180/math.pi*math.asin(position_vector[1]/(math.cos(theta)*L))
	theta = 180/math.pi*theta
	# print str(quad_attached_marker) + ' , ' + str(payload_marker)
	# print ("theta = " + str(theta))

	# if flag_header == 0:
	# 	header = 'Time, Vicon_phi, Vicon_theta\n'
	# 	file_name = "static_trial_2.csv"
	# 	data_file = open(file_name, "a")
	# 	data_file.write(header)
	# 	data_file.close()
	# 	flag_header = 1
	# else:
	# 	# header = 'encoder_phi, encoder_theta\n'
	# 	file_name = "hand_trial.csv"
	# 	data_file = open(file_name, "a")
	# 	data_file.write(str(current_time) + "," + str(phi) + ',' + str(theta) + '\n')
	# 	data_file.close()
	

class read_data():
    def __init__(self):
		global theta
		global phi
		rospy.init_node('CAM_device_calibration')
		self.pub = rospy.Publisher('CAM_vicon_comparison', Twist, queue_size=1)

		self.data = Twist()

		rospy.Subscriber('Encoder_feedback', Twist, self.callback)
		rospy.Subscriber("/vicon/markers", Markers, callback)

    def callback(self, msg):
		global flag_header
		global phi
		global theta
		global start_time, current_time

		current_time = time.time() - start_time

		encoder_phi 		= msg.linear.x
		encoder_theta 		= msg.linear.y
		quad_phi      		= msg.angular.x
		quad_theta    		= msg.angular.y

		self.data.linear.x = encoder_phi + quad_phi
		self.data.linear.y = encoder_theta + quad_theta
		self.data.angular.x = phi
		self.data.angular.y = theta
		self.pub.publish(self.data)
		# rospy.sleep(0.01)
		if flag_header == 0:
			header = 'Time, CAM_phi, CAM_theta, Vicon_phi, Vicon_theta\n'
			file_name = "hand_motion_trial_26_july_2020.csv"
			data_file = open(file_name, "a")
			data_file.write(header)
			data_file.close()
			flag_header = 1
		else:
			# header = 'encoder_phi, encoder_theta\n'
			file_name = "hand_motion_trial_26_july_2020_another.csv"
			data_file = open(file_name, "a")
			data_file.write(str(current_time) + "," + str(encoder_phi+quad_phi) + ',' + str(encoder_theta+quad_theta) + ',' + str(phi) + ',' + str(theta) + '\n')
			data_file.close()
		# print str(" T1 ") + str(theta_1) + str(" T2 ") + str(theta_2)
		print("Encoder phi -> " + str(encoder_phi) + "\t\t Vicon phi -> " + str(phi))
		print("Encoder theta -> " + str(encoder_theta) + "\t\t Vicon theta -> " + str(theta))


if __name__ == '__main__':
	test = read_data()
	while not rospy.is_shutdown():
		rospy.spin()
		sys.exit(1)
