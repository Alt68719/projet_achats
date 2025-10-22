#include "form/FormBonLivraison.h"
#include "ui_FormBonLivraison.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

FormBonLivraison::FormBonLivraison(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormBonLivraison),
    modelBonLivraison(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    chargerBonLivraison();
}

FormBonLivraison::~FormBonLivraison()
{
    delete ui;
}

// Charger les bons de livraison depuis la vue SQL
void FormBonLivraison::chargerBonLivraison()
{
    QSqlQuery query(Database::db);
    query.prepare("SELECT * FROM bon_livraison ORDER BY Date_de_Livraison DESC");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL",
                              "Impossible de charger les bons de livraison :\n" +
                              query.lastError().text());
        return;
    }

    modelBonLivraison->setQuery(std::move(query));
    ui->tableView->setModel(modelBonLivraison);

    // Ajustement de l'affichage
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}
