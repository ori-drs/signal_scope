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
  printf("initLCM() mfallon 1.\n");
  QMutexLocker locker(&mMutex);

  printf("initLCM() mfallon 3x.\n");

  if (mNodeHandle)
  {
    return;
  }

  printf("initLCM() mfallon 4x.\n");

  int argc = 0;
  char** argv = NULL;
  ros::init(argc, argv, "scope");
  printf("initLCM() mfallon .\n");
  mNodeHandle = new ros::NodeHandle();

}

void LCMThread::addSubscriber(LCMSubscriber* subscriber)
{
  printf("addSubscriber mfallon.\n");
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
  printf("run mfallon.\n");
  this->initLCM();

  while (!mShouldStop)
  {
    if (mShouldPause)
    {
      mWaitCondition.wakeAll();
      this->waitForResume();
    }

    //int result = mLCM->handle();
    std::cout << "number of subscribers:" << mSubscribers.size() << "\n";

    ros::spin();
    std::cout << "finished spin\n";

    /*
    if (result != 0)
    {
      printf("mLCM->handle() returned non-zero.  Terminating LCM thread.\n");
      break;
    } */
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
