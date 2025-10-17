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

    // Configurer le QTableView
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

ViewInventaire::~ViewInventaire()
{
    delete ui;
}

void ViewInventaire::on_btnAfficher_clicked()
{
    QDate date = ui->dateEdit->date();

    QSqlQuery query(Database::db);
    query.prepare("SELECT * FROM view_inventaire WHERE date_jour = ?");
    query.addBindValue(date);

    if(!query.exec()){
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer l'inventaire:\n" + query.lastError().text());
        return;
    }

    model->setQuery(query);

    if(model->lastError().isValid()){
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'affichage:\n" + model->lastError().text());
    }
}
