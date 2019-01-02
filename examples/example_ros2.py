'''
A working example for signals from Anymal

Plots x,y,z in position and the yaw angle
'''


import numpy

import sys
sys.argv = ['test']
import tf


def getYawDegrees(msg):
    '''yaw degrees'''
    quaternion = (
        msg.pose.pose.orientation.x,
        msg.pose.pose.orientation.y,
        msg.pose.pose.orientation.z,
        msg.pose.pose.orientation.w)
    euler = tf.transformations.euler_from_quaternion(quaternion)
    return msg.header.stamp, euler[2]*180.0/numpy.pi

def positionNorm(msg):
    '''position magnitude'''
    x = [msg.pose.pose.position.x, msg.pose.pose.position.y, msg.pose.pose.position.z]
    return msg.header.stamp, numpy.linalg.norm(x)

addSignal('/state_estimator/quadruped_state', msg.header.stamp, msg.pose.pose.position.x)
addSignal('/state_estimator/quadruped_state', msg.header.stamp, msg.pose.pose.position.y)
addSignal('/state_estimator/quadruped_state', msg.header.stamp, msg.pose.pose.position.z)
addSignalFunction('/state_estimator/quadruped_state', positionNorm)

addPlot()

addSignalFunction('/state_estimator/quadruped_state', getYawDegrees)
