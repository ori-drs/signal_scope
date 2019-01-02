#!/usr/bin/env python
import rospy
from tf2_msgs.msg import TFMessage
from geometry_msgs.msg import PoseStamped
from std_msgs.msg import Int32

import tf
from tf import transformations

import lcm
from bot_core.pose_t import pose_t

import math
import numpy
lc = lcm.LCM()
import time



from geometry_msgs.msg import TwistStamped


msgout = pose_t()
i = 0.0


pub = rospy.Publisher('/pose_controller/twist', TwistStamped, queue_size=10)
rospy.init_node('talker', anonymous=True)
rate = rospy.Rate(10) # 10hz

while not rospy.is_shutdown():
    msg = TwistStamped()
    msg.header.frame_id = "base"
    msg.header.stamp = rospy.Time.now()

    msg.twist.linear.x = numpy.sin(i/10);
    pub.publish(msg)

    msgout.utime = (i/10)*1E6;#getUtime()
    msgout.pos[0] = numpy.sin(i/10);
    lc.publish("POSE_BODY", msgout.encode())
    #time.sleep(0.1)

    i = i+1.0
    print "sent",i

    rate.sleep()    