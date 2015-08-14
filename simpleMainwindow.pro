######################################################################
# Automatically generated by qmake (2.01a) Wed Nov 5 09:14:54 2014
######################################################################

QT += qt3support
QMAKE_CXXFLAGS += -frounding-math
QMAKE_CXXFLAGS += -std=c++0x##使用 C++ 11 的新特性\

DEFINES += GL_GLEXT_PROTOTYPES

SOURCES += \
    src/main.cpp \
    src/GLViewer.cpp \
    src/MainWidow.cpp \
    src/Mesh.cpp \
    src/Shader.cpp\
    src/newTest.cpp \
    src/CGSolver.cpp \
    src/LaplaceTransform.cpp

HEADERS += \
    include/MainWindow.h \
    include/GLViewer.h \
    include/Shader.h \
    include/Mesh.h \
    include/LaplaceTransform.h \
    include/LinearSolver.h \
    include/union.h \
    lib/CGAL/Nef_polyhedron_3.h \
    lib/CGAL/Polyhedron_3.h \
    lib/CGAL/Exact_predicates_exact_constructions_kernel.h \
    lib/CGAL/IO/Polyhedron_iostream.h\
    lib/CGAL/IO/Nef_polyhedron_iostream_3.h



INCLUDEPATH +=   /usr/include/qt4/QtCore     \
                 /usr/include/qt4/QtGui      \
                 /usr/include/qt4/QtOpenGL    \
                 /usr/include/qt4             \
                 /usr/include/qt3/          \
                 /usr/include/qt4/QtXml/    \
                 eigen/ \
                 include/ \
                 lib/\
                 lib/libQGLViewer-2.5.3/ \
                 lib/openMesh3.3/inlcude/ \


LIBS  +=     -L/usr/X11R6/lib64 -lQtOpenGL -lQtGui -lQtCore  -lpthread\
             -lGL  -lGLU \
             -L./lib/libQGLViewer-2.5.3/QGLViewer/ -lQGLViewer \
             -L./lib/openMesh3.3/lib/ -lOpenMeshCore -lOpenMeshTools \
              -lboost_thread   -lboost_program_options  -lboost_filesystem  -lboost_system\
             -L/usr/lib/x86_64-linux-gnu/ -lgmp -lmpfr  \
             -L./lib/openMesh3.3/lib/ -lOpenMeshCore -lOpenMeshTools \
             -L./lib/CGAL-4.6/build/lib/ -lCGAL  -lCGAL_Core -lCGAL_ImageIO

FORMS += \
    ui_templates/mainwindow.ui

DEFINES += GL_GLEXT_PROTOTYPES

