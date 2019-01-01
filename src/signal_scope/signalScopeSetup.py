import os
import sys
import glob
import time
import traceback
from PythonQt import QtGui


class LocalTimeHelper(object):

  def _getResolverFunction(self):
      return lambda x: time.time()*1e6


class LookupHelper(object):


  def __init__(self, lookups=()):
      self._lookups = lookups

  def __getitem__(self, i):
      return LookupHelper(self._lookups + ([i],))

  def __getattr__(self, attr):
      return LookupHelper(self._lookups + (attr,))

  def _getResolverFunction(self):

    lookups = []
    name = ''
    for a in self._lookups:
        if isinstance(a, str):
            if len(name): name += '.'
            name += '%s' % a
            f = lambda msg, field, a=a: getattr(field, a)
        else:
          a = a[0]
          if isinstance(a, int):
              name += '[%d]' % a
              f = lambda msg, field, a=a: field[a]
          if isinstance(a, str):
              name += "['%s']" % a

              def f(msg, field, a=a, data=[]):
                  if not data:
                      data.append(field.index(a))
                  return data[0]

          elif isinstance(a, LookupHelper):
              key = a._lookups[-1][0]
              subFunc = a._getResolverFunction()
              name += '[%s]' % key #subFunc.__doc__

              f = lambda msg, field, a=a: field[subFunc(msg)]

        lookups.append(f)


    def func(msg):
        value = msg
        for x in lookups:
            value = x(msg, value)
        return value

    func.__doc__ = name
    return func


def createSignalFunction(timeLookup, valueLookup):
    t = timeLookup._getResolverFunction()
    v = valueLookup._getResolverFunction()

    def func(msg):
        return t(msg).to_sec(), v(msg)
    func.__doc__ = v.__doc__
    return func



def decodeMessageFunction(messageBytes, messageType):
    s = str(messageBytes)
    # Use messageType string to import ros message type
    # TODO: 
    # - find out if is this efficient - e.g. at 500Hz
    # - cache the imported messages and skip importing those that have been
    # - remove the LCM does for decoding
    messagePackage,messageType= str.split(str(messageType),'/')
    exec('from ' + messagePackage + '.msg import ' + messageType )
    exec('p = ' + messageType + '()')
    p.deserialize(s)
    return p


msg = LookupHelper()
tNow = LocalTimeHelper()

def setFormatOptions(pointSize=None, timeWindow=None, curveStyle=None):
    window = _mainWindow;
    if pointSize is not None:
      window.onPointSizeChanged(pointSize)
    if timeWindow is not None:
      window.onTimeWindowChanged(timeWindow)
    if curveStyle is not None:
      if curveStyle in ["dots","lines"]:
        window.onCurveStyleChanged(curveStyle);

def addPlot(pointSize=None, timeWindow=None, yLimits=None):
    plot = _mainWindow.addPlot()
    if timeWindow is not None:
        plot.setTimeWindow(timeWindow)
    if yLimits is not None:
        plot.setYAxisScale(yLimits[0], yLimits[1])

    return plot


def getPlot():
    plots = getPlots()
    return plots[-1] if plots else None


def getPlots():
    return _mainWindow.getPlots()


def removePlots():
    _mainWindow.onRemoveAllPlots()


def addSignalFunction(channel, signalFunction, plot=None, color=None, wrap=True, label=None):

    if plot is None:
        plot = getPlot()
        if plot is None:
            plot = addPlot()

    if color is not None:
        rgb = [int(c*255) for c in color]
        color = QtGui.QColor(*rgb)
    else:
        color = QtGui.QColor()

    if wrap:
        def _signalFunction(x):
            t, v = signalFunction(x)
            return t*1e-6, float(v)
    else:
        _signalFunction = signalFunction

    _mainWindow.addPythonSignal(plot, [channel, _signalFunction, label or signalFunction.__doc__, color])


def addSignalFunctions(channel, signalFunction, keys, keyLookup=None, plot=None, colors=None, labels=None):
    
    def func(key, keyStr):
        def f (msg):
            t, x = signalFunction(msg)
            return t, x[key]
        if signalFunction.__doc__:
            f.__doc__ = signalFunction.__doc__ + " " + keyStr
        return f

    if colors is None:
        colors = [None] * len(keys)

    if labels is None:
        labels = [None] * len(keys)

    for key, color, label in zip(keys, colors, labels):

        keyStr = str(key)        
        if keyLookup is not None:
            key = keyLookup[key]

        addSignalFunction(channel, func(key,keyStr), plot=plot, color=color, label=label)



def addSignal(channel, timeLookup, valueLookup, plot=None, color=None, label=None):

    signalFunction = createSignalFunction(timeLookup, valueLookup)
    addSignalFunction(channel, signalFunction, plot=plot, color=color, wrap=False, label=label)


def addSignals(channel, timeLookup, valueLookup, keys, keyLookup=None, plot=None, colors=None, labels=None):

    if colors is None:
        colors = [None] * len(keys)

    if labels is None:
        labels = [None] * len(keys)

    for key, color, label in zip(keys, colors, labels):

        if keyLookup is not None:
            key = keyLookup[key]

        addSignal(channel, timeLookup, valueLookup[key], plot=plot, color=color, label=label)
