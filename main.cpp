#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QPushButton>
#include "database.h"
#include "database.cpp"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!Database::connect()){
            qDebug() << "Impossible de se connecter a la base";
            return -1;
        }
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Gestion_achats_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
