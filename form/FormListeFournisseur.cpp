#include "FormListeFournisseur.h"
#include "ui_FormListeFournisseur.h" // Le fichier généré par le nouveau UI
#include "FormAjouterFournisseur.h"   // Pour ouvrir la fenêtre d'ajout
#include "database/database.h"        // Nécessaire pour initialiser le modèle
#include <QMessageBox>
#include <QSqlError>
#include <QHeaderView>

FormListeFournisseur::FormListeFournisseur(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormListeFournisseur),
    // Initialise le modèle avec la connexion de votre classe Database
    model(new QSqlTableModel(this, Database::db))
{
    ui->setupUi(this);
    configurerModele();
    configurerTableau();
}

FormListeFournisseur::~FormListeFournisseur()
{
    delete ui;
}

void FormListeFournisseur::configurerModele()
{
    model->setTable("fournisseur");

    // On conserve OnFieldChange pour faciliter l'édition directe dans la table (UPDATE)
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();

    // Renommer les en-têtes pour l'affichage
    model->setHeaderData(0, Qt::Horizontal, "Référence (ref_frs)");
    model->setHeaderData(1, Qt::Horizontal, "Raison Sociale");
    model->setHeaderData(2, Qt::Horizontal, "NIF");
    model->setHeaderData(3, Qt::Horizontal, "Adresse");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
}

void FormListeFournisseur::configurerTableau()
{
    ui->tableViewFournisseurs->setModel(model);
    ui->tableViewFournisseurs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewFournisseurs->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewFournisseurs->resizeColumnsToContents();
    ui->tableViewFournisseurs->horizontalHeader()->setStretchLastSection(true);
}

// Ouvre le formulaire d'ajout, lui passant le modèle à mettre à jour
void FormListeFournisseur::on_btnOuvrirAjout_clicked()
{
    // On passe le modèle au formulaire d'ajout pour qu'il puisse le modifier et le rafraîchir
    FormAjouterFournisseur *form = new FormAjouterFournisseur(model, this);
    form->exec();
}

// Opération DELETE: Supprimer un fournisseur
void FormListeFournisseur::on_btnSupprimer_clicked()
{
    QModelIndexList selection = ui->tableViewFournisseurs->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ligne à supprimer.");
        return;
    }

    int row = selection.first().row();

    if (QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer ce fournisseur ?")
        == QMessageBox::No)
    {
        return;
    }

    if (model->removeRow(row)) {
        if (!model->submitAll()) {
            QMessageBox::critical(this, "Erreur SQL", "Échec de la suppression en base : " + model->lastError().text());
            model->revertAll();
        } else {
            model->select();
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de supprimer la ligne sélectionnée.");
    }
}

// Enregistrement manuel des modifications (sera toujours appelé si vous changez la stratégie pour OnManualSubmit)
void FormListeFournisseur::on_btnEnregistrerModifications_clicked()
{
    if (model->submitAll()) {
        QMessageBox::information(this, "Succès", "Modifications enregistrées avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur SQL", "Échec de l'enregistrement : " + model->lastError().text());
        model->revertAll();
    }
}
