INCLUDES = -I$(top_srcdir)/src ${DEPS_CFLAGS} ${GTKMM_CFLAGS}

if WITH_GUI
gui_sources=LocalGUI.h LocalGUI.cpp GridDrawingArea.cpp GridDrawingArea.h MainWindow.cpp MainWindow.h
else
gui_sources=
endif

bin_PROGRAMS = comrade

comrade_SOURCES = AbstractHardware.cpp AbstractHardware.h ArgDecs.cpp ArgDecs.h ByteObjectConvertor.cpp ByteObjectConvertor.h CircularBuffer.cpp CircularBuffer.h CommandBinder.cpp CommandBinder.h CommandFunctionList.cpp CommandFunctionList.h ComradeFrontEnd.cpp ComradeFrontEnd.h Dependency.cpp Dependency.h ExecutionEngine.cpp ExecutionEngine.h Feature.cpp Feature.h FeatureList.cpp FeatureList.h FrontEnd.cpp FrontEnd.h Interpreter.cpp Interpreter.h LineFollower.cpp LineFollower.h LocalisationEngine.cpp LocalisationEngine.h MotorController.h MotorController.cpp MulticastSignalBuffer.cpp MulticastSignalBuffer.h ObstacleAvoider.h ObstacleAvoider.cpp OdometryEngine.h OdometryEngine.cpp RuntimeDemo.cpp SensorStructure.cpp SensorStructure.h StereovisionMotor.cpp StereovisionMotor.h WordFinder.cpp WordFinder.h $(gui_sources)

comrade_LDADD = ${DEPS_LIBS} ${LOG4CPP_LIBS} ${GTKMM_LIBS} -lfltk
