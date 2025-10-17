#include "FormMarchandise.h"
#include "ui_FormMarchandise.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

FormMarchandise::FormMarchandise(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormMarchandise)
{
    ui->setupUi(this);
}

FormMarchandise::~FormMarchandise()
{
    delete ui;
}

void FormMarchandise::on_btnAjouter_clicked()
{
    QString ref = ui->lineEditRef->text();          // CORRIGÉ
    QString design = ui->lineEditDesign->text();    // CORRIGÉ
    double prix = ui->lineEditPrix->text().toDouble(); // CORRIGÉ

    QSqlQuery query(Database::db);
    query.prepare("INSERT INTO marchandise(ref_mar, design, prix_vente) VALUES (?, ?, ?)");
    query.addBindValue(ref);
    query.addBindValue(design);
    query.addBindValue(prix);

    if(!query.exec()){
        QMessageBox::warning(this,"Erreur","Impossible d'ajouter la marchandise:\n"+query.lastError().text());
        return;
    }

    QMessageBox::information(this,"Succès","Marchandise ajoutée !");
    accept();
}
