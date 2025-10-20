#include "Entrepot.h"
#include "ui_Entrepot.h"

#include <QSqlTableModel>
#include <QSqlRecord>       // <<-- IMPORTANT : définition de QSqlRecord
#include <QMessageBox>
#include <QModelIndexList>
#include <QHeaderView>
#include <QSqlError>

Entrepot::Entrepot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Entrepot),
    model(new QSqlTableModel(this))
{
    ui->setupUi(this);

    // Configure le modèle
    model->setTable("entrepot");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    // Connecte le modèle à la tableView
    ui->tableViewEntrepot->setModel(model);
    ui->tableViewEntrepot->resizeColumnsToContents();
    ui->tableViewEntrepot->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewEntrepot->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewEntrepot->setSelectionMode(QAbstractItemView::SingleSelection);

    // Connecte les boutons (optionnel si tu utilises les noms auto-connect)
    connect(ui->btnAjouter, &QPushButton::clicked, this, &Entrepot::on_btnAjouter_clicked);
    connect(ui->btnSupprimer, &QPushButton::clicked, this, &Entrepot::on_btnSupprimer_clicked);
}

Entrepot::~Entrepot()
{
    delete ui;
}

void Entrepot::on_btnAjouter_clicked()
{
    QString ref = ui->lineEditRefEntrepot->text().trimmed();
    QString adresse = ui->lineEditAdresse->text().trimmed();

    if (ref.isEmpty() || adresse.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    // Vérifie si l'entrepôt existe déjà (en regardant la colonne 0)
    for (int row = 0; row < model->rowCount(); ++row) {
        if (model->data(model->index(row, 0)).toString() == ref) {
            QMessageBox::warning(this, "Erreur", "Cet entrepôt existe déjà.");
            return;
        }
    }

    QSqlRecord newRecord = model->record(); // <- ok maintenant que QSqlRecord est inclus
    newRecord.setValue("ref_entrepot", ref);
    newRecord.setValue("adresse", adresse);

    if (!model->insertRecord(-1, newRecord)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ajouter l'entrepôt.");
        model->revertAll();
        return;
    }

    if (!model->submitAll()) {
        QMessageBox::warning(this, "Erreur", "Échec de la sauvegarde en base : " + model->lastError().text());
        model->revertAll();
        return;
    }

    model->select(); // Rafraîchir le tableau
    ui->lineEditRefEntrepot->clear();
    ui->lineEditAdresse->clear();
}

void Entrepot::on_btnSupprimer_clicked()
{
    QModelIndexList selection = ui->tableViewEntrepot->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ligne à supprimer.");
        return;
    }

    int row = selection.first().row();
    model->removeRow(row);

    if (!model->submitAll()) {
        QMessageBox::warning(this, "Erreur", "Impossible de supprimer l'entrepôt : " + model->lastError().text());
        model->revertAll();
    } else {
        model->select(); // Rafraîchir le tableau
    }
}
