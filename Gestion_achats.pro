0QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    database/database.cpp \
    entrepot.cpp \
    entrepotform.cpp \
    fournisseur.cpp \
    fournisseurform.cpp \
    livraison.cpp \
    livraisonform.cpp \
    main.cpp \
    mainwindow.cpp \
    marchandise.cpp \
    marchandiseform.cpp \
    models/entrepot.cpp \
    models/fournisseur.cpp \
    models/livraison.cpp \
    models/marchandise.cpp \
    models/stock.cpp \
    stock.cpp \
    stockmaj.cpp

HEADERS += \
    database/database.h \
    entrepot.h \
    fournisseur.h \
    livraison.h \
    mainwindow.h \
    marchandise.h \
    model/entrepot.h \
    model/fournisseur.h \
    model/livraison.h \
    model/marchandise.h \
    model/stock.h \
    stock.h

FORMS += \
    fornisseur.ui \
    mainwindow.ui

TRANSLATIONS += \
    Gestion_achats_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations
QT += core gui widgets sql
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    image000/logo00.jpg \
    image000/logo01.png
