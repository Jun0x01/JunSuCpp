# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

# SUOBJECTSDIR is path of SuperMap iObjects for C++, it must be defined in Qt Project Build Environment or PC's System Enviroment.
SUOBJECTSDIR = "$$(SUOBJECTSDIR)"
!exists($${SUOBJECTSDIR}){
   message("SUOBJECTSDIR is path of SuperMap iObjects for C++, it must be defined in Qt Project Build Environment or PC's System Enviroment.")
   error(Error: The path of SUOBJECTSDIR doesn't exist, it's value : $${SUOBJECTSDIR} )
}
#message($${SUOBJECTSDIR})

# Include Path
INCLUDEPATH +=$${SUOBJECTSDIR}/include\
    $${SUOBJECTSDIR}/include/private\
    ../../../Include

# wchar_t
QMAKE_CXXFLAGS += -Zc:wchar_t
# x64 /bigobj
QMAKE_CXXFLAGS += /bigobj

TEMPLATE = lib
TARGET = Data

#Config DESTDIR
CONFIG(debug, debug|release){
    DESTDIR = ../../../../Bin/Debug/x64
}else:CONFIG(release, debug|release){
    DESTDIR = ../../../../Bin/Release/x64
}

QT += core
CONFIG += debug

# Preprocessor Definitions
DEFINES += _UNICODE QT_DLL SU_DATA_EXPORTS

# Include Path of this project
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/$(ConfigurationName)

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

#Dependencies

win32{
    # x64
    CONFIG(debug, debug|release){

    LIBPATH = $${SUOBJECTSDIR}/lib/libd_x64
    LIBS +=-lSuToolkitd \
       -lSuElementd \
       -lSuOGDCd \
       -lSuBased \
       -lSuWorkspaced \
       -lSuDrawingd \
       -lSuMapEditord \
       -lSuSpatialIndexd \
       -lSuEngined \
       -lSuGraphicsd \
       -lSuMapd \
       -lSuChartBased \
       -lSuBase3Dd \
       -lSuGeometryd \
       -lSuFileParserd \
    }else:CONFIG(release, debug|release){

    LIBPATH = $${SUOBJECTSDIR}/lib/lib_x64
    LIBS +=-lSuToolkit \
       -lSuElement \
       -lSuOGDC \
       -lSuBase \
       -lSuWorkspace \
       -lSuDrawing \
       -lSuMapEditor \
       -lSuSpatialIndex \
       -lSuEngine \
       -lSuGraphics \
       -lSuMap \
       -lSuChartBase \
       -lSuBase3D \
       -lSuGeometry \
       -lSuFileParser \
    }
}

unix:{

    LIBS +=-L$${SUOBJECTSDIR} \
       -lSuToolkit \
       -lSuElement \
       -lSuOGDC \
       -lSuBase \
       -lSuWorkspace \
       -lSuDrawing \
       -lSuMapEditor \
       -lSuSpatialIndex \
       -lSuEngine \
       -lSuGraphics \
       -lSuMap \
       -lSuChartBase \
       -lSuBase3D \
       -lSuGeometry \
       -lSuFileParser \
}

message($${DESTDIR})
message($${LIBPATH})

# Module source files
include(Data.pri)
