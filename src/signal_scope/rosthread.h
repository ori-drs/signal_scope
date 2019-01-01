#ifndef _ROSTHREAD_H_
#define _ROSTHREAD_H_

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <string>

class ROSSubscriber;

namespace ros
{
  class NodeHandle;
}

class ROSThread : public QThread
 {
  Q_OBJECT

public:

  ROSThread();

  ~ROSThread();

  void stop();
  void pause();
  void resume();

  void addSubscriber(ROSSubscriber* subscriber);
  void removeSubscriber(ROSSubscriber* subscriber);

 protected:


  void handleMessageOnChannel();


  void run();
  void initROS();
  void waitForResume();

  bool mShouldPause;
  bool mShouldStop;
  QList<ROSSubscriber*> mSubscribers;

  ros::NodeHandle* mNodeHandle;

  QMutex mMutex;
  QWaitCondition mWaitCondition;
};

#endif
