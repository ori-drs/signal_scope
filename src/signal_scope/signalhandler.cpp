#include "signalhandler.h"

#include "signaldata.h"
#include "signaldescription.h"


SignalHandler::SignalHandler(const SignalDescription* signalDescription, QObject* parent) : ROSSubscriber(parent)
{
  mDescription = *signalDescription;
  mSignalData = new SignalData();
}

SignalHandler::~SignalHandler()
{
  delete mSignalData;
}

void SignalHandler::handleMessage()
{
}

void SignalHandler::subscribe(ros::NodeHandle* rosInstance)
{
}

SignalHandlerFactory& SignalHandlerFactory::instance()
{
  static SignalHandlerFactory factory;
  return factory;
}

SignalHandler* SignalHandlerFactory::createHandler(const SignalDescription* desc) const
{
  Constructor constructor = mConstructors.value(desc->mMessageType).value(desc->mFieldName);
  if (constructor == NULL)
  {
    return NULL;
  }
  return (*constructor)(desc);
}

double SignalHandlerFactory::getOffsetTime(int64_t messageTime)
{
  if (mTimeOffset == 0)
  {
    mTimeOffset = messageTime;
  }
  return (messageTime - mTimeOffset) * 1e-6;
}
