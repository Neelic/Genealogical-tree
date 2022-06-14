#ifndef HEADER_H
#define HEADER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include "human.h"

int testInputData (QString* familyText);

int buildStructScheme(QString* familyText, QString* familyList, QMap <QString, human>* allPeople);

int testLogic(QString* familyList, QMap <QString, human>* allPeople);

void completingSchema(QString* familyList, QMap <QString, human>* allPeople);

void buildSchemeTree(QString* outputText, QString* familyList, QMap <QString, human>* allPeople);

#endif // HEADER_H
