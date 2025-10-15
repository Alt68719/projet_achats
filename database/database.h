#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSettings>

class Database {
public:
    static QSqlDatabase db;

    static bool connect() {
        // Lecture du fichier config.ini
        QSettings settings("config.ini", QSettings::IniFormat);

        // Lecture des paramètres avec valeurs par défaut
        QString type = settings.value("database/type", "QSQLITE").toString();
        QString host = settings.value("database/host", "127.0.0.1").toString();
        int port = settings.value("database/port", 3306).toInt();
        QString name = settings.value("database/name", "projetdatabase").toString();
        QString user = settings.value("database/user", "dinart_remote").toString();
        QString password = settings.value("database/password", "Deralaza").toString();

        // Configuration de la base
        db = QSqlDatabase::addDatabase(type);
        db.setHostName(host);
        db.setPort(port);
        db.setDatabaseName(name);
        db.setUserName(user);
        db.setPassword(password);

        // Tentative de connexion
        if (!db.open()) {
            qDebug() << "❌ Erreur de connexion à la base de données :" << db.lastError().text();
            return false;
        }

        qDebug() << "✅ Connexion réussie à la base de données sur" << host;
        return true;
    }
};

#endif // DATABASE_H
