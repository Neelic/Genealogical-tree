#ifndef HEADER_H
#define HEADER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QDebug>

class human {
public:
    QString name;
    QString Parent1;
    QString Parent2;
    QString GrandParent1;
    QString GrandParent2;
    QString GrandParent3;
    QString GrandParent4;
    QString Spouse;
    QVector <QString> sibling; //Брат или сеcтра
    QVector <QString> children;
};

QMap <QString, human> allPeople;
QString familyList;

int testInputData (QString* familyText);

int buildStructScheme(QString* familyText, QString* familyList);

int testLogic();

void completingSchema();

void buildSchemeTree(QString* outputText, QString* familyList);

#endif // HEADER_H
