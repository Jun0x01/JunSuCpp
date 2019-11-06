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
CONFIG += debug

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
    LIBS +=  -lSuAlgorithm3Dd   \
       -lSuAlgorithmd   \
       -lSuAlgorithmVCGd   \
       -lSuAllocationd   \
       -lSuAnalyst3Dd   \
       -lSuAnimationd   \
       -lSuBase3Dd   \
       -lSuBased   \
       -lSuBGDataCompilerd   \
       -lSuBPlusTreed   \
       -lSuCacheBuilderd   \
       -lSuCacheFiled   \
       -lSuChartBased   \
       -lSuChartToolkitd   \
       -lSuCompactFiled   \
       -lSuCVToolkitd   \
       -lSuDataCheckd   \
       -lSuDataExchanged   \
       -lSuDB2CId   \
       -lSuDCPackagerd   \
       -lSuDCToolkitsd   \
       -lSuDrawing3Dd   \
       -lSuDrawingd   \
       -lSuDrawingLayoutd   \
       -lSuDRDSCId   \
       -lSuElementd   \
       -lSuEngineBaiduMapsd   \
       -lSuEngineBingMapsd   \
       -lSuEngined   \
       -lSuEngineDB2d   \
       -lSuEngineDRDSd   \
       -lSuEngineESd   \
       -lSuEngineExtendFiled   \
       -lSuEngineGBased   \
       -lSuEngineGoogleMapsd   \
       -lSuEngineGPd   \
       -lSuEngineImagePlugind   \
       -lSuEngineKingbased   \
       -lSuEngineMapWorldMaps   \
       -lSuEngineMySQLd   \
       -lSuEngineODBCd   \
       -lSuEngineOGCd   \
       -lSuEngineOGDCd   \
       -lSuEngineOpenStreetMapsd   \
       -lSuEngineOracled   \
       -lSuEnginePGd   \
       -lSuEnginePGisd   \
       -lSuEngineRestd   \
       -lSuEngineSCVd   \
       -lSuEngineSDEd   \
       -lSuEngineSMCloudd   \
       -lSuEngineSQLPLUSd   \
       -lSuEngineSRDBd   \
       -lSuEngineSspd   \
       -lSuEngineUDBd   \
       -lSuFileParser3DModeld   \
       -lSuFileParser3dsd   \
       -lSuFileParserAcadd   \
       -lSuFileParserAiGridd   \
       -lSuFileParserBGRIDd   \
       -lSuFileParserBMPd   \
       -lSuFileParserCSVd   \
       -lSuFileParserd   \
       -lSuFileParserDEMd   \
       -lSuFileParserDGNd   \
       -lSuFileParserE00d   \
       -lSuFileParserECWd   \
       -lSuFileParserENCd   \
       -lSuFileParserFileGDBd   \
       -lSuFileParserFMEd   \
       -lSuFileParserGDALd   \
       -lSuFileParserGEOJSONd   \
       -lSuFileParserGJBd   \
       -lSuFileParserGPKGd   \
       -lSuFileParserGRIBd   \
       -lSuFileParserGridd   \
       -lSuFileParserGTiffd   \
       -lSuFileParserHFAd   \
       -lSuFileParserKMLd   \
       -lSuFileParserLidard   \
       -lSuFileParserMAPGISd   \
       -lSuFileParserMitabd   \
       -lSuFileParserMrSIDd   \
       -lSuFileParserOrangeTabd   \
       -lSuFileParserOSMd   \
       -lSuFileParserPGDBVd   \
       -lSuFileParserPointCloudd   \
       -lSuFileParserRAWd   \
       -lSuFileParserSCVd   \
       -lSuFileParserSGMd   \
       -lSuFileParserShaped   \
       -lSuFileParserSimpleJsond   \
       -lSuFileParserSTETd   \
       -lSuFileParserTEMSd   \
       -lSuFileParserVCTd   \
       -lSuFileParserVRTd   \
       -lSuFilePyramidd   \
       -lSuFilePyramidECWd   \
       -lSuFilePyramidGDALd   \
       -lSuFilePyramidMrSidd   \
       -lSuFilePyramidSITd   \
       -lSuFMELicensed   \
       -lSuGBaseCId   \
       -lSuGeneralizationd   \
       -lSuGeoCoded   \
       -lSuGeometricNetworkd   \
       -lSuGeometry3Dd   \
       -lSuGeometryCADd   \
       -lSuGeometryConverterd   \
       -lSuGeometryd   \
       -lSuGeometryLayoutd   \
       -lSuGeometryPlotd   \
       -lSuGeoOperation3Dd   \
       -lSuGeoOperationd   \
       -lSuGraphics3Dd   \
       -lSuGraphics3DOGREd   \
       -lSuGraphicsd   \
       -lSuGraphicsPDFd   \
       -lSuGraphicsPSd   \
       -lSuGraphicsWd   \
       -lSuGridAnalystd   \
       -lSuGridModelingd   \
       -lSuInterNationalICUd   \
       -lSuKDBCId   \
       -lSuLayer3DDatasetd   \
       -lSuLayer3DFiled   \
       -lSuLayer3DMapd   \
       -lSuLayer3DTreed   \
       -lSuLayoutEditord   \
       -lSuLinearReferenced   \
       -lSuLogisticsd   \
       -lSuMapd   \
       -lSuMapEditord   \
       -lSuMapServiceStubd   \
       -lSuMongoDBCId   \
       -lSuMosaicd   \
       -lSuMySQLCId   \
       -lSuNetToolkitd   \
       -lSuNetworkBuilderd   \
       -lSuNetworkEnvironmentd   \
       -lSuNTopoDataCompilerd   \
       -lSuNTopoIndexTreed   \
       -lSuODBCCId   \
       -lSuOGCParserd   \
       -lSuOGDCd   \
       -lSuOracleCId   \
       -lSuOverlayd   \
       -lSuParticleSystemd   \
       -lSuPathAnalystCHd   \
       -lSuPathAnalystd   \
       -lSuPathNavid   \
       -lSuPGSQLCId   \
       -lSuPluginCommond   \
       -lSuPluginGECached   \
       -lSuPluginGlobalCached   \
       -lSuPluginGridCached   \
       -lSuPluginMapCache50d   \
       -lSuPluginMapCached   \
       -lSuPluginWebCached   \
       -lSuProjectiond   \
       -lSuProximityd   \
       -lSuPublicTransportd   \
       -lSuRectifyd   \
       -lSuRenderd   \
       -lSuRepresentationd   \
       -lSuRStarTreed   \
       -lSuScened   \
       -lSuSceneEditord   \
       -lSuSDECId   \
       -lSuSpatialIndexd   \
       -lSuSpatialQueryd   \
       -lSuSQLiteCId   \
       -lSuSRDBCId   \
       -lSuStreamd   \
       -lSuSymbold   \
       -lSuSymbolExchanged   \
       -lSuSymbolMarker3Dd   \
       -lSuTheme3DBased   \
       -lSuTileStoraged   \
       -lSuToolkitd   \
       -lSuTopoBased   \
       -lSuTopoBuilderd   \
       -lSuWorkspaced   \
    }else:CONFIG(release, debug|release){

    LIBPATH = $${SUOBJECTSDIR}/lib/lib_x64
    LIBS += -lSuAlgorithm   \
       -lSuAlgorithm3D   \
       -lSuAlgorithmVCG   \
       -lSuAllocation   \
       -lSuAnalyst3D   \
       -lSuAnimation   \
       -lSuBase   \
       -lSuBase3D   \
       -lSuBGDataCompiler   \
       -lSuBPlusTree   \
       -lSuCacheBuilder   \
       -lSuCacheFile   \
       -lSuChartBase   \
       -lSuChartToolkit   \
       -lSuCompactFile   \
       -lSuCVToolkit   \
       -lSuDataCheck   \
       -lSuDataExchange   \
       -lSuDB2CI   \
       -lSuDCPackager   \
       -lSuDCToolkits   \
       -lSuDrawing   \
       -lSuDrawing3D   \
       -lSuDrawingLayout   \
       -lSuDRDSCI   \
       -lSuElement   \
       -lSuEngine   \
       -lSuEngineBaiduMaps   \
       -lSuEngineBingMaps   \
       -lSuEngineDB2   \
       -lSuEngineDRDS   \
       -lSuEngineES   \
       -lSuEngineExtendFile   \
       -lSuEngineGBase   \
       -lSuEngineGoogleMaps   \
       -lSuEngineGP   \
       -lSuEngineImagePlugin   \
       -lSuEngineKadb   \
       -lSuEngineKDB   \
       -lSuEngineKingbase   \
       -lSuEngineMapWorldMaps   \
       -lSuEngineMySQL   \
       -lSuEngineODBC   \
       -lSuEngineOGC   \
       -lSuEngineOGDC   \
       -lSuEngineOpenStreetMaps   \
       -lSuEngineOracle   \
       -lSuEnginePG   \
       -lSuEnginePGis   \
       -lSuEngineRest   \
       -lSuEngineSCV   \
       -lSuEngineSDE   \
       -lSuEngineSMCloud   \
       -lSuEngineSpatialite   \
       -lSuEngineSQLPLUS   \
       -lSuEngineSRDB   \
       -lSuEngineSsp   \
       -lSuEngineUDB   \
       -lSuFileParser   \
       -lSuFileParser3DModel   \
       -lSuFileParser3ds   \
       -lSuFileParserAcad   \
       -lSuFileParserAiGrid   \
       -lSuFileParserBGRID   \
       -lSuFileParserBMP   \
       -lSuFileParserCSV   \
       -lSuFileParserDEM   \
       -lSuFileParserE00   \
       -lSuFileParserECW   \
       -lSuFileParserENC   \
       -lSuFileParserFileGDB   \
       -lSuFileParserFME   \
       -lSuFileParserGDAL   \
       -lSuFileParserGRIB   \
       -lSuFileParserGrid   \
       -lSuFileParserGTiff   \
       -lSuFileParserHFA   \
       -lSuFileParserKML   \
       -lSuFileParserLidar   \
       -lSuFileParserMAPGIS   \
       -lSuFileParserMitab   \
       -lSuFileParserMrSID   \
       -lSuFileParserPointCloud   \
       -lSuFileParserRAW   \
       -lSuFileParserSCV   \
       -lSuFileParserSGM   \
       -lSuFileParserShape   \
       -lSuFileParserSTET   \
       -lSuFileParserTEMS   \
       -lSuFileParserVRT   \
       -lSuFilePyramid   \
       -lSuFilePyramidECW   \
       -lSuFilePyramidGDAL   \
       -lSuFilePyramidMrSid   \
       -lSuFilePyramidSIT   \
       -lSuFMELicense   \
       -lSuGBaseCI   \
       -lSuGeneralization   \
       -lSuGeoCode   \
       -lSuGeometricNetwork   \
       -lSuGeometry   \
       -lSuGeometry3D   \
       -lSuGeometryCAD   \
       -lSuGeometryConverter   \
       -lSuGeometryLayout   \
       -lSuGeometryPlot   \
       -lSuGeoOperation   \
       -lSuGeoOperation3D   \
       -lSuGraphics   \
       -lSuGraphics3D   \
       -lSuGraphics3DOGRE   \
       -lSuGraphicsPDF   \
       -lSuGraphicsPS   \
       -lSuGraphicsW   \
       -lSuGridAnalyst   \
       -lSuGridModeling   \
       -lSuInterNationalICU   \
       -lSuKDBCI   \
       -lSuLayer3DDataset   \
       -lSuLayer3DFile   \
       -lSuLayer3DMap   \
       -lSuLayer3DTree   \
       -lSuLayoutEditor   \
       -lSuLinearReference   \
       -lSuLogistics   \
       -lSuMap   \
       -lSuMapEditor   \
       -lSuMapServiceStub   \
       -lSuMongoDBCI   \
       -lSuMosaic   \
       -lSuMySQLCI   \
       -lSuNetToolkit   \
       -lSuNetworkBuilder   \
       -lSuNetworkEnvironment   \
       -lSuNTopoDataCompiler   \
       -lSuNTopoIndexTree   \
       -lSuODBCCI   \
       -lSuOGCParser   \
       -lSuOGDC   \
       -lSuOracleCI   \
       -lSuOverlay   \
       -lSuParticleSystem   \
       -lSuPathAnalyst   \
       -lSuPathAnalystCH   \
       -lSuPathNavi   \
       -lSuPGSQLCI   \
       -lSuPluginCommon   \
       -lSuPluginGECache   \
       -lSuPluginGlobalCache   \
       -lSuPluginGridCache   \
       -lSuPluginMapCache   \
       -lSuPluginMapCache50   \
       -lSuPluginWebCache   \
       -lSuProjection   \
       -lSuProximity   \
       -lSuPublicTransport   \
       -lSuRectify   \
       -lSuRender   \
       -lSuRepresentation   \
       -lSuRStarTree   \
       -lSuScene   \
       -lSuSceneEditor   \
       -lSuSDECI   \
       -lSuSpatialIndex   \
       -lSuSpatialQuery   \
       -lSuSQLiteCI   \
       -lSuSRDBCI   \
       -lSuStream   \
       -lSuSymbol   \
       -lSuSymbolExchange   \
       -lSuSymbolMarker3D   \
       -lSuTheme3DBase   \
       -lSuTileStorage   \
       -lSuToolkit   \
       -lSuTopoBase   \
       -lSuTopoBuilder   \
       -lSuWorkspace   \
    }
}

unix:{
    # 16bit wchar
    QMAKE_CXXFLAGS =-fshort-wchar

    LIBS +=-L$${SUOBJECTSDIR}/bin/bin \
       -lSuAlgorithm   \
       -lSuAlgorithm3D   \
       -lSuAllocation   \
       -lSuAnimation   \
       -lSuBase   \
       -lSuBase3D   \
       -lSuBeyonDBCI   \
       -lSuBGDataCompiler   \
       -lSuBPlusTree   \
       -lSuCacheBuilder   \
       -lSuCacheFile   \
       -lSuChartBase   \
       -lSuChartToolkit   \
       -lSuCompactFile   \
       -lSuDataCheck   \
       -lSuDataExchange   \
       -lSuDB2CI   \
       -lSuDCPackager   \
       -lSuDCToolkits   \
       -lSuDrawing   \
       -lSuDrawing3D   \
       -lSuDrawingLayout   \
       -lSuDRDSCI   \
       -lSuElement   \
       -lSuEngine   \
       -lSuEngineBD   \
       -lSuEngineDataServer   \
       -lSuEngineDB2   \
       -lSuEngineES   \
       -lSuEngineExtendFile   \
       -lSuEngineHWPG   \
       -lSuEngineImagePlugin   \
       -lSuEngineKDB   \
       -lSuEngineODBC   \
       -lSuEngineOGDC   \
       -lSuEngineOracle   \
       -lSuEngineOsp   \
       -lSuEnginePG   \
       -lSuEnginePGis   \
       -lSuEngineSpatialite   \
       -lSuEngineUDB   \
       -lSuFileParser   \
       -lSuFileParser3ds   \
       -lSuFileParserBGRID   \
       -lSuFileParserCSV   \
       -lSuFileParserEGC   \
       -lSuFileParserGDAL   \
       -lSuFileParserGrid   \
       -lSuFileParserGTiff   \
       -lSuFileParserHFA   \
       -lSuFileParserMitab   \
       -lSuFileParserRAW   \
       -lSuFileParserSCV   \
       -lSuFileParserSGM   \
       -lSuFileParserShape   \
       -lSuFilePyramid   \
       -lSuFMELicense   \
       -lSuGeneralization   \
       -lSuGeoCode   \
       -lSuGeometricNetwork   \
       -lSuGeometry   \
       -lSuGeometry3D   \
       -lSuGeometryCAD   \
       -lSuGeometryConverter   \
       -lSuGeometryLayout   \
       -lSuGeometryPlot   \
       -lSuGeoOperation   \
       -lSuGPKG   \
       -lSuGraphics   \
       -lSuGraphics3D   \
       -lSuGridAnalyst   \
       -lSuGridModeling   \
       -lSuInterNationalICU   \
       -lSuKDBCI   \
       -lSuLayer3DDataset   \
       -lSuLayer3DFile   \
       -lSuLayer3DGeImage   \
       -lSuLayer3DMap   \
       -lSuLayer3DTree   \
       -lSuLayoutEditor   \
       -lSuLinearReference   \
       -lSuLogistics   \
       -lSuMap   \
       -lSuMapEditor   \
       -lSuMapServiceStub   \
       -lSuMongoCI   \
       -lSuMosaic   \
       -lSuMySQLCI   \
       -lSuNetToolkit   \
       -lSuNetworkBuilder   \
       -lSuNetworkEnvironment   \
       -lSuNTopoDataCompiler   \
       -lSuNTopoIndexTree   \
       -lSuODBCCI   \
       -lSuOGCParser   \
       -lSuOGDC   \
       -lSuOracleCI   \
       -lSuOverlay   \
       -lSuParticleSystem   \
       -lSuPathAnalyst   \
       -lSuPathAnalystCH   \
       -lSuPathNavi   \
       -lSuPluginCommon   \
       -lSuPluginGECache   \
       -lSuPluginGlobalCache   \
       -lSuPluginMapCache   \
       -lSuPluginMapCache50   \
       -lSuPluginWebCache   \
       -lSuPostgreSQLCI   \
       -lSuProjection   \
       -lSuProximity   \
       -lSuPublicTransport   \
       -lSuRectify   \
       -lSuRender   \
       -lSuRepresentation   \
       -lSuRStarTree   \
       -lSuScene   \
       -lSuSceneEditor   \
       -lSuSheetJoin   \
       -lSuSpatialIndex   \
       -lSuSpatialQuery   \
       -lSuSQLiteCI   \
       -lSuStream   \
       -lSuSymbol   \
       -lSuSymbolExchange   \
       -lSuSymbolMarker3D   \
       -lSuTheme3DBase   \
       -lSuTileStorage   \
       -lSuTileStorageGPKG   \
       -lSuTileStorageMongo   \
       -lSuToolkit   \
       -lSuTopoBase   \
       -lSuTopoBuilder   \
       -lSuUniversalCI   \
       -lSuUOracleCI   \
       -lSuWorkspace   \
}

    # Libs of SuObjectsCpp
    LIBS += -L$${DESTDIR} \
            -lData


# Module source files
include(Mapping.pri)
