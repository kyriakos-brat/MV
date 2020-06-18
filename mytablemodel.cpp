#include "mytablemodel.h"
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QtXml>
#include <QXmlStreamWriter>
#include <QXmlStreamAttribute>

myTableModel::myTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

    QSqlDatabase temp_db = QSqlDatabase::database("db");
    QSqlQuery query = QSqlQuery(temp_db);
    query.exec("SELECT mrp.fullname, TO_CHAR(mrp.date_of_birth, 'DD-MM-YYYY'), mrp.post, department.title "
                    "FROM mrp JOIN department ON mrp.id_dept = department.id_dept");

    QString str = query.lastError().text();
    while (query.next()) {
        _vec.push_back({query.value(0).toString(),
                               query.value(1).toString(),
                               query.value(2).toString(),
                               query.value(3).toString() });
    }

}

int myTableModel::rowCount(const QModelIndex &parent) const
{
    return _vec.count();
}

int myTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant myTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole){
        if (checkIndex(index)){
            switch (index.column()) {
            case 0:
                return _vec[index.row()].fullname;
                break;
            case 1:
                return _vec[index.row()].dob;
                break;
            case 2:
                return _vec[index.row()].post;
                break;
            case 3:
                return _vec[index.row()].dept;
                break;
            default:
                return QVariant();
            }
        }
    }
    return QVariant();

}

bool myTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (checkIndex(index)){
        if (role == Qt::EditRole){
            switch (index.column()) {
            case 0:
                _vec[index.row()].fullname = value.toString();
                return true;
                break;
            case 1:
                _vec[index.row()].dob = value.toString();
                return true;
                break;
            case 2:
                _vec[index.row()].post = value.toString();
                return true;
                break;
            case 3:
                _vec[index.row()].dept = value.toString();
                return true;
                break;
            default:
                return false;
            }
        }
    }
    return false;
}

Qt::ItemFlags myTableModel::flags(const QModelIndex &index) const
{
    if (checkIndex(index))
        return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    else
        return Qt::NoItemFlags;
}

QVariant myTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal){
        switch (section){
        case 0:
            return QString("Fullname");
        case 1:
            return QString("Date of birth");
        case 2:
            return QString("Post");
        case 3:
            return QString("Department");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool myTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (!((row < 0) || (row > this->rowCount()))){
        beginInsertRows(parent, row, row+count-1);
        _vec.push_back({"", "", "", ""});
        endInsertRows();
        return true;
    }
    else
        return false;
}

bool myTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!((row < 0) || (row > this->rowCount()-1))){
        for (int i = 0; i < count; ++i){
            beginRemoveRows(parent, row, row+count-1);
            _vec.removeAt(row);
            endRemoveRows();
        }
        return true;
    }
    else
        return false;
}

bool myTableModel::saveToFile()
{
    QFile file("./users.xml");
    QXmlStreamWriter xmlWriter;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    xmlWriter.setDevice(&file);
    xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("mrps");
            for (int i = 0; i < _vec.count(); ++i ){
                xmlWriter.writeStartElement("mrp");
                    xmlWriter.writeAttribute("id", QString::number(i+1));
                    xmlWriter.writeTextElement("fullname", _vec[i].fullname );
                    xmlWriter.writeTextElement("dob", _vec[i].dob);
                    xmlWriter.writeTextElement("post", _vec[i].post);
                    xmlWriter.writeTextElement("dept", _vec[i].dept);
                xmlWriter.writeEndElement();
            }
        xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
    return true;

}

bool myTableModel::loadFromFile(QString &string)
{
    sqlrecs temp_rec;
    clear_model();
    QFile file(string);
    QXmlStreamReader xmlReader;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    xmlReader.setDevice(&file);
    while (!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement){
            if (xmlReader.name() == "mrps")
                continue;
            if (xmlReader.name() == "mrp"){
                QXmlStreamAttributes attrs = xmlReader.attributes();
                if (attrs.hasAttribute("id")){
                    xmlReader.readNext();
                    while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "mrp")){
                        if(xmlReader.tokenType() == QXmlStreamReader::StartElement){
                            if (xmlReader.name() == "fullname"){
                                xmlReader.readNext();
                                temp_rec.fullname = xmlReader.text().toString();
                            }
                            if (xmlReader.name() == "dob"){
                                xmlReader.readNext();
                                temp_rec.dob = xmlReader.text().toString();
                            }
                            if (xmlReader.name() == "post"){
                                xmlReader.readNext();
                                temp_rec.post = xmlReader.text().toString();
                            }
                            if (xmlReader.name() == "dept"){
                                xmlReader.readNext();
                                temp_rec.dept = xmlReader.text().toString();
                            }
                        }
                         xmlReader.readNext();
                        if (xmlReader.name() == "mrp" && xmlReader.tokenType() == QXmlStreamReader::EndElement){
                            beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());
                            _vec.push_back(temp_rec);
                            endInsertRows();
                        }
                    }
                }
            }
        }
    }
    return true;
}

void myTableModel::clear_model()
{
    int rowCount_before_del = this->rowCount();
    for (int i = 0; i < rowCount_before_del; ++i){
        beginRemoveRows(QModelIndex(), 0,  0);
        _vec.removeAt(0);
        endRemoveRows();
    }
}








