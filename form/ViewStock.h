#ifndef VIEWSTOCK_H
#define VIEWSTOCK_H

#include <QDialog>

class QSqlQueryModel;

namespace Ui {
class ViewStock;
}

class ViewStock : public QDialog
{
    Q_OBJECT

public:
    explicit ViewStock(QWidget *parent = nullptr);
    ~ViewStock();

private slots:
    void supprimerStockSelectionne();

private:
    void chargerStock();
    void chargerMouvements();

private:
    Ui::ViewStock *ui;
    QSqlQueryModel *model;
    QSqlQueryModel *modelMouvements;
};

#endif // VIEWSTOCK_H
