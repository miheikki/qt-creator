shared {
    DEFINES += CLANGSUPPORT_BUILD_LIB
} else {
    DEFINES += CLANGSUPPORT_BUILD_STATIC_LIB
}

QT += network

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/cancelmessage.cpp \
    $$PWD/clangsupportdebugutils.cpp \
    $$PWD/clangcodemodelclientinterface.cpp \
    $$PWD/clangcodemodelclientproxy.cpp \
    $$PWD/clangcodemodelconnectionclient.cpp \
    $$PWD/clangcodemodelserverinterface.cpp \
    $$PWD/clangcodemodelserverproxy.cpp \
    $$PWD/alivemessage.cpp \
    $$PWD/completionsmessage.cpp \
    $$PWD/requestcompletionsmessage.cpp \
    $$PWD/echomessage.cpp \
    $$PWD/endmessage.cpp \
    $$PWD/documentsopenedmessage.cpp \
    $$PWD/documentsclosedmessage.cpp \
    $$PWD/codecompletionchunk.cpp \
    $$PWD/codecompletion.cpp \
    $$PWD/connectionclient.cpp \
    $$PWD/connectionserver.cpp \
    $$PWD/diagnosticcontainer.cpp \
    $$PWD/annotationsmessage.cpp \
    $$PWD/dynamicastmatcherdiagnosticcontainer.cpp \
    $$PWD/dynamicastmatcherdiagnosticcontextcontainer.cpp \
    $$PWD/dynamicastmatcherdiagnosticmessagecontainer.cpp \
    $$PWD/filecontainer.cpp \
    $$PWD/filecontainerv2.cpp \
    $$PWD/filepath.cpp \
    $$PWD/fixitcontainer.cpp \
    $$PWD/followsymbolmessage.cpp \
    $$PWD/lineprefixer.cpp \
    $$PWD/messageenvelop.cpp \
    $$PWD/pchmanagerclientinterface.cpp \
    $$PWD/pchmanagerclientproxy.cpp \
    $$PWD/pchmanagerserverinterface.cpp \
    $$PWD/pchmanagerserverproxy.cpp \
    $$PWD/precompiledheadersupdatedmessage.cpp \
    $$PWD/projectpartpch.cpp \
    $$PWD/readmessageblock.cpp \
    $$PWD/refactoringclientinterface.cpp \
    $$PWD/refactoringclientproxy.cpp \
    $$PWD/refactoringserverinterface.cpp \
    $$PWD/refactoringserverproxy.cpp \
    $$PWD/referencesmessage.cpp \
    $$PWD/unsavedfilesupdatedmessage.cpp \
    $$PWD/requestannotationsmessage.cpp \
    $$PWD/requestfollowsymbolmessage.cpp \
    $$PWD/requestreferencesmessage.cpp \
    $$PWD/requestsourcelocationforrenamingmessage.cpp \
    $$PWD/requestsourcerangesanddiagnosticsforquerymessage.cpp \
    $$PWD/requestsourcerangesforquerymessage.cpp \
    $$PWD/requesttooltipmessage.cpp \
    $$PWD/sourcelocationcontainer.cpp \
    $$PWD/sourcelocationcontainerv2.cpp \
    $$PWD/sourcelocationscontainer.cpp \
    $$PWD/sourcelocationsforrenamingmessage.cpp \
    $$PWD/sourcerangecontainer.cpp \
    $$PWD/processcreator.cpp \
    $$PWD/processexception.cpp \
    $$PWD/processstartedevent.cpp \
    $$PWD/sourcerangecontainerv2.cpp \
    $$PWD/sourcerangesanddiagnosticsforquerymessage.cpp \
    $$PWD/sourcerangescontainer.cpp \
    $$PWD/sourcerangesforquerymessage.cpp \
    $$PWD/sourcerangewithtextcontainer.cpp \
    $$PWD/tokeninfocontainer.cpp \
    $$PWD/tooltipmessage.cpp \
    $$PWD/tooltipinfo.cpp \
    $$PWD/unsavedfilesremovedmessage.cpp \
    $$PWD/updateprojectpartsmessage.cpp \
    $$PWD/documentschangedmessage.cpp \
    $$PWD/documentvisibilitychangedmessage.cpp \
    $$PWD/writemessageblock.cpp \
    $$PWD/filepathcaching.cpp \
    $$PWD/filepathid.cpp \
    $$PWD/baseserverproxy.cpp \
    $$PWD/updategeneratedfilesmessage.cpp \
    $$PWD/removegeneratedfilesmessage.cpp \
    $$PWD/generatedfiles.cpp \
    $$PWD/projectpartartefact.cpp \
    $$PWD/projectpartcontainer.cpp

HEADERS += \
    $$PWD/cancelmessage.h \
    $$PWD/clangpathwatcher.h \
    $$PWD/clangpathwatcherinterface.h \
    $$PWD/clangpathwatchernotifier.h \
    $$PWD/clangsupportdebugutils.h \
    $$PWD/clangsupport_global.h \
    $$PWD/clangcodemodelclientinterface.h \
    $$PWD/clangcodemodelclientmessages.h \
    $$PWD/clangcodemodelclientproxy.h \
    $$PWD/clangcodemodelconnectionclient.h \
    $$PWD/clangcodemodelserverinterface.h \
    $$PWD/clangcodemodelservermessages.h \
    $$PWD/clangcodemodelserverproxy.h \
    $$PWD/clangrefactoringclientmessages.h \
    $$PWD/clangrefactoringmessages.h \
    $$PWD/clangrefactoringservermessages.h \
    $$PWD/alivemessage.h \
    $$PWD/clangsupportexceptions.h \
    $$PWD/completionsmessage.h \
    $$PWD/executeinloop.h \
    $$PWD/pchpaths.h \
    $$PWD/projectpartid.h \
    $$PWD/projectpartsstorage.h \
    $$PWD/projectpartsstorageinterface.h \
    $$PWD/requestcompletionsmessage.h \
    $$PWD/echomessage.h \
    $$PWD/endmessage.h \
    $$PWD/documentsopenedmessage.h \
    $$PWD/documentsclosedmessage.h \
    $$PWD/codecompletionchunk.h \
    $$PWD/codecompletion.h \
    $$PWD/connectionclient.h \
    $$PWD/connectionserver.h \
    $$PWD/diagnosticcontainer.h \
    $$PWD/annotationsmessage.h \
    $$PWD/dynamicastmatcherdiagnosticcontainer.h \
    $$PWD/dynamicastmatcherdiagnosticcontextcontainer.h \
    $$PWD/dynamicastmatcherdiagnosticmessagecontainer.h \
    $$PWD/dynamicmatcherdiagnostics.h \
    $$PWD/filecontainer.h \
    $$PWD/filecontainerv2.h \
    $$PWD/fixitcontainer.h \
    $$PWD/followsymbolmessage.h \
    $$PWD/idpaths.h \
    $$PWD/ipcclientinterface.h \
    $$PWD/ipcinterface.h \
    $$PWD/ipcserverinterface.h \
    $$PWD/lineprefixer.h \
    $$PWD/messageenvelop.h \
    $$PWD/pchmanagerclientinterface.h \
    $$PWD/pchmanagerclientproxy.h \
    $$PWD/pchmanagerserverinterface.h \
    $$PWD/pchmanagerserverproxy.h \
    $$PWD/precompiledheadersupdatedmessage.h \
    $$PWD/projectpartpch.h \
    $$PWD/readmessageblock.h \
    $$PWD/refactoringclientinterface.h \
    $$PWD/refactoringclientproxy.h \
    $$PWD/refactoringserverinterface.h \
    $$PWD/refactoringserverproxy.h \
    $$PWD/referencesmessage.h \
    $$PWD/unsavedfilesupdatedmessage.h \
    $$PWD/removeprojectpartsmessage.h \
    $$PWD/requestannotationsmessage.h \
    $$PWD/requestfollowsymbolmessage.h \
    $$PWD/requestreferencesmessage.h \
    $$PWD/requestsourcelocationforrenamingmessage.h \
    $$PWD/requestsourcerangesanddiagnosticsforquerymessage.h \
    $$PWD/requestsourcerangesforquerymessage.h \
    $$PWD/requesttooltipmessage.h \
    $$PWD/sourcelocationcontainer.h \
    $$PWD/sourcelocationcontainerv2.h \
    $$PWD/sourcelocationscontainer.h \
    $$PWD/sourcelocationsforrenamingmessage.h \
    $$PWD/sourcerangecontainer.h \
    $$PWD/filepath.h \
    $$PWD/processcreator.h \
    $$PWD/processexception.h \
    $$PWD/processhandle.h \
    $$PWD/processstartedevent.h \
    $$PWD/sourcerangecontainerv2.h \
    $$PWD/sourcerangesanddiagnosticsforquerymessage.h \
    $$PWD/sourcerangescontainer.h \
    $$PWD/sourcerangesforquerymessage.h \
    $$PWD/sourcerangewithtextcontainer.h \
    $$PWD/stringcache.h \
    $$PWD/tokeninfocontainer.h \
    $$PWD/tooltipmessage.h \
    $$PWD/tooltipinfo.h \
    $$PWD/unsavedfilesremovedmessage.h \
    $$PWD/updateprojectpartsmessage.h \
    $$PWD/documentschangedmessage.h \
    $$PWD/documentvisibilitychangedmessage.h \
    $$PWD/writemessageblock.h \
    $$PWD/ipcclientprovider.h \
    $$PWD/requestsourcerangesforquerymessage.h \
    $$PWD/stringcachefwd.h \
    $$PWD/stringcachealgorithms.h \
    $$PWD/projectmanagementserverinterface.h \
    $$PWD/refactoringdatabaseinitializer.h \
    $$PWD/filepathcache.h \
    $$PWD/filepathid.h \
    $$PWD/filepathstorage.h \
    $$PWD/filepathstoragesqlitestatementfactory.h \
    $$PWD/filepathstoragesources.h \
    $$PWD/filepathexceptions.h \
    $$PWD/filepathcachinginterface.h \
    $$PWD/filepathcaching.h \
    $$PWD/filepathcachingfwd.h \
    $$PWD/baseserverproxy.h \
    $$PWD/filepath.h \
    $$PWD/nativefilepath.h \
    $$PWD/filepathview.h \
    $$PWD/compilermacro.h \
    $$PWD/updategeneratedfilesmessage.h \
    $$PWD/removegeneratedfilesmessage.h \
    $$PWD/generatedfiles.h \
    $$PWD/generatedfilesinterface.h \
    $$PWD/progressmessage.h \
    $$PWD/progresscounter.h \
    $$PWD/includesearchpath.h \
    $$PWD/commandlinebuilder.h \
    $$PWD/projectpartartefact.h \
    $$PWD/projectpartcontainer.h \
    $$PWD/sourceentry.h \
    $$PWD/modifiedtimecheckerinterface.h \
    $$PWD/environment.h \
    $$PWD/modifiedtimechecker.h

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols
