#if("${USED_QTVERSION}" STREQUAL "5")
  find_package(Qt5 REQUIRED Core Gui Script Widgets)
  set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Script Qt5::Widgets)
  # Compatibility macros
  macro(qt4_wrap_cpp)
    qt5_wrap_cpp(${ARGN})
  endmacro()
  macro(qt4_wrap_ui)
    qt5_wrap_ui(${ARGN})
  endmacro()
  macro(qt4_add_resources)
    qt5_add_resources(${ARGN})
  endmacro()
  if(APPLE)
    find_library(QWT_LIBRARY qwt)
  else()
    find_library(QWT_LIBRARY qwt-qt5)
  endif()
#else()
#  find_library(QWT_LIBRARY qwt)
#  find_package(Qt4 REQUIRED QtCore QtGui QtScript)
#  include(${QT_USE_FILE})
#endif()

set(deps)

find_path(QWT_INCLUDE_DIR qwt/qwt.h HINTS /usr/local/lib)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${QWT_INCLUDE_DIR})

if(USE_PYTHON)

  if(APPLE)
    find_program(PYTHON_CONFIG_EXECUTABLE python-config)
    if (NOT PYTHON_CONFIG_EXECUTABLE)
      message(SEND_ERROR "python-config executable not found, but python is required.")
    endif()
    # using "python-config --prefix" so that cmake always uses the python that is
    # in the users path, this is a fix for homebrew on Mac:
    # https://github.com/Homebrew/homebrew/issues/25118
    execute_process(COMMAND ${PYTHON_CONFIG_EXECUTABLE} --prefix OUTPUT_VARIABLE python_prefix OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(PYTHON_INCLUDE_DIR ${python_prefix}/include/python2.7)
    set(PYTHON_LIBRARY ${python_prefix}/lib/libpython2.7${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

  find_package(PythonLibs 2.7 REQUIRED)
  find_package(PythonQt REQUIRED)
  find_package(CTKPythonConsole REQUIRED)

  include_directories(${PYTHON_INCLUDE_DIRS})
  include_directories(${PYTHONQT_INCLUDE_DIRS})
  include_directories(${CTK_PYTHONCONSOLE_INCLUDE_DIRS})

  set(python_deps ${PYTHON_LIBRARIES} ${CTK_PYTHONCONSOLE_LIBRARIES} ${PYTHONQT_LIBRARIES})
  set(python_moc_srcs
    pythonsignalhandler.h
    pythonchannelsubscribercollection.h
    pythonchannelsubscriber.h
    pythonmessageinspector.h
    )
  set(python_srcs)
endif()

qt4_wrap_cpp(moc_srcs
  mainwindow.h
  plot.h
  plotwidget.h
  rosthread.h
  rossubscriber.h
  signalhandler.h
  setscaledialog.h
  selectsignaldialog.h
  ${python_moc_srcs}
  )

qt4_wrap_ui(ui_srcs
  mainwindow.ui
  setscaledialog.ui
  selectsignaldialog.ui
  )

qt4_add_resources(resource_srcs
  resources.qrc
  )

set(srcs
  ${moc_srcs}
  ${ui_srcs}
  ${resource_srcs}
  curvedata.cpp
  jointnames.cpp
  main.cpp
  mainwindow.cpp
  plot.cpp
  plotwidget.cpp
  setscaledialog.cpp
  signaldata.cpp
  signaldescription.cpp
  rosthread.cpp
  signalhandler.cpp
  selectsignaldialog.cpp
  ${python_srcs}
  )

add_executable(signal_scope ${srcs})

list(APPEND deps
  ${QT_LIBRARIES}
  ${QWT_LIBRARY}
  ${python_deps}
  ${catkin_LIBRARIES}
  )

target_link_libraries(signal_scope ${deps})


install(TARGETS ${TARGETS}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)