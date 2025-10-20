#include "ViewStock.h"
#include "ui_ViewStock.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

ViewStock::ViewStock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewStock),
    model(new QSqlQueryModel(this)),
    modelMouvements(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    // Charger le contenu du stock (premier tableau)
    chargerStock();
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // Charger les mouvements récents (deuxième tableau)
    chargerMouvements();
    if (ui->tableViewMouvements) {
        ui->tableViewMouvements->setModel(modelMouvements);
        ui->tableViewMouvements->resizeColumnsToContents();
        ui->tableViewMouvements->horizontalHeader()->setStretchLastSection(true);
    }
}

ViewStock::~ViewStock()
{
    delete ui;
    // Les modèles sont détruits automatiquement (parents Qt)
}

void ViewStock::chargerStock()
{
    if (!Database::db.isOpen()) {
        QMessageBox::warning(this, "Erreur", "La base de données n'est pas ouverte.");
        return;
    }

    QString queryStr = R"(
        SELECT
            s.ref_entrepot AS 'Réf Entrepôt',
            s.ref_mar AS 'Réf Marchandise',
            m.design AS 'Désignation',
            s.qte_stock AS 'Qté livrée'
        FROM stocker s
        JOIN marchandise m ON s.ref_mar = m.ref_mar
        ORDER BY s.ref_entrepot, s.ref_mar
    )";

    model->setQuery(queryStr, Database::db);

    if (model->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur",
                             "Impossible de charger le stock :\n" +
                             model->lastError().text());
        qWarning() << "Erreur SQL ViewStock:" << model->lastError().text();
        return;
    }

    qDebug() << "Requête exécutée dans ViewStock:" << queryStr;
}

void ViewStock::chargerMouvements()
{
    if (!Database::db.isOpen()) {
        QMessageBox::warning(this, "Erreur", "La base de données n'est pas ouverte.");
        return;
    }

    QString queryStr = R"(
SELECT
    m.design AS 'Désignation',
    l.num_livraison AS 'N° Livraison',
    (s.qte_stock - l.qte_livre) AS 'Ancienne qte stock',
    s.qte_stock AS 'Nouvelle qte stock',
    s.ref_entrepot AS 'Réf Entrepôt'
FROM stocker s
JOIN marchandise m ON s.ref_mar = m.ref_mar
JOIN livrer l ON s.ref_mar = l.ref_mar AND s.ref_entrepot = l.ref_entrepot
ORDER BY l.num_livraison DESC, s.ref_entrepot, m.design
    )";

    modelMouvements->setQuery(queryStr, Database::db);

    if (modelMouvements->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur",
            "Impossible de charger les mouvements de stock :\n" +
            modelMouvements->lastError().text());
        qWarning() << "Erreur SQL ViewStock (mouvements):" << modelMouvements->lastError().text();
        return;
    }

    qDebug() << "Requête exécutée dans ViewStock (mouvements):" << queryStr;
}
