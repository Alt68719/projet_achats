#include "ViewStock.h"
#include "ui_ViewStock.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

ViewStock::ViewStock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewStock)
{
    ui->setupUi(this);

    model = new QSqlQueryModel(this);
    chargerStock();

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

ViewStock::~ViewStock()
{
    delete ui;
}

void ViewStock::chargerStock()
{
    // 1. Récupérer tous les entrepôts
    QSqlQuery query(Database::db);
    if(!query.exec("SELECT ref_entrepot FROM entrepot")) {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer les entrepôts:\n" + query.lastError().text());
        return;
    }

    QStringList entrepots;
    while(query.next())
        entrepots << query.value(0).toString();

    if(entrepots.isEmpty()) {
        QMessageBox::information(this, "Info", "Aucun entrepôt trouvé !");
        return;
    }

    // 2. Construire la requête SQL dynamique
    QStringList colonnes;
    colonnes << "m.ref_mar AS 'Réf Marchandise'" << "m.design AS 'Désignation'";
    for(const QString &e : entrepots){
        colonnes << QString("(SELECT s.qte_stock FROM stocker s WHERE s.ref_mar = m.ref_mar AND s.ref_entrepot = '%1') AS '%1'").arg(e);
    }

    QString queryStr = QString("SELECT %1 FROM marchandise m").arg(colonnes.join(", "));

    model->setQuery(queryStr, Database::db);

    if(model->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger le stock:\n" + model->lastError().text());
    }
}
