#include "form/FormBonLivraison.h"
#include "ui_FormBonLivraison.h"
#include <QMessageBox>

FormBonLivraison::FormBonLivraison(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormBonLivraison)
{
    ui->setupUi(this);

    // Connexion manuelle pour être sûr que le slot est appelé
    connect(ui->btnGenerer, &QPushButton::clicked, this, &FormBonLivraison::on_btnGenerer_clicked);
}

FormBonLivraison::~FormBonLivraison()
{
    delete ui;
}

void FormBonLivraison::on_btnGenerer_clicked()
{
    QMessageBox::information(this, "Bon de Livraison", "Le bon de livraison a été généré !");
}
