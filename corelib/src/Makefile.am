INCLUDES = -I$(top_srcdir)/src ${DEPS_CFLAGS}

libcomradedir = $(includedir)/comrade

libcomrade_HEADERS = CIRC.hh Configurator.hh Constants.hh CorelibObject.hh EvidenceGrid.hh Frame.hh FrameCache.hh Framer.hh Logger.hh MouseOdometer.hh Multicaster.hh ParallelPort.hh RobotInterface.hh RobotList.hh SectorDraw.hh SerialRobotInterface.hh Singleton.hh SerialHandler.hh

lib_LTLIBRARIES     = libcomrade.la
libcomrade_la_SOURCES = CIRC.cc Configurator.cc CorelibObject.cc EvidenceGrid.cc Frame.cc FrameCache.cc Framer.cc Logger.cc MouseOdometer.cc Multicaster.cc ParallelPort.cc RobotInterface.cc RobotList.cc SectorDraw.cc SerialRobotInterface.cc SerialHandler.cc
libcomrade_la_LIBS = $(DEPS_LIBS)
