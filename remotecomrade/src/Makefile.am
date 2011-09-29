INCLUDES = -I$(top_srcdir)/src ${DEPS_CFLAGS}

bin_PROGRAMS = comraderemote

comraderemote_SOURCES = GridDrawingArea.cc GridDrawingArea.hh MainWindow.cc MainWindow.hh main.cc GLDrawingArea.cc GLDrawingArea.hh adam.cc adam.h gui_shared_data.cc gui_shared_data.h model.cc model.h model_parts.cc model_parts.h bitmap.c bitmap.h view3d.hh view3d.cc MapUpdater.cc MapUpdater.hh texture.c texture.h CompassDA.hh CompassDA.cc CameraDA.hh CameraDA.cc MouseDriveDA.hh MouseDriveDA.cc

comraderemote_LDADD = ${DEPS_LIBS} ${LOG4CPP_LIBS}
