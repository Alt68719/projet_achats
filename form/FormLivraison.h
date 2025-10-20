#ifndef FORMLIVRAISON_H
#define FORMLIVRAISON_H


#pragma once
#include <QDialog>
#include <QSqlQuery>
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

    void mettreAJourStock(const QString& ref_mar, const QString& ref_entrepot, int qte_stock);
};
#endif // FORMLIVRAISON_H
