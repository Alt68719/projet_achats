#ifndef FORMLIVRAISON_H
#define FORMLIVRAISON_H

#endif // FORMLIVRAISON_H

#pragma once
#include <QDialog>

namespace Ui { class FormLivraison; }

class FormLivraison : public QDialog
{
    Q_OBJECT

public:
    explicit FormLivraison(QWidget *parent = nullptr);
    ~FormLivraison();

private slots:
    void on_btnAjouter_clicked();

private:
    Ui::FormLivraison *ui;

    void mettreAJourStock(const QString& ref_Mar, const QString& ref_Entr, int qte);
};

