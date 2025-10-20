#ifndef ENTREPOTMANAGER_H
#define ENTREPOTMANAGER_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class EntrepotManager;
}

class EntrepotManager : public QDialog
{
    Q_OBJECT

public:
    explicit EntrepotManager(QWidget *parent = nullptr);
    ~EntrepotManager();

private slots:
    void on_btnAjouter_clicked();
    void on_btnSupprimer_clicked();

private:
    Ui::EntrepotManager *ui;
    QSqlTableModel *model;
    void rafraichirTable();
};

#endif // ENTREPOTMANAGER_H
