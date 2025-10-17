#include "mainwindow.h"
#include "ui_mainwindow.h"

// Formulaires spécifiques
#include "form/FormFournisseur.h"
#include "form/FormMarchandise.h"
#include "form/FormEntrepot.h"
#include "form/FormLivraison.h"
#include "form/FormBonLivraison.h"


#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connexion des boutons si non déjà fait dans Qt Designer
    connect(ui->btnNouveauAchat, &QPushButton::clicked, this, &MainWindow::on_btnNouveauAchat_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// -------- Bouton Nouveau Achat --------
void MainWindow::on_btnNouveauAchat_clicked()
{
    FormFournisseur fFournisseur(this);
    if(fFournisseur.exec() == QDialog::Rejected) return;

    FormMarchandise fMarchandise(this);
    if(fMarchandise.exec() == QDialog::Rejected) return;

    FormEntrepot fEntrepot(this);
    if(fEntrepot.exec() == QDialog::Rejected) return;

    FormLivraison fLivraison(this);
    if(fLivraison.exec() == QDialog::Rejected) return;

    FormBonLivraison fBon(this);
    fBon.exec();  // Affiche le bouton générer le bon
}

// -------- Autres boutons --------
void MainWindow::on_btnVoirStock_clicked()
{
    ViewStock view(this);
    view.exec();
}

void MainWindow::on_btnInventaire_clicked()
{
    ViewInventaire view(this);
    view.exec();
}

void MainWindow::on_btnListeLivraison_clicked()
{
    ViewListeLivraison view(this);
    view.exec();
}

void MainWindow::on_btnAPropos_clicked()
{
    ViewAPropos view(this);
    view.exec();
}
