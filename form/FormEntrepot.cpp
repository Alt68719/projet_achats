#include "FormEntrepot.h"
#include "ui_FormEntrepot.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

FormEntrepot::FormEntrepot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormEntrepot)
{
    ui->setupUi(this);
    remplirComboBoxes();
}

FormEntrepot::~FormEntrepot()
{
    delete ui;
}

// -------- Remplir le comboBox des entrepôts --------
void FormEntrepot::remplirComboBoxes()
{
    modelEntrepot = new QSqlQueryModel(this);
    modelEntrepot->setQuery("SELECT ref_entrepot FROM entrepot", Database::db);
    ui->comboBoxEntrepot->setModel(modelEntrepot);
}

// -------- Assigner un entrepôt existant à une marchandise --------
void FormEntrepot::on_btnAjouter_clicked()
{
    QString refEntrepot = ui->comboBoxEntrepot->currentText().trimmed();
    QString refMar = ui->lineEditMarchandise->text().trimmed();

    if(refEntrepot.isEmpty() || refMar.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Veuillez saisir la marchandise et sélectionner un entrepôt.");
        return;
    }

    QSqlQuery query(Database::db);

    // 1️⃣ Vérifier si la marchandise existe
    query.prepare("SELECT COUNT(*) FROM marchandise WHERE ref_mar = ?");
    query.addBindValue(refMar);
    if(!query.exec()){
        QMessageBox::critical(this,"Erreur","Impossible de vérifier la marchandise:\n"+query.lastError().text());
        return;
    }
    query.next();
    int count = query.value(0).toInt();

    // 2️⃣ Si la marchandise n’existe pas, créer automatiquement
    if(count == 0){
        QSqlQuery insertMar(Database::db);
        insertMar.prepare("INSERT INTO marchandise (ref_mar, design, prix_de_vente) VALUES (?, ?, ?)");
        insertMar.addBindValue(refMar);
        insertMar.addBindValue("Design par défaut"); // tu peux changer
        insertMar.addBindValue(0.0);                 // prix par défaut
        if(!insertMar.exec()){
            QMessageBox::critical(this,"Erreur","Impossible de créer la marchandise:\n"+insertMar.lastError().text());
            return;
        }
    }

    // 3️⃣ Générer un numéro d’opération unique
    QString numOperat = "OP" + QString::number(QDateTime::currentSecsSinceEpoch());

    // 4️⃣ Assigner la marchandise à l’entrepôt
    QSqlQuery insertStock(Database::db);
    insertStock.prepare(R"(
        INSERT OR REPLACE INTO stocker (num_operat, ref_mar, ref_entrepot, qte_stock)
        VALUES (?, ?, ?, COALESCE((SELECT qte_stock FROM stocker WHERE ref_mar=? AND ref_entrepot=?),0))
    )");
    insertStock.addBindValue(numOperat);
    insertStock.addBindValue(refMar);
    insertStock.addBindValue(refEntrepot);
    insertStock.addBindValue(refMar);
    insertStock.addBindValue(refEntrepot);

    if(!insertStock.exec()){
        QMessageBox::critical(this,"Erreur","Impossible d'assigner l'entrepôt:\n"+insertStock.lastError().text());
        return;
    }

    QMessageBox::information(this,"Succès","Entrepôt assigné à la marchandise !");
    accept();
}
