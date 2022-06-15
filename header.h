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
/*!
 * Функция проверяет форматирование входного текста
 * param[in] familyText - входной текст
 * param[out] - код ошибки
 * код ошибки: 0 - нет ошибок, 1 -пустой файл, 2 - небуквенный символ, 3 - неверное кол-во слов в строке, 4 - не указана родсвенная связь
*/
int testInputData (QString* familyText);
/*!
 * Функция строит структуру генеологического древа в виде массива классов
 * param[in] familyText - входной текст
 * param[in|out] familyList - пустой список всех людей, будет заполнятся в процессе выполнения ф-ции
 * param[in|out] allPeople - массив классов,  будет заполнятся в процессе выполнения ф-ции
 * param[out] - код ошибки
 * код ошибки: 0 - нет ошибок, 1 - человек уже встречался, 2 - превышение кол-ва родственников(родитель или бабушка/дедушка), 3 - неуникальная связь у родственника,
 * 4 - несовпадение отчества у человека и имени родителя, 5 - несовпадение отчеств у братьев/сестер, 6 - разные родители у братьев/сестер
*/
int buildStructScheme(QString* familyText, QString* familyList, QMap <QString, human>* allPeople);
/*!
 * Функция проверяет логику структуры генеологического древа
 * param[in] familyList - список всех людей
 * param[in] allPeople - массив классов (структура генеологического древа)
 * param[out] - код ошибки
 * код ошибки: 0 - нет ошибок, 3 - неуникальная связь у родственника, 4 - несовпадение отчества у человека и имени родителя, 5 - несовпадение отчеств у братьев/сестер,
 * 6 - разные родители у братьев/сестер
*/
int testLogic(QString* familyList, QMap <QString, human>* allPeople);
/*!
 * Функция достраивает "очевидные" родственные свзи, если нужно
 * param[in] familyList - список всех людей
 * param[in] allPeople - массив классов (структура генеологического древа)
 * param[out] allPeople - массив с достроенными связями
*/
void completingSchema(QString* familyList, QMap <QString, human>* allPeople);
/*!
 * Функция создает текст с структурой древа
 * param[in] familyList - список всех людей
 * param[in] allPeople - массив классов (структура генеологического древа)
 * param[out] outputText - текст с древом
*/
void buildSchemeTree(QString* outputText, QString* familyList, QMap <QString, human>* allPeople);

#endif // HEADER_H
