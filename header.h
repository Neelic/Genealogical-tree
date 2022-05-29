#ifndef HEADER_H
#define HEADER_H
#include <QString>
#include <QMap>

struct human //Структура, хранящая все родственные связи человека, используется для построе-ния генеалогического древа
{
    QString name;
    int idParent1;
    int idParent2;
    int idGrandParent1;
    int idGrandParent2;
    int idGrandParent3;
    int idGrandParent4;
    int idSpouse;
    QVector <int> sibling; //Брат или сеcтра
    QVector <int> children;
};

QMap <int, human> allPeople; //Массив, в котором хранятся индекс и ФИО всех людей. Используется для проверки получившегося древа
#endif // HEADER_H
