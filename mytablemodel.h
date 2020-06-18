#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QString>



class myTableModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    struct sqlrecs{
        QString fullname;
        QString dob;
        QString post;
        QString dept;
    };
    QVector<sqlrecs> _vec;
public:
    myTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool saveToFile();
    bool loadFromFile(QString &string);
    void clear_model();


};




#endif // MYTABLEMODEL_H
