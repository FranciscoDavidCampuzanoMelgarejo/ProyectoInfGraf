#-------------------------------------------------
#
# Project created by QtCreator 2022-09-01T12:00:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QFotoPaint5

TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    ajustelineal.cpp \
    aplicar_morfologia.cpp \
    bajorrelieve.cpp \
    capturarvideo.cpp \
    convertir_color_falso.cpp \
    convolucion.cpp \
    copiar_efectos.cpp \
    dialogo_ecualizacion_local.cpp \
    dialogo_histograma2d.cpp \
    dialogo_inpaint.cpp \
    informacionimagen.cpp \
    mainwindow.cpp \
    imagenes.cpp \
    dialognueva.cpp \
    brillocontraste.cpp \
    mat_sat_lum.cpp \
    perfilado.cpp \
    perspectiva.cpp \
    pinchar_estirar.cpp \
    poner_texto.cpp \
    rojo_verde_azul.cpp \
    rotar_reescalar.cpp \
    suavizados.cpp \
    video.cpp \
    rotaravideo.cpp \
    mediaponderada.cpp \
    acercade.cpp \
    mediadevideo.cpp

HEADERS  += mainwindow.h \
    ajustelineal.h \
    aplicar_morfologia.h \
    bajorrelieve.h \
    capturarvideo.h \
    convertir_color_falso.h \
    convolucion.h \
    copiar_efectos.h \
    dialogo_ecualizacion_local.h \
    dialogo_histograma2d.h \
    dialogo_inpaint.h \
    imagenes.h \
    dialognueva.h \
    brillocontraste.h \
    informacionimagen.h \
    mat_sat_lum.h \
    perfilado.h \
    perspectiva.h \
    pinchar_estirar.h \
    poner_texto.h \
    rojo_verde_azul.h \
    rotar_reescalar.h \
    suavizados.h \
    video.h \
    rotaravideo.h \
    mediaponderada.h \
    acercade.h \
    mediadevideo.h

FORMS    += mainwindow.ui \
    ajustelineal.ui \
    aplicar_morfologia.ui \
    bajorrelieve.ui \
    capturarvideo.ui \
    convertir_color_falso.ui \
    convolucion.ui \
    copiar_efectos.ui \
    dialognueva.ui \
    brillocontraste.ui \
    dialogo_ecualizacion_local.ui \
    dialogo_histograma2d.ui \
    dialogo_inpaint.ui \
    informacionimagen.ui \
    mat_sat_lum.ui \
    perfilado.ui \
    perspectiva.ui \
    pinchar_estirar.ui \
    poner_texto.ui \
    rojo_verde_azul.ui \
    rotar_reescalar.ui \
    suavizados.ui \
    rotaravideo.ui \
    mediaponderada.ui \
    acercade.ui \
    mediadevideo.ui

INCLUDEPATH += "C:\OpenCV\OpenCV4.6.0G\include"

LIBS += -L"C:\OpenCV\OpenCV4.6.0G\lib"\
        -llibopencv_world460

RESOURCES += recursos.qrc

RC_ICONS = imagenes/icono.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
