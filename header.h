#ifndef HEADER_H
#define HEADER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QDebug>
#include "human.h"

QMap <QString, human> allPeople;

QString familyList;

int testInputData (QString* familyText);

int buildStructScheme(QString* familyText, QString* familyList);

int testLogic(QString* familyList);

void completingSchema(QString* familyList);

void buildSchemeTree(QString* outputText, QString* familyList);

#endif // HEADER_H
