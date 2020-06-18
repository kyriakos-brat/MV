#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QVector>
#include <QObject>
#include <QSqlQuery>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("MTEofEP");
    db.setUserName("johnhowd");
    db.setPassword("coldasice");
    ok = db.open();
    if (!ok)
        QMessageBox::critical(this, "Connection error", "ERROR: No connection to DB. " + db.lastError().text());
    else
        qDebug() << "Connection established!";

    model = new myTableModel();

    ui->tableView->setModel(model);

    curr_row = model->rowCount()-1;


}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}



void MainWindow::on_btnAdd_user_clicked()
{
    ok = model->insertRows(model->rowCount(), 1);
    if (!ok)
        QMessageBox::critical(this, "Adding error", "ERROR: Inappropiate index to add");
    else {
        curr_row = model->rowCount()-1;
        qDebug() << "User succeful added!";
    }
}

void MainWindow::on_btnDelete_user_clicked()
{
    ok = model->removeRows(curr_row, 1);
    if (!ok)
        QMessageBox::critical(this, "Deleting error", "ERROR: Inappropiate index to delete");
    else {
        qDebug() << "User succeful deleted!";
        curr_row = model->rowCount()-1;
    }

}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    curr_row = index.row();
}



void MainWindow::on_btnSave_clicked()
{
    ok = model->saveToFile();
    if (!ok)
        QMessageBox::critical(this, "Saving error", "ERROR");
    else {
        qDebug() << "Data succeful saved to file!";
    }

}

void MainWindow::on_btnLoad_clicked()
{
    QString filter = "XML File (*.xml) ;; All File (*.*)";
    QString file_name = QFileDialog::getOpenFileName(this, "Open a file", QDir::currentPath(), filter);
    if (!file_name.isEmpty())
        model->loadFromFile(file_name);
    else
        QMessageBox::critical(this, "Load from file error!", "ERROR: File wasn't selected.");
}
