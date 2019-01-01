#ifndef _ROSSUBSCRIBER_h
#define _ROSSUBSCRIBER_h

#include <QObject>

#include "ros/ros.h"

namespace ros
{
  class NodeHandle;
}


class ROSSubscriber : public QObject
{
  Q_OBJECT

public:

  ROSSubscriber(QObject* parent=0) : QObject(parent)
  {
    mSubscription = 0;
  }

  virtual ~ROSSubscriber()
  {

  }

  virtual void subscribe(ros::NodeHandle* nodeInstance) = 0;

  virtual void unsubscribe(ros::NodeHandle* nodeInstance)
  {
    mSubscription->shutdown();
    mSubscription = 0;
  }

 protected:

  ros::Subscriber* mSubscription;
};

#endif
