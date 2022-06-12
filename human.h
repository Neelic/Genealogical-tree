#ifndef HUMAN_H
#define HUMAN_H
#include <QString>
#include <QVector>
#include <QMap>

class human
{
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

    bool repetitionCheck(QString name, QString role, human* tmp);
    bool humanRepetition(QString ownName, human* tmp);
    void buildingParGrandparfRelation(QString parentPatr,human* other);
};

#endif // HUMAN_H
