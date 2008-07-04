INCLUDE_DIRECTORIES(${KDE4_INCLUDES})

SET(
     aku_SRCS
     main.cpp
     mainwindow.cpp
     akumaintable.cpp
     akutip.cpp
     rarprocess.cpp
     zipprocess.cpp
     tarprocess.cpp
     threadprocess.cpp
     akuratiowidget.cpp
     akusearchwidget.cpp
     akusearchline.cpp
     rar.cpp
     zip.cpp
     tar.cpp
     akumetawidget.cpp
     akuviewer.cpp
     overwritedialog.cpp
     akuprogressdialog.cpp
     akuprogresstooltip.cpp
     extractdialog.cpp
     akucomment.cpp
     akuerrordialog.cpp
     quickextract.cpp
   )

KDE4_ADD_UI_FILES(aku_SRCS overwriteui.ui extractdialog.ui)
KDE4_ADD_EXECUTABLE(aku ${aku_SRCS})
TARGET_LINK_LIBRARIES(aku ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS})
INSTALL(TARGETS aku DESTINATION ${BIN_INSTALL_DIR})
INSTALL(FILES akuui.rc 
        DESTINATION ${DATA_INSTALL_DIR}/aku)
INSTALL(FILES akuextract.desktop DESTINATION
        ${SERVICES_INSTALL_DIR}/ServiceMenus)
INSTALL(FILES aku.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
KDE4_INSTALL_ICONS(${ICON_INSTALL_DIR})
