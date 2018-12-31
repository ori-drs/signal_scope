#ifndef _LCMTHREAD_H_
#define _LCMTHREAD_H_

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <string>

#include "std_msgs/String.h"

class LCMSubscriber;

namespace lcm
{
  class LCM;
  class Subscription;
  class ReceiveBuffer;
}

namespace ros
{
  class NodeHandle;
}

class LCMThread : public QThread
 {
  Q_OBJECT

public:

  LCMThread();

  ~LCMThread();

  void stop();
  void pause();
  void resume();

  void addSubscriber(LCMSubscriber* subscriber);
  void removeSubscriber(LCMSubscriber* subscriber);

 protected:


  void handleMessageOnChannel(const std_msgs::String::ConstPtr& msg);//const lcm::ReceiveBuffer* rbuf, const std::string& channel);


  void run();
  void initLCM();
  void waitForResume();

  bool mShouldPause;
  bool mShouldStop;
  QList<LCMSubscriber*> mSubscribers;

  lcm::LCM* mLCM;

  ros::NodeHandle* mNodeHandle;

  QMutex mMutex;
  QWaitCondition mWaitCondition;
};

#endif
