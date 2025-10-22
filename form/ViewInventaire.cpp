#include "ViewInventaire.h"
#include "ui_ViewInventaire.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

ViewInventaire::ViewInventaire(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewInventaire)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // Rafraîchir automatiquement au lancement
    rafraichirVue();
}

ViewInventaire::~ViewInventaire()
{
    delete ui;
}

void ViewInventaire::rafraichirVue()
{
    QSqlQuery query(Database::db);

    // Vérifie si la vue existe (sécurité)
    if (!query.exec("SELECT name FROM sqlite_master WHERE type='view' AND name='inventaire';")) {
        qCritical() << "Erreur lors de la vérification de la vue inventaire :" << query.lastError().text();
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Vue manquante",
                             "La vue 'inventaire' n'existe pas dans la base de données.");
        return;
    }

    qDebug() << "Vue inventaire prête à être utilisée.";
}

void ViewInventaire::on_btnAfficher_clicked()
{
    QString dateString = ui->dateEdit->date().toString("yyyy-MM-dd");

    QSqlQuery query(Database::db);

    // ✅ Nouvelle requête plus robuste : on s'assure que même sans date, ça affiche tout
    QString sql = R"(
        SELECT
            s.ref_mar AS "Réf Marchandise",
            m.design AS "Désignation",
            s.ref_entrepot AS "Réf Entrepôt",
            e.adresse AS "Adresse Entrepôt",
            COALESCE(l.date_de_livraison, '---') AS "Date Livraison",
            s.qte_stock AS "Quantité Stock"
        FROM stocker AS s
        JOIN marchandise AS m ON s.ref_mar = m.ref_mar
        JOIN entrepot AS e ON s.ref_entrepot = e.ref_entrepot
        LEFT JOIN livrer AS l ON s.ref_mar = l.ref_mar AND s.ref_entrepot = l.ref_entrepot
        WHERE date(l.date_de_livraison) = date(:date)
           OR :date IS NULL
           OR :date = ''
        ORDER BY e.adresse, m.design;
    )";

    query.prepare(sql);
    query.bindValue(":date", dateString);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur SQL",
                             "Impossible de récupérer l'inventaire :\n" + query.lastError().text());
        qCritical() << "Erreur SQL dans inventaire :" << query.lastError().text();
        return;
    }

    model->setQuery(query);

    if (model->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'affichage :\n" + model->lastError().text());
        qCritical() << "Erreur modèle inventaire :" << model->lastError().text();
        return;
    }

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Aucun résultat",
                                 "Aucun inventaire trouvé pour la date " + dateString);
    } else {
        qDebug() << "Inventaire chargé avec" << model->rowCount() << "lignes.";
    }
}
