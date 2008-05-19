 
INCLUDE_DIRECTORIES(${KDE4_INCLUDES}) #${KDE4_INCLUDES} ${QT_USE_FILE}

SET(aku_SRCS
 main.cpp
 rar.cpp
 mainwindow.cpp
 extractdialog.cpp
 dragsource.cpp
 dragtarget.cpp
 akuviewer.cpp
 akusearchline.cpp
 rarprocesshandler.cpp
 overwritedialog.cpp
 akuratiowidget.cpp
 akutooltip.cpp
 akushadeanimation.cpp
 akucompressionwidget.cpp
 akumetawidget.cpp
 akuprogressdialog.cpp
 akusearchwidget.cpp
 akudragsourcewidget.cpp
 akucomment.cpp
 akuwaitdialog.cpp
 akutreewidget.cpp
 threadprocess.cpp
 akuprogresstooltip.cpp
 akuwaitthread.cpp
)

KDE4_ADD_UI_FILES(aku_SRCS extracto.ui overwriteui.ui)
KDE4_ADD_EXECUTABLE(aku ${aku_SRCS})
TARGET_LINK_LIBRARIES(aku ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS})  # ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS}
INSTALL(TARGETS aku DESTINATION ${BIN_INSTALL_DIR})
INSTALL(FILES akuui.rc 
        DESTINATION ${DATA_INSTALL_DIR}/aku)
#INSTALL(FILES aku.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
KDE4_INSTALL_ICONS(${ICON_INSTALL_DIR})
