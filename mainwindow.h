#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QtCore>
#include <QtGui>
#include <mytablemodel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btnAdd_user_clicked();

    void on_btnDelete_user_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_btnSave_clicked();

    void on_btnLoad_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    myTableModel *model;
    int curr_row;
    bool ok;
    QStandardItem* item;
};
#endif // MAINWINDOW_H
