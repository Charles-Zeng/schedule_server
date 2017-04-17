TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    logger/logger.cpp \
    project/main.cpp \
    common/commonFunction.cpp \
    database/DBAdaptor.cpp \
    dataLayer/dataLayer.cpp \
    dataQueue/dataQueue.cpp \
    httpSvr/httpSvr.cpp \
    json/jsonParser.cpp \
    processor/processAddGroup.cpp \
    processor/processAddTemplate.cpp \
    processor/processDelGroup.cpp \
    processor/processDelTemplate.cpp \
    processor/processOneToN.cpp \
    processor/processOneToOne.cpp \
    processor/processorManager.cpp \
    processor/processUploadImage.cpp \
    service/sysService.cpp \
    syscfg/cfgReader.cpp \
    syscfg/sysConfig.cpp \
    templateServer/templateServerProxy.cpp \
    tool/base64/base64.cpp \
    json/jsonBuilder.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    global/cmdParse.h \
    global/shareMem.h \
    logger/logger.h \
    tool/synclist.h \
    3rdparty/occi/include/ldap.h \
    3rdparty/occi/include/nzerror.h \
    3rdparty/occi/include/nzt.h \
    3rdparty/occi/include/occi.h \
    3rdparty/occi/include/occiAQ.h \
    3rdparty/occi/include/occiCommon.h \
    3rdparty/occi/include/occiControl.h \
    3rdparty/occi/include/occiData.h \
    3rdparty/occi/include/occiObjects.h \
    3rdparty/occi/include/oci.h \
    3rdparty/occi/include/oci1.h \
    3rdparty/occi/include/oci8dp.h \
    3rdparty/occi/include/ociap.h \
    3rdparty/occi/include/ociapr.h \
    3rdparty/occi/include/ocidef.h \
    3rdparty/occi/include/ocidem.h \
    3rdparty/occi/include/ocidfn.h \
    3rdparty/occi/include/ociextp.h \
    3rdparty/occi/include/ocikpr.h \
    3rdparty/occi/include/ocixmldb.h \
    3rdparty/occi/include/ocixstream.h \
    3rdparty/occi/include/odci.h \
    3rdparty/occi/include/oratypes.h \
    3rdparty/occi/include/ori.h \
    3rdparty/occi/include/orid.h \
    3rdparty/occi/include/orl.h \
    3rdparty/occi/include/oro.h \
    3rdparty/occi/include/ort.h \
    3rdparty/occi/include/xa.h \
    common/commonFunction.h \
    common/dataStruct.h \
    database/DBAdaptor.h \
    dataLayer/dataLayer.h \
    dataQueue/dataQueue.h \
    global/config.h \
    httpSvr/httpSvr.h \
    json/jsonParser.h \
    processor/processAddGroup.h \
    processor/processAddTemplate.h \
    processor/processDelGroup.h \
    processor/processDelTemplate.h \
    processor/processOneToN.h \
    processor/processOneToOne.h \
    processor/processor.h \
    processor/processorManager.h \
    processor/processUploadImage.h \
    service/sysService.h \
    syscfg/cfgReader.h \
    syscfg/sysConfig.h \
    templateServer/templateServerProxy.h \
    tool/base64/base64.h \
    json/jsonBuilder.h

