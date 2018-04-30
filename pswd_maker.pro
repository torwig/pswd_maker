HEADERS += \
    hasher.h \
    leet.h \
    listaccounts.h \
    passwordmaker.h \
    pwmdefaults.h \
    tinystr.h \
    tinyxml.h \
    urlsearch.h \
    pwm_common.h \
    tclap/Arg.h \
    tclap/ArgException.h \
    tclap/CmdLine.h \
    tclap/CmdLineInterface.h \
    tclap/CmdLineOutput.h \
    tclap/DocBookOutput.h \
    tclap/HelpVisitor.h \
    tclap/IgnoreRestVisitor.h \
    tclap/MultiArg.h \
    tclap/StdOutput.h \
    tclap/SwitchArg.h \
    tclap/UnlabeledMultiArg.h \
    tclap/UnlabeledValueArg.h \
    tclap/ValueArg.h \
    tclap/VersionVisitor.h \
    tclap/Visitor.h \
    tclap/XorHandler.h \
    ui/main_window.h

SOURCES += \
    hasher.cpp \
    leet.cpp \
    listaccounts.cpp \
    main.cpp \
    passwordmaker.cpp \
    pwmdefaults.cpp \
    tinystr.cpp \
    tinyxml.cpp \
    tinyxmlerror.cpp \
    tinyxmlparser.cpp \
    urlsearch.cpp \
    ui/main_window.cpp

QT += core gui widgets

DEFINES += USE_MHASH TIXML_USE_STL

LIBS += -lmhash -lpcre -lpcrecpp

FORMS += \
    ui/main_window.ui

