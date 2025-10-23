# Nom du projet
QT       += core gui widgets sql
QT       += sql
CONFIG += c++11
CONFIG += warn_on
CONFIG += embed_translations
LRELEASE = lrelease

# Sources
SOURCES += \
    form/Entrepot.cpp \
    form/FormAjouterFournisseur.cpp \
    form/FormBonLivraison.cpp \
    form/FormListeFournisseur.cpp \
    form/ViewInventaire.cpp \
    form/ViewStock.cpp \
    main.cpp \
    mainwindow.cpp \
    database/database.cpp \
    form/FormEntrepot.cpp \
    form/FormLivraison.cpp \
    form/FormMarchandise.cpp \
    models/fournisseur.cpp \
    models/livraison.cpp \
    models/marchandise.cpp \
    models/stock.cpp

# Headers
HEADERS += \
     Entrepotmanager.h \
    form/Entrepot.h \
    form/FormAjouterFournisseur.h \
    form/FormBonLivraison.h \
    form/FormListeFournisseur.h \
    form/ViewInventaire.h \
    form/ViewStock.h \
    mainwindow.h \
    database/database.h \
    form/FormEntrepot.h \
    form/FormLivraison.h \
    form/FormMarchandise.h \
    models/entrepot.h \
    models/fournisseur.h \
    models/livraison.h \
    models/marchandise.h \
    models/stock.h

# UI files
FORMS += \
    form/Entrepot.ui \
    form/FormAjouterFournisseur.ui \
    form/FormListeFournisseur.ui \
    form/ViewInventaire.ui \
    form/ViewStock.ui \
    mainwindow.ui \
    form/FormBonLivraison.ui \
    form/FormEntrepot.ui \
    form/FormLivraison.ui \
    form/FormMarchandise.ui

# Translations
TRANSLATIONS += Gestion_achats_fr_FR.ts

# Deployment (Linux / Unix)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Ressources
DISTFILES += \
    image000/logo00.jpg \
    image000/logo01.png \
    schema.sql

RESOURCES += resources.qrc \
    resources.qrc

# UI files
FORMS += \
    form/Entrepot.ui \
    form/FormAjouterFournisseur.ui \
    form/FormListeFournisseur.ui \
    form/ViewInventaire.ui \
    form/ViewStock.ui \
    mainwindow.ui \
    form/FormBonLivraison.ui \
    form/FormEntrepot.ui \
    form/FormLivraison.ui \
    form/FormMarchandise.ui

# Translations
TRANSLATIONS += Gestion_achats_fr_FR.ts

# Deployment (Linux / Unix)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Ressources
DISTFILES += \
    image000/logo00.jpg \
    image000/logo01.png \
    schema.sql

RESOURCES += resources.qrc \
    resources.qrc
