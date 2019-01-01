#ifndef _PYTHONCHANNELSUBSCRIBERCOLLECTION_H_
#define _PYTHONCHANNELSUBSCRIBERCOLLECTION_H_

#include "rosthread.h"
#include "pythonchannelsubscriber.h"
#include "pythonsignalhandler.h"


#include <PythonQt.h>


class PythonChannelSubscriberCollection : public QObject
{
  Q_OBJECT

public:

  PythonChannelSubscriberCollection(ROSThread* rosThread, PythonQtObjectPtr decodeCallback, QObject* parent=0) : QObject(parent)
  {
    mROSThread = rosThread;
    mDecodeCallback = decodeCallback;
  }

  void addSignalHandler(SignalHandler* handler)
  {
    PythonSignalHandler* pythonHandler = qobject_cast<PythonSignalHandler*>(handler);
    if (pythonHandler)
    {
      this->addSignalHandler(pythonHandler);
    }
    else
    {
      mROSThread->addSubscriber(handler);
    }
  }

  void removeSignalHandler(SignalHandler* handler)
  {
    PythonSignalHandler* pythonHandler = qobject_cast<PythonSignalHandler*>(handler);
    if (pythonHandler)
    {
      this->removeSignalHandler(pythonHandler);
    }
    else
    {
      mROSThread->removeSubscriber(handler);
    }
  }

  void addSignalHandler(PythonSignalHandler* handler)
  {
    QString channel = handler->channel();

    if (!mChannelSubscribers.contains(channel))
    {
      PythonChannelSubscriber* subscriber = new PythonChannelSubscriber(channel, mDecodeCallback, this);
      mChannelSubscribers[channel] = subscriber;
      mROSThread->addSubscriber(subscriber);
    }

    PythonChannelSubscriber* subscriber = mChannelSubscribers[channel];
    subscriber->addSignalHandler(handler);
  }

  void removeSignalHandler(PythonSignalHandler* handler)
  {
    QString channel = handler->channel();
    PythonChannelSubscriber* subscriber = mChannelSubscribers.value(channel);
    if (!subscriber)
    {
      return;
    }

    subscriber->removeSignalHandler(handler);
  }

 protected:

  ROSThread* mROSThread;
  PythonQtObjectPtr mDecodeCallback;
  QMap<QString, PythonChannelSubscriber* > mChannelSubscribers;
};


#endif
