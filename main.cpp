#include "mainwindow.h"
#include <QApplication>
#include "database/database.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //if (!Database::ouvrir()) {
        //return -1; // Impossible d’ouvrir la base
    //}
    if (!Database::ouvrir()) { // 2. Tenter d'ouvrir la connexion
        // Afficher l'erreur et quitter si l'ouverture échoue
        QMessageBox::critical(nullptr, "Erreur Fatale", "Impossible de se connecter à la base de données.");
        return -1;
    }

    Database::creerTables();

    MainWindow w;
    w.show();
    return a.exec();
}
