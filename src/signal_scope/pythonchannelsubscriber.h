#ifndef _PYTHONCHANNELSUBSCRIBER_H_
#define _PYTHONCHANNELSUBSCRIBER_H_

#include "lcmsubscriber.h"
#include "lcmthread.h"
#include "pythonsignalhandler.h"
#include <topic_tools/shape_shifter.h>

#include <PythonQt.h>


class PythonChannelSubscriber : public LCMSubscriber
{
  Q_OBJECT

public:

  PythonChannelSubscriber(QString channel, PythonQtObjectPtr decodeCallback, QObject* parent=0) : LCMSubscriber(parent)
  {
    mChannel = channel;
    mDecodeCallback = decodeCallback;
  }

  QString channel()
  {
    return mChannel;
  }

  void subscribe(ros::NodeHandle* lcmHandle)
  {
    if (mSubscription)
    {
      printf("error: subscribe() called without first calling unsubscribe.\n");
      return;
    }

    std::string topic_name;
#if QT_VERSION >= 0x050000
    topic_name = this->channel().toLatin1().data();
#else
    topic_name = this->channel().toAscii().data();
#endif
    boost::function<void(const topic_tools::ShapeShifter::ConstPtr&) > callback;
    callback = boost::bind( &PythonChannelSubscriber::handleMessageOnChannel, this, _1, topic_name ) ;
    mSub = lcmHandle->subscribe( topic_name, 1000, callback);
    mSubscription = &mSub;
  }

  void addSignalHandler(PythonSignalHandler* handler)
  {
    mHandlers.append(handler);
  }

  void removeSignalHandler(PythonSignalHandler* handler)
  {
    mHandlers.removeAll(handler);
  }

  QVariant decodeMessage(const topic_tools::ShapeShifter::ConstPtr& msg)
  {
    if (!mDecodeCallback)
    {
      return QVariant();
    }

    // Once again, it would be nice to access directly the 
    // private member ShapeShifter::msgBuf, unfortunately we can't.
    // This oblige us to do a copy of the buffer using ShapeShifter::write
    static std::vector<uint8_t> buffer;
    buffer.resize( msg->size() );
    ros::serialization::OStream stream(buffer.data(), buffer.size());
    msg->write(stream);

    QVariantList args;
    args << QByteArray((char*)buffer.data(), buffer.size());
    args << QString(msg->getDataType().c_str());
    return PythonQt::self()->call(mDecodeCallback, args);
  }


  void handleMessageOnChannel(const topic_tools::ShapeShifter::ConstPtr& msg,
                              const std::string &topic_name )
  {
    if (!mHandlers.size())
    {
      return;
    }

    QVariant decodedMessage = this->decodeMessage(msg);
    if (!decodedMessage.isValid())
    {
      printf("failed to decode message on topic: %s\n", topic_name.c_str());
      return;
    }

    foreach (PythonSignalHandler* handler, mHandlers)
    {
      handler->onNewMessage(decodedMessage);
    }
  }


  QString mChannel;
  PythonQtObjectPtr mDecodeCallback;
  QList<PythonSignalHandler*> mHandlers;

  ros::Subscriber mSub; // needed so at object is not destroyed
};

#endif
