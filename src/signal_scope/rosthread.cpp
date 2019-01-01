#include "rosthread.h"
#include "rossubscriber.h"

#include <QMutexLocker>
#include "ros/ros.h"


ROSThread::ROSThread()
{
  mShouldStop = false;
  mShouldPause = false;
  mNodeHandle = 0;
}

ROSThread::~ROSThread()
{
}

void ROSThread::initROS()
{
  QMutexLocker locker(&mMutex);

  if (mNodeHandle)
  {
    return;
  }

  int argc = 0;
  char** argv = NULL;
  ros::init(argc, argv, "signal_scope", ros::init_options::AnonymousName);
  mNodeHandle = new ros::NodeHandle();

}

void ROSThread::addSubscriber(ROSSubscriber* subscriber)
{
  this->initROS();
  mSubscribers.append(subscriber);
  subscriber->subscribe(mNodeHandle);
}

void ROSThread::removeSubscriber(ROSSubscriber* subscriber)
{
  this->initROS();
  subscriber->unsubscribe(mNodeHandle);
  mSubscribers.removeAll(subscriber);
}

void ROSThread::run()
{
  this->initROS();

  while (!mShouldStop)
  {
    if (mShouldPause)
    {
      mWaitCondition.wakeAll();
      this->waitForResume();
    }

    std::cout << "Number of ROS subscribers: " << mSubscribers.size() << "\n";
    ros::spin();
    printf("Terminating ROS thread.\n");
    break;
  }
}

void ROSThread::stop()
{
  mShouldStop = true;
  this->resume();
}

void ROSThread::pause()
{
  mMutex.lock();
  mShouldPause = true;
  mWaitCondition.wait(&mMutex, 100);
  mMutex.unlock();
}

void ROSThread::waitForResume()
{
  mMutex.lock();
  mWaitCondition.wait(&mMutex);
  mMutex.unlock();
}

void ROSThread::resume()
{
  mShouldPause = false;
  mWaitCondition.wakeAll();
}
