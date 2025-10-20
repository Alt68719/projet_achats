#ifndef VIEWSTOCK_H
#define VIEWSTOCK_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class ViewStock;
}

class ViewStock : public QDialog
{
    Q_OBJECT

public:
    explicit ViewStock(QWidget *parent = nullptr);
    ~ViewStock();

private:
    Ui::ViewStock *ui;
    QSqlQueryModel *model;
    QSqlQueryModel *modelMouvements;

    void chargerStock();
    void chargerMouvements();
};

#endif // VIEWSTOCK_H
