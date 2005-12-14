######################################################################
# Automatically generated by qmake (1.07a) Thu Nov 24 21:42:45 2005
######################################################################

TEMPLATE = app
TARGET = test_runner

INCLUDEPATH += . spelling ../../src/database ../../src/compatibility
LIBS	+= -L/usr/local/lib -lcppunit

# Input
HEADERS += playerdatabasetest.h \
           spelling/spellcheckertests.h \
           ../../src/database/databaseconversion.h \
           ../../src/database/partialdate.h \
           ../../src/database/playerdatabase.h \
           ../../src/database/playerdata.h \
           ../../src/database/spellchecker.h \
           ../../src/database/board.h \
           ../../src/database/move.h \
           ../../src/database/common.h \
           ../../src/database/history.h \
           ../../src/database/search.h \
           ../../src/database/query.h \
           databaseconversiontest.h \
           playerdatabasechangestest.h \
           boardtest.h \
           querytest.h


SOURCES += playerdatabasetest.cpp \
           test_runner.cpp \
           spelling/spellcheckertests.cpp \
           ../../src/database/databaseconversion.cpp \
           ../../src/database/partialdate.cpp \
           ../../src/database/playerdatabase.cpp \
           ../../src/database/playerdata.cpp \
           ../../src/database/spellchecker.cpp \
          ../../src/database/board.cpp \
           ../../src/database/move.cpp \
           ../../src/database/common.cpp \
           ../../src/database/history.cpp \
           ../../src/database/search.cpp \
           ../../src/database/query.cpp \
          databaseconversiontest.cpp \
          playerdatabasechangestest.cpp \
          boardtest.cpp \
          querytest.cpp

