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

# x64 /bigobj
QMAKE_CXXFLAGS += /bigobj


TEMPLATE = lib
TARGET = Mapping

#Config DESTDIR
CONFIG(debug, debug|release){
    DESTDIR = ../../../../Bin/Debug/x64
}else:CONFIG(release, debug|release){
    DESTDIR = ../../../../Bin/Release/x64
}

QT += core

# Preprocessor Definitions
DEFINES += _UNICODE QT_DLL SU_MAPPING_EXPORTS _UGUNICODE

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
    # wchar_t
    QMAKE_CXXFLAGS += -Zc:wchar_t

    # x64
    CONFIG(debug, debug|release){

    LIBPATH = $${SUOBJECTSDIR}/lib/libd_x64
    LIBS +=  -lSuBased         \
             -lSuBase3Dd       \
             -lSuChartBased    \
             -lSuDrawingd      \
             -lSuElementd      \
             -lSuEngined       \
             -lSuFileParserd   \
             -lSuFMELicensed   \
             -lSuGeometryd     \
             -lSuGraphicsd     \
             -lSuGraphics3Dd   \
             -lSuMapd          \
             -lSuMapEditord    \
             -lSuNetToolkitd   \
             -lSuOGDCd         \
             -lSuProjectiond   \
             -lSuRenderd       \
             -lSuScened        \
             -lSuSceneEditord  \
             -lSuSpatialIndexd \
             -lSuStreamd       \
             -lSuToolkitd      \
             -lSuWorkspaced    \
             -lSuLayer3DDatasetd \
             -lSuGeometry3Dd   \

    }else:CONFIG(release, debug|release){

    LIBPATH = $${SUOBJECTSDIR}/lib/lib_x64
    LIBS +=  -lSuBase         \
             -lSuBase3D       \
             -lSuChartBase    \
             -lSuDrawing      \
             -lSuElement      \
             -lSuEngine       \
             -lSuFileParser   \
             -lSuFMELicense   \
             -lSuGeometry     \
             -lSuGraphics     \
             -lSuGraphics3D   \
             -lSuMap          \
             -lSuMapEditor    \
             -lSuNetToolkit   \
             -lSuOGDC         \
             -lSuProjection   \
             -lSuRender       \
             -lSuScene        \
             -lSuSceneEditor  \
             -lSuSpatialIndex \
             -lSuStream       \
             -lSuToolkit      \
             -lSuWorkspace    \
             -lSuLayer3DDataset \
    }
}

unix:{
    # 16bit wchar
    QMAKE_CXXFLAGS =-fshort-wchar
    # 支持C++11
    QMAKE_CXXFLAGS += -std=c++11
    # 对std库使用旧ABI, 也可在头文件开始处使用#define _GLIBCXX_USE_CXX11_ABI 0, 如此只对该文件起作用；但独立的库更好管理;
    # 新ABI在库中的链接名称如_cxx11::std::string, 旧ABI在库中名称如std::string
    # GCC 默认值为1，使用新ABI;
    DEFINES += _GLIBCXX_USE_CXX11_ABI=0

    LIBS +=-L$${SUOBJECTSDIR}/bin/bin \
             -lSuBase         \
             -lSuBase3D       \
             -lSuChartBase    \
             -lSuDrawing      \
             -lSuElement      \
             -lSuEngine       \
             -lSuFileParser   \
             -lSuFMELicense   \
             -lSuGeometry     \
             -lSuGraphics     \
             -lSuGraphics3D   \
             -lSuMap          \
             -lSuMapEditor    \
             -lSuNetToolkit   \
             -lSuOGDC         \
             -lSuProjection   \
             -lSuRender       \
             -lSuScene         \
             -lSuSceneEditor   \
             -lSuSpatialIndex \
             -lSuStream       \
             -lSuToolkit      \
             -lSuWorkspace    \
}

    # Libs of SuObjectsCpp
    LIBS += -L$${DESTDIR} \
            -lData


# Module source files
include(Mapping.pri)
