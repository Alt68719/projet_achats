#ifndef FORMFOURNISSEUR_H
#define FORMFOURNISSEUR_H

#endif // FORMFOURNISSEUR_H

#pragma once
#include <QDialog>

namespace Ui { class FormFournisseur; }

class FormFournisseur : public QDialog
{
    Q_OBJECT

public:
    explicit FormFournisseur(QWidget *parent = nullptr);
    ~FormFournisseur();

private slots:
    void on_btnAjouter_clicked();

private:
    Ui::FormFournisseur *ui;
};

