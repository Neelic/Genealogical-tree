#ifndef HEADER_H
#define HEADER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QDebug>

class human {
public:
    QString parent1;
    QString parent2;
    QString grandParent1;
    QString grandParent2;
    QString grandParent3;
    QString grandParent4;
    QVector <QString> sibling; //Брат или сеcтра
    QVector <QString> children;
    QVector <QString> grandChildren;
};

QMap <QString, human> allPeople;
QString familyList;

int testInputData (QString* familyText);

int buildStructScheme(QString* familyText, QString* familyList);

int testLogic(QString* familyList);

void completingSchema(QString* familyList);

void buildSchemeTree(QString* outputText, QString* familyList);

#endif // HEADER_H
