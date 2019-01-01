#include "lcmthread.h"
#include "lcmsubscriber.h"

#include <QMutexLocker>
#include <lcm/lcm-cpp.hpp>

#include "ros/ros.h"


LCMThread::LCMThread()
{
  mShouldStop = false;
  mShouldPause = false;
  mLCM = 0;
  mNodeHandle = 0;
}

LCMThread::~LCMThread()
{
}

void LCMThread::initLCM()
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

void LCMThread::addSubscriber(LCMSubscriber* subscriber)
{
  this->initLCM();
  mSubscribers.append(subscriber);
  subscriber->subscribe(mNodeHandle);
}

void LCMThread::removeSubscriber(LCMSubscriber* subscriber)
{
  this->initLCM();
  subscriber->unsubscribe(mNodeHandle);
  mSubscribers.removeAll(subscriber);
}

void LCMThread::run()
{
  this->initLCM();

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

void LCMThread::stop()
{
  mShouldStop = true;
  this->resume();
}

void LCMThread::pause()
{
  mMutex.lock();
  mShouldPause = true;
  mWaitCondition.wait(&mMutex, 100);
  mMutex.unlock();
}

void LCMThread::waitForResume()
{
  mMutex.lock();
  mWaitCondition.wait(&mMutex);
  mMutex.unlock();
}

void LCMThread::resume()
{
  mShouldPause = false;
  mWaitCondition.wakeAll();
}
