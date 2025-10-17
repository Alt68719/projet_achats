#include "database.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

QSqlDatabase Database::db = QSqlDatabase::addDatabase("QSQLITE");

bool Database::ouvrir() {
    db.setDatabaseName("gestion_achats.db");
    if(!db.open()) {
        qDebug() << "Erreur ouverture DB:" << db.lastError().text();
        return false;
    }
    qDebug() << "Base ouverte avec succès!";
    return true;
}

void Database::fermer() {
    db.close();
}
