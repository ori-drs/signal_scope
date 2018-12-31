#ifndef _LCMSUBSCRIBER_h
#define _LCMSUBSCRIBER_h

#include <QObject>

#include <lcm/lcm-cpp.hpp>
#include "ros/ros.h"

namespace lcm
{
  class LCM;
}

namespace ros
{
  class NodeHandle;
}


class LCMSubscriber : public QObject
{
  Q_OBJECT

public:

  LCMSubscriber(QObject* parent=0) : QObject(parent)
  {
    mSubscription = 0;
  }

  virtual ~LCMSubscriber()
  {

  }

  virtual void subscribe(ros::NodeHandle* nodeInstance) = 0;

  virtual void unsubscribe(ros::NodeHandle* nodeInstance)
  {
    std::cout <<"REIMPLEMNT unsubscribe\n";
    //lcmHandle->unsubscribe(mSubscription);
    mSubscription = 0;
  }

 protected:

  //lcm::Subscription* mSubscription;
  ros::Subscriber* mSubscription;
};

#endif
