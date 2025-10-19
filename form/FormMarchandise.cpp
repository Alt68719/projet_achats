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
    connect(ui->btnAjouter, &QPushButton::clicked,
            this, &FormMarchandise::slotAjouterMarchandise);
}

FormMarchandise::~FormMarchandise()
{
    delete ui;
}

void FormMarchandise:: slotAjouterMarchandise()
{
    QString ref_mar = ui->lineEditRef->text();          // CORRIGÉ
    QString design = ui->lineEditDesign->text();    // CORRIGÉ
    double prix_de_achate = ui->lineEditPrix->text().toDouble(); // CORRIGÉ

    QSqlQuery query(Database::db);
    query.prepare("INSERT INTO marchandise(ref_mar, design, prix_de_vente) VALUES (?, ?, ?)");
    query.addBindValue(ref_mar);
    query.addBindValue(design);
    query.addBindValue(prix_de_achate);

    if(!query.exec()){
        QMessageBox::warning(this,"Erreur","Impossible d'ajouter la marchandise:\n"+query.lastError().text());
        return;
    }

    QMessageBox::information(this,"Succès","Marchandise ajoutée !");
    accept();
}
