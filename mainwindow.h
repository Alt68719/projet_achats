#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnNouveauAchat_clicked();
    void on_btnVoirStock_clicked();
    void on_btnInventaire_clicked();
    void on_btnListeLivraison_clicked();
    void on_btnAPropos_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
