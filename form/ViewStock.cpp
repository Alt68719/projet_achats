#include "ViewStock.h"
#include "ui_ViewStock.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQueryModel>

ViewStock::ViewStock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewStock),
    model(new QSqlQueryModel(this)),
    modelMouvements(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    // Charger le stock
    chargerStock();
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // Charger les mouvements (groupés par désignation + entrepôt)
    chargerMouvements();
    ui->tableViewMouvements->setModel(modelMouvements);
    ui->tableViewMouvements->resizeColumnsToContents();
    ui->tableViewMouvements->horizontalHeader()->setStretchLastSection(true);

    if (ui->btnSupprimer) {
        connect(ui->btnSupprimer, &QPushButton::clicked, this, &ViewStock::supprimerStockSelectionne);
    }
}

ViewStock::~ViewStock()
{
    delete ui;
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
            s.qte_stock AS 'Qté stock'
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
    }
}

void ViewStock::chargerMouvements()
{
    if (!Database::db.isOpen()) {
        QMessageBox::warning(this, "Erreur", "La base de données n'est pas ouverte.");
        return;
    }

    // Group by design + entrepot.
    // last_qte_livre: take the qte_livre of the most recent livrer row for any ref_mar whose design = m.design
    // and for the same ref_entrepot.
    QString queryStr = R"(
        SELECT
            m.design AS 'Désignation',
            (IFNULL(SUM(s.qte_stock),0) - IFNULL((
                SELECT l2.qte_livre
                FROM livrer l2
                JOIN marchandise m2 ON l2.ref_mar = m2.ref_mar
                WHERE m2.design = m.design
                  AND l2.ref_entrepot = s.ref_entrepot
                ORDER BY l2.date_de_livraison DESC, l2.num_livraison DESC
                LIMIT 1
            ), 0)) AS 'Ancienne qte stock',
            IFNULL(SUM(s.qte_stock),0) AS 'Nouvelle qte stock',
            s.ref_entrepot AS 'Réf Entrepôt'
        FROM stocker s
        JOIN marchandise m ON s.ref_mar = m.ref_mar
        GROUP BY m.design, s.ref_entrepot
        ORDER BY s.ref_entrepot, m.design
    )";

    modelMouvements->setQuery(queryStr, Database::db);

    if (modelMouvements->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur",
                             "Impossible de charger les mouvements :\n" +
                             modelMouvements->lastError().text());
        qWarning() << "Erreur SQL ViewStock (mouvements):" << modelMouvements->lastError().text();
    }
}

void ViewStock::supprimerStockSelectionne()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un stock à supprimer !");
        return;
    }

    QString refEntrepot = model->data(model->index(index.row(), 0)).toString();
    QString refMar = model->data(model->index(index.row(), 1)).toString();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
        "Voulez-vous vraiment supprimer ce stock ?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query(Database::db);
        query.prepare("DELETE FROM stocker WHERE ref_entrepot = ? AND ref_mar = ?");
        query.addBindValue(refEntrepot);
        query.addBindValue(refMar);

        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur",
                                  "Impossible de supprimer le stock :\n" +
                                  query.lastError().text());
            return;
        }

        // Recharger les tableaux
        chargerStock();
        chargerMouvements();
    }
}
