#!/usr/bin/env python
import rospy
import numpy
from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import TwistStamped

def talker():
    pub1 = rospy.Publisher('/PoseStamped', PoseStamped, queue_size=10)
    pub2 = rospy.Publisher('/TwistStamped', TwistStamped, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(100) # 100hz
    i = 0.0
    while not rospy.is_shutdown():
        message = "running: %s" % rospy.get_time()
        message = message + " " + str(int(i))
        rospy.loginfo_throttle(1, message)

        p = PoseStamped()
        p.pose.position.x = numpy.sin(i/10);
        p.header.stamp = rospy.Time.now()
        pub1.publish(p)

        t = TwistStamped()
        t.twist.linear.x = 1.5*numpy.cos(i/20);
        t.header.stamp = p.header.stamp
        pub2.publish(t)

        i = i+1.0
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
