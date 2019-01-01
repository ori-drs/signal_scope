'''
Here is a basic example to plot two signals from ROS messages.
In this example, the topic names are /PoseStamped and /TwistStamped.

The X coordinate is extraced from the message stampthe message timestamp in microseconds,
The Y value is indicated variable.

Note, msg is a pre-defined variable that you must use in order
for this to work.  When you define a signal, the msg variable
is used to record the attribute lookups that are required to
extract the signal data from the lcm message in the future.
'''

addSignal('/PoseStamped', msg.header.stamp, msg.pose.position.x)
addSignal('/TwistStamped', msg.header.stamp, msg.twist.linear.x)