#include <QCoreApplication>
#include "header.h"
#include "human.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QMap <QString, human> allPeople;
    QString familyList;

    //Проверка на существование файла
    //if (!(QFile::exists(argv[1]))) {
    //    qDebug() << "Файл не сущетсвует";
    //    return 0;
    //}

    QString familyText;
    QFile fileIn(argv[1]);
    //Считать файл, если он открыт для чтения
    //if (fileIn.open(QIODevice::ReadOnly)) {
    //    familyText = fileIn.readAll();
    //    fileIn.close();
    //}
    //else {
    //    qDebug() << "Не удалось считать файл";
    //    return 0;
    //}

    familyText = "Абрамов Кирилл Васильевич сын Абрамов Василий Артемович\r\nАбрамов Артем Александрович дед Абрамов Кирилл Васильевич\r\nАбрамов Мефодий Александрович дед Абрамов Кирилл Васильевич";

    int error = testInputData(&familyText);

    if (error != 0) {
        if (error == 1) {
            qDebug() << "Файл пуст";
            return 0;
        }
        if (error == 2) {
            qDebug() << "Возможно, неправильно указано одно из имен";
            return 0;
        }
        if (error == 3 || error == 4) {
            qDebug() << "Одна из строк неправильно объявлена";
            return 0;
        }
        return 0;
    }
    //Построить структуру деревы и проверить ее
    error = 0;
    error = buildStructScheme(&familyText, &familyList, &allPeople);
    //Получить содержимое dot файла
    QString outputText;
    buildSchemeTree(&outputText, &familyList, &allPeople);
    //Создать dot файл
    //QFile fileOut("Geneological_Tree.dot");
    //fileOut.open(QIODevice::WriteOnly | QIODevice::Text);
    //QTextStream stream(&fileOut);
    //stream << outputText;
    //fileOut.close();

    //Запустить GraphViz
    //QProcessEnvironment env = QProcessEnvironment::systemEnvironment();


    return 0;
}

int testInputData (QString* familyText) {
    if (familyText->isEmpty())                  //Если файл пуст, вывести ошибку
        return 1;
    int idNewStr = familyText->indexOf("\r\n"); //Получить индекс конца первой строки
    if (idNewStr == -1)
        idNewStr = familyText->length();
    int idOldStr = 0;                          //Инициализировать индекс начала строки
    //Пока не проверен весь входной текст...
    do {
        QString tmpStr = familyText->mid(idOldStr,idNewStr - idOldStr); //Если в текущей строке есть небуквенные символы, вернуть ошибку
        //Проверить текущую строку на небуквенный символ
        if (tmpStr.indexOf("\"") != -1 || tmpStr.indexOf("@") != -1 || tmpStr.indexOf("!") != -1 || tmpStr.indexOf("#") != -1 || tmpStr.indexOf("$") != -1
                || tmpStr.indexOf(";") != -1 || tmpStr.indexOf("%") != -1 || tmpStr.indexOf("^") != -1 || tmpStr.indexOf(":") != -1 || tmpStr.indexOf("&") != -1
                || tmpStr.indexOf("?") != -1 || tmpStr.indexOf("*") != -1 || tmpStr.indexOf("(") != -1 || tmpStr.indexOf(")") != -1 || tmpStr.indexOf("\\") != -1
                || tmpStr.indexOf("'") != -1 || tmpStr.indexOf("/") != -1 || tmpStr.indexOf(",") != -1 || tmpStr.indexOf(".") != -1 || tmpStr.indexOf("`") != -1
                || tmpStr.indexOf("~") != -1)
            return 2;
        //Если в текущей строке неверное кол-во слов, вернуть ошибку
        if ((tmpStr.split(" ").count()) != 7)
            return 3;
        //Если в троке нет родственной связи, вернуть ошибку
        if (!(tmpStr.contains("мать") || tmpStr.contains("отец") || tmpStr.contains("сын") || tmpStr.contains("дочь") || tmpStr.contains("бабушка") || tmpStr.contains("дед") ||
              tmpStr.contains("дедушка") || tmpStr.contains("брат") || tmpStr.contains("сестра") || tmpStr.contains("внук") || tmpStr.contains("внучка")))
            return 4;
        //Перейти к следующей строке
        idOldStr = idNewStr + 2;
        idNewStr = familyText->indexOf("\r\n", idOldStr);
        if (idNewStr == -1 && idOldStr < familyText->length())
            idNewStr = familyText->length();
    } while (idNewStr != -1);
    return 0;
}

int buildStructScheme(QString* familyText, QString* familyList, QMap <QString, human>* allPeople) {
    int idNewStr = familyText->indexOf("\r\n"); //Получить индекс конца строки
    if (idNewStr == -1)
        idNewStr = familyText->length();
    int idOldStr = 0;                           //Инициализировать индекс начала строки
    //Пока не пройден весь входной текст
    do {
        //Получить индекс родственной связи текущей строки
        int idKinship = familyText->indexOf("мать", idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("отец",idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("сын",idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("дочь",idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("бабушка",idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("дед",idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("дедушка",idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("брат",idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("сестра",idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("внук",idOldStr);
        if (idKinship == -1 || idKinship > idNewStr)
            idKinship = familyText->indexOf("внучка",idOldStr);
        //Получить строку с ФИО первого человека
        QString name1 = familyText->mid(idOldStr, idKinship - idOldStr - 1);
        //Получить строку с ФИО второго человека
        QString name2 = familyText->mid(familyText->indexOf(" ",idKinship) + 1,idNewStr - familyText->indexOf(" ",idKinship) - 1);
        //Получить строку с родственной связью
        QString kinship = familyText->mid(idKinship, familyText->indexOf(" ", idKinship) - idKinship);
        //Для первого человека...
        human tmp;
        //Занести первого человека в общий список, если он встречается впервые...
        if (!(allPeople->contains(name1))) {
            familyList->insert(familyList->lastIndexOf("\n") + 1,name1 + "\n");
        } else  //иначе достать его из массива всех людей
            tmp = allPeople->value(name1);
        //Добавить ребенка, если связь мать или отец
        if (kinship == "мать" || kinship == "отец") {
            int count = tmp.children.count();
            for (int i = 1; i <= count; i++) {
                if (tmp.children[i - 1] == name2) //Вывести ошибку, если уже встречался
                    return 1;
            }
            tmp.children << name2;
            //Добавить родителя, если связь сын или дочь
        } else if (kinship == "сын" || kinship == "дочь") {
            if (tmp.parent1.contains(name2) || tmp.parent2.contains(name2)) //Вывести ошибку, если уже встречался
                return 1;
            if (!(tmp.parent1.isEmpty())) {
                if (!(tmp.parent2.isEmpty())) //Вывести ошибку, если родителей уже 2
                    return 2;
                else tmp.parent2 = name2;
            } else tmp.parent1 = name2;
            //Добавить внука/внучку, если связь бабушка, дедушка или дед
        } else if (kinship == "бабушка" || kinship == "дед" || kinship == "дедушка") {
            int count = tmp.grandChildren.count();
            for (int i = 1; i <= count; i++) {
                if (tmp.grandChildren[i - 1] == name2)  //Вывести ошибку, если уже встречался
                    return 1;
            }
            //Добавить брата/сестру, если связь брат или сестра
            tmp.grandChildren << name2;
        } else if (kinship == "брат" || kinship == "сестра") {
            int count = tmp.sibling.count();
            for (int i = 1; i <= count; i++) {
                if (tmp.sibling[i - 1] == name2)  //Вывести ошибку, если уже встречался
                    return 1;
            }
            tmp.sibling << name2;
            //Добавить бабушку/дедушка, если связь внук или внучка
        } else if (kinship == "внук" || kinship == "внучка") {
            if (tmp.grandParent1.contains(name2) || tmp.grandParent2.contains(name2) || tmp.grandParent3.contains(name2) || tmp.grandParent4.contains(name2)) //Вывести ошибку, если уже встречался
                return 1;
            if (!(tmp.grandParent1.isEmpty())) {   //Вывести ошибку, если прародителей уже 4
                if (!(tmp.grandParent2.isEmpty())) {
                    if (!(tmp.grandParent3.isEmpty())) {
                        if (!(tmp.grandParent4.isEmpty()))
                            return 2;
                        else tmp.grandParent4 = name2;
                    } else tmp.grandParent3 = name2;
                } else tmp.grandParent2 = name2;
            } else tmp.grandParent1 = name2;
        }
        allPeople->insert(name1,tmp); //Добавить первого человека в массив
        //Для второго человека
        human tmp1;
        //Занести его в список, если встречается впервые
        if (!(allPeople->contains(name2))) {
            familyList->insert(familyList->lastIndexOf("\n") + 1,name2 + "\n");
        }
        else //иначе достать его из массива всех людей
            tmp1 = allPeople->value(name2);
        //Добавить родителя, если связь мать или отец
        if (kinship == "мать" || kinship == "отец") {
            if (tmp1.parent1.contains(name1) || tmp1.parent2.contains(name1)) //Вывести ошибку, если уже встречался
                return 1;
            if (!(tmp1.parent1.isEmpty())) {
                if (!(tmp1.parent2.isEmpty()))  //Вывести ошибку, если родителей уже 2
                    return 2;
                else tmp1.parent2 = name1;
            } else tmp1.parent1 = name1;
            //добавить ребенка, если связь сын или дочь
        } else if (kinship == "сын" || kinship == "дочь") {
            int count = tmp1.children.count();
            for (int i = 1; i <= count; i++) {
                if (tmp1.children[i - 1] == name1) //Вывести ошибку, если уже встречался
                    return 1;
            }
            tmp1.children << name1;
            //добавить прародителя, если связь бабушка, дедушка или дед
        } else if (kinship == "бабушка" || kinship == "дед" || kinship == "дедушка") {
            if (tmp1.grandParent1.contains(name1) || tmp1.grandParent2.contains(name1) || tmp1.grandParent3.contains(name1) || tmp1.grandParent4.contains(name1)) //Вывести ошибку, если уже встречался
                return 1;
            if (!(tmp1.grandParent1.isEmpty())) {  //Вывести ошибку, если прародителя уже 4
                if (!(tmp1.grandParent2.isEmpty())) {
                    if (!(tmp1.grandParent3.isEmpty())) {
                        if (!(tmp1.grandParent4.isEmpty()))
                            return 2;
                        else tmp1.grandParent4 = name1;
                    } else tmp1.grandParent3 = name1;
                } else tmp1.grandParent2 = name1;
            } else tmp1.grandParent1 = name1;
            //Добавить брата/сестру, если связь брат или сестра
        } else if (kinship == "брат" || kinship == "сестра") {
            int count = tmp1.sibling.count();
            for (int i = 1; i <= count; i++) {
                if (tmp1.sibling[i - 1] == name1) //Вывести ошибку, если уже встречался
                    return 1;
            }
            tmp1.sibling << name1;
            //Добавить внука/внучку, если связь внук или внучка
        } else if (kinship == "внук" || kinship == "внучка") {
            int count = tmp.grandChildren.count();
            for (int i = 1; i <= count; i++) {
                if (tmp.grandChildren[i - 1] == name1) //Вывести ошибку, если уже встречался
                    return 1;
            }
            tmp1.grandChildren << name1;
        }
        allPeople->insert(name2,tmp1); //Добавить второго человека в массив
        //Получить индекс начала и конца следующей строки
        idOldStr = idNewStr + 2;
        idNewStr = familyText->indexOf("\r\n",idOldStr);
        if (idNewStr == -1 && idOldStr < familyText->length())
            idNewStr = familyText->length();
    } while (idNewStr != -1);
    //Проверить логику массива всех людей
    int error = 0;
    error = testLogic(familyList, allPeople);
    //Достроить связи, если ошибок нет
    if (error == 0)
        completingSchema(familyList,allPeople);
    return error;
}

int testLogic(QString* familyList, QMap <QString, human>* allPeople) {
    //Получить индекс начала и конца первого имени в списке всех членов семьи
    int idStartName = 0;
    int idEndName = familyList->indexOf("\n");
    int error = 0;
    //Пока не будет проверен весь список семьи
    do {
        human tmp;
        QString name = familyList->mid(idStartName, idEndName - idStartName);  //Получить ФИО текущего человека
        QString patr = name.right(name.length() - name.lastIndexOf(" ") - 1);  //Получить его отчество без 3-х последних букв
        patr.remove(patr.length() - 3, 3);
        tmp = allPeople->value(name);
        //Проверить, чтобы у родственников была только уникальная связь
        if (tmp.humanRepetition(name, &tmp))
            error = 3;
        //Проверить отчество текущего человека и имена родителей на соответсвие
        if (!(tmp.parent1.isEmpty())) {
            int mis = 0;
            //Получить имя родителя
            QString parentName = tmp.parent1.mid(tmp.parent1.indexOf(" ") + 1, tmp.parent1.indexOf(" ", tmp.parent1.indexOf(" ") + 1) - tmp.parent1.indexOf(" ") - 1);
            parentName.remove(parentName.length() - 3, 3);
            if (!(patr.contains(parentName))) { //Вывести ошибку, если отчество не совпадает ни с одним именем родителей
                mis = 1;
                if (tmp.parent2.isEmpty()) //Если родитель 1, то ошибкой не считать(неточность входных данных)
                    mis = 0;
                if (mis == 1 && !(tmp.parent2.isEmpty())) {
                    parentName = tmp.parent2.mid(tmp.parent2.indexOf(" "), tmp.parent2.indexOf(" ", tmp.parent2.indexOf(" ") + 1) - tmp.parent2.indexOf(" "));
                    if (patr.contains(parentName))
                        mis = 0;
                }
            }
            if (mis == 1)
                error = 4;
        }
        //Проверка отчества и проверка на разных родителей у братьев/сестер, если они есть
        if (!(tmp.sibling.isEmpty()) && error == 0) {
            for (int i = 0; i < tmp.sibling.count() && error == 0; i++) {
                QString siblingPatr = tmp.sibling[i].right(tmp.sibling[i].length() - tmp.sibling[i].lastIndexOf(" "));
                if (!(siblingPatr.contains(patr)))
                    error = 5;
                human sibl = allPeople->value(tmp.sibling[i]);
                if (tmp.parent1 != sibl.parent1 && tmp.parent1 != sibl.parent2 && tmp.parent2 != sibl.parent1 && tmp.parent2 != sibl.parent2)
                    error = 6;
            }
        }
        //Получить индекс начала и конца следующего имени
        idStartName = idEndName + 1;
        idEndName = familyList->indexOf("\n", idStartName);
    } while (idEndName != -1);
    return error;
}

void completingSchema(QString* familyList, QMap <QString, human>* allPeople) {
    //Получить индекс начала и конца первого имени в списке всех членов семьи
    int idStartName = 0;
    int idEndName = familyList->indexOf("\n");
    //Пока не будет проверен весь спискок всех членов семьи
    do {
        human tmp;
        QString name = familyList->mid(idStartName, idEndName - idStartName); //Получить ФИО текущего человека
        QString patr = name.right(name.length() - name.lastIndexOf(" ") - 1); // Получить отчество текущего человека без 3 правых букв
        patr.remove(patr.length() - 3, 3);
        tmp = allPeople->value(name);
//Если у текущего человека есть дедушка и родитель, у родителя нет своего родителя, у родителя отчество и фамилия совпадает с именем дедушки, добавить к родителю дедушку
        if (!(tmp.parent1.isEmpty())) {
            if (!(tmp.grandParent1.isEmpty())) {
                human parent = allPeople->value(tmp.parent1); //Получить родственные связи первого родителя
                //Если прародитель текущего человека не является никаким родителем родителя...
                if (!(!(parent.parent1.isEmpty()) && (parent.parent1 == tmp.grandParent1 || parent.parent1 == tmp.grandParent2 || parent.parent1 == tmp.grandParent3 || parent.parent1 == tmp.grandParent4)) &&
                      !(!(parent.parent1.isEmpty()) && (parent.parent2 == tmp.grandParent1 || parent.parent2 == tmp.grandParent2 || parent.parent2 == tmp.grandParent3 || parent.parent2 == tmp.grandParent4))) {
                    QString parentPatr = tmp.parent1.right(tmp.parent1.length() - tmp.parent1.lastIndexOf(" "));
                    parentPatr.remove(parentPatr.length() - 3, 3);
                    tmp.buildingParGrandparfRelation(parentPatr, &parent); //Добавить родителя к родителю, если есть сопадение имя-отчество
                    allPeople->insert(tmp.parent1, parent);
                }
            }
        }
        if (!(tmp.parent2.isEmpty())) {
            if (!(tmp.grandParent1.isEmpty())) {
                human parent = allPeople->value(tmp.parent2); //Получить родственные связи первого родителя
                //Если прародитель текущего человека не является никаким родителем родителя...
                if (!(parent.parent1 == tmp.grandParent1 || parent.parent1 == tmp.grandParent2 || parent.parent1 == tmp.grandParent3 || parent.parent1 == tmp.grandParent4) &&
                      !(parent.parent2 == tmp.grandParent1 || parent.parent2 == tmp.grandParent2 || parent.parent2 == tmp.grandParent3 || parent.parent2 == tmp.grandParent4)) {
                    QString parentPatr = tmp.parent2.right(tmp.parent2.length() - tmp.parent2.lastIndexOf(" "));
                    parentPatr.remove(parentPatr.length() - 3, 3);
                    tmp.buildingParGrandparfRelation(parentPatr, &parent); //Добавить родителя к родителю, если есть сопадение имя-отчество
                    allPeople->insert(tmp.parent1, parent);
                }
            }
        }
//Если у текущего человека есть братья(сестры) с одним отчеством, а у братьев/сестер нет родителя, и у родителя нет среди детей братьев/сестер текущего человека, добавить родителю детей
        if (!(tmp.parent1.isEmpty()) && tmp.sibling.count() != 0) {
            human parent = allPeople->value(tmp.parent1); //Получить родственне связи родителя
            QString missChild; //Инициализировать массив "потерянных" детей
            for (int j = 0; j < tmp.sibling.count(); j++) { //Для каждого брата/сестры
                int match = 0;
                for (int i = 0; i < parent.children.count(); i ++) { //Для каждого ребенка родителя
                    if (parent.children[i] == tmp.sibling[j]) //Получить совпадение, если брат/сестра найдена в детях родителя
                        match = 1;
                }
                if (match == 0) //Добавить текущего брата/сестру в список "потерянных" детей, если нет совпадений
                    missChild.insert(missChild.length(), tmp.sibling[j] + "\n");
            } //Получить индекс начала и конца первого ребенка из списка "потерянных" детей
            int idStartNameM = 0;
            int idEndNameM = missChild.indexOf("\n");
            while (idEndNameM != -1) { //Добавить родителю всех "потерянных" детей
                parent.children << missChild.mid(idStartNameM,idEndNameM - idStartNameM);
                idStartNameM = idEndNameM + 1;
                idEndNameM = missChild.lastIndexOf("\n", idStartNameM);
            }            
            allPeople->insert(tmp.parent1, parent);
            //Проверить, чтобы у всех братьев сестер не было разных родителей (1-й родитель)
            for (int i = 0; i < tmp.sibling.count(); i++) {
                human sibling;
                sibling = allPeople->value(tmp.sibling[i]);
                if (tmp.parent1 != sibling.parent1 && tmp.parent1 != sibling.parent2) {
                    if (!(sibling.parent1.isEmpty()))
                        sibling.parent2 = tmp.parent1;
                    else
                        sibling.parent1 = tmp.parent1;
                    allPeople->insert(tmp.sibling[i], sibling);
                }
            }
            //Поиск "потерянных" детей у второго родителя
            if (!(tmp.parent2.isEmpty()) && tmp.sibling.count() != 0) {
                human parent = allPeople->value(tmp.parent2);
                QString missChild;
                for (int j = 0; j < tmp.sibling.count(); j++) {
                    int match = 0;
                    for (int i = 0; i < parent.children.count(); i ++) {
                        if (parent.children[i] == tmp.sibling[j])
                            match = 1;
                    }
                    if (match == 0)
                        missChild.insert(missChild.length(), tmp.sibling[j] + "\n");
                } //Добавление их к родителю
                int idStartNameM = 0;
                int idEndNameM = missChild.indexOf("\n");
                while (idEndNameM != -1) {
                    parent.children << missChild.mid(idStartNameM,idEndNameM - idStartNameM);
                    idStartNameM = idEndNameM + 1;
                    idEndNameM = missChild.lastIndexOf("\n", idStartNameM);
                }
                allPeople->insert(tmp.parent2, parent);
                //Проверка на несовпадение второго родителя
                for (int i = 0; i < tmp.sibling.count(); i++) {
                    human sibling;
                    sibling = allPeople->value(tmp.sibling[i]);
                    if (tmp.parent2 != sibling.parent1 && tmp.parent2 != sibling.parent2) {
                        if (!(sibling.parent1.isEmpty()))
                            sibling.parent2 = tmp.parent1;
                        else
                            sibling.parent1 = tmp.parent2;
                        allPeople->insert(tmp.sibling[i], sibling);
                    }
                }
            }
        }
        //Получить индексы начала и конца имени следующего человека из списка семьи
        idStartName = idEndName + 1;
        idEndName = familyList->indexOf("\n", idStartName);
    } while (idEndName != -1);
}

void buildSchemeTree(QString* outputText, QString* familyList, QMap <QString, human>* allPeople) {
    outputText->insert(0,"digraph Geneological_Tree {\r\n"); //Добавить строки для инициализации графа в выходной текст
    int idStartName = 0;
    int idEndName = familyList->indexOf("\n");
    //Пока не будут проверены все люди
    do {
        human tmp;
        human parent;
        QString name = familyList->mid(idStartName, idEndName - idStartName);
        tmp = allPeople->value(name);
        //Добавить соответсвующую строчку в выходной текст, если у текущего человека есть первый родитель
        if (!(tmp.parent1.isEmpty())) {
            if (!(outputText->contains(tmp.parent1 + " -> " + name + " [label=\"Родитель\"];\r\n")))
                outputText->insert(outputText->length()," " + tmp.parent1 + " -> " + name + " [label=\"Родитель\"];\r\n");
            parent = allPeople->value(tmp.parent1);
            bool isMeetGP1, isMeetGP2, isMeetGP3, isMeetGP4;
            isMeetGP1 = isMeetGP2 = isMeetGP3 = isMeetGP4 = 0;
            //Проверить, всречается ли прародители текущаго человека в родителях первого родителя и они не встречались в тексте
            if(!(tmp.grandParent1.isEmpty()) && (tmp.grandParent1 == parent.parent1 || tmp.grandParent1 == parent.parent2))
                isMeetGP1 = 1;
            if(!(tmp.grandParent2.isEmpty()) && (tmp.grandParent2 == parent.parent1 || tmp.grandParent2 == parent.parent2))
                isMeetGP2 = 1;
            if(!(tmp.grandParent3.isEmpty()) && (tmp.grandParent3 == parent.parent1 || tmp.grandParent3 == parent.parent2))
                isMeetGP3 = 1;
            if(!(tmp.grandParent4.isEmpty()) && (tmp.grandParent4 == parent.parent1 || tmp.grandParent4 == parent.parent2))
                isMeetGP4 = 1;
            if (!(tmp.parent2.isEmpty())) { //Добавить соответсвующую строчку в выходной текст, если у текущего человека есть второй родитель и они не встречались в тексте
                if (!(outputText->contains(tmp.parent2 + " -> " + name + " [label=\"Родитель\"];\r\n")))
                    outputText->insert(outputText->length()," " + tmp.parent2 + " -> " + name + " [label=\"Родитель\"];\r\n");
                parent = allPeople->value(tmp.parent2);
                //Проверить, всречается ди прародители текущаго человека в родителях первого родителя
                if(!(tmp.grandParent1.isEmpty()) && (tmp.grandParent1 == parent.parent1 || tmp.grandParent1 == parent.parent2))
                    isMeetGP1 = 1;
                if(!(tmp.grandParent2.isEmpty()) && (tmp.grandParent2 == parent.parent1 || tmp.grandParent2 == parent.parent2))
                    isMeetGP2 = 1;
                if(!(tmp.grandParent3.isEmpty()) && (tmp.grandParent3 == parent.parent1 || tmp.grandParent3 == parent.parent2))
                    isMeetGP3 = 1;
                if(!(tmp.grandParent4.isEmpty()) && (tmp.grandParent4 == parent.parent1 || tmp.grandParent4 == parent.parent2))
                    isMeetGP4 = 1;
            }//Добавить соответсвующие строки прародителей, если они не встречались в родителях родителей и они не встречались в тексте
            if (!(isMeetGP1) && !(tmp.grandParent1.isEmpty()) && !(outputText->contains(tmp.grandParent1 + " -> " + name + " [label=\"Прародитель\"];\r\n")))
                outputText->insert(outputText->length()," " + tmp.grandParent1 + " -> " + name + " [label=\"Прародитель\"];\r\n");
            if (!(isMeetGP2) && !(tmp.grandParent2.isEmpty()) && !(outputText->contains(tmp.grandParent2 + " -> " + name + " [label=\"Прародитель\"];\r\n")))
                outputText->insert(outputText->length()," " + tmp.grandParent2 + " -> " + name + " [label=\"Прародитель\"];\r\n");
            if (!(isMeetGP3) && !(tmp.grandParent3.isEmpty()) && !(outputText->contains(tmp.grandParent3 + " -> " + name + " [label=\"Прародитель\"];\r\n")))
                outputText->insert(outputText->length()," " + tmp.grandParent3 + " -> " + name + " [label=\"Прародитель\"];\r\n");
            if (!(isMeetGP4) && !(tmp.grandParent4.isEmpty()) && !(outputText->contains(tmp.grandParent4 + " -> " + name + " [label=\"Прародитель\"];\r\n")))
                outputText->insert(outputText->length()," " + tmp.grandParent4 + " -> " + name + " [label=\"Прародитель\"];\r\n");
        } else { //Иначе добавить соответсвующие строки прародителей, если они есть и они не встречались в тексте
            if (!(tmp.grandParent1.isEmpty()) && !(outputText->contains(tmp.grandParent1 + " -> " + name + " [label=\"Прародитель\"];\r\n")))
                outputText->insert(outputText->length()," " + tmp.grandParent1 + " -> " + name + " [label=\"Прародитель\"];\r\n");
            if (!(tmp.grandParent2.isEmpty()) && !(outputText->contains(tmp.grandParent2 + " -> " + name + " [label=\"Прародитель\"];\r\n")))
                outputText->insert(outputText->length()," " + tmp.grandParent2 + " -> " + name + " [label=\"Прародитель\"];\r\n");
            if (!(tmp.grandParent3.isEmpty()) && !(outputText->contains(tmp.grandParent3 + " -> " + name + " [label=\"Прародитель\"];\r\n")))
                outputText->insert(outputText->length()," " + tmp.grandParent3 + " -> " + name + " [label=\"Прародитель\"];\r\n");
            if (!(tmp.grandParent4.isEmpty()) && !(outputText->contains(tmp.grandParent4 + " -> " + name + " [label=\"Прародитель\"];\r\n")))
                outputText->insert(outputText->length()," " + tmp.grandParent4 + " -> " + name + " [label=\"Прародитель\"];\r\n");
        } //Добавить соответствующие строки детей, если они до этого не втсречались в тексте
        if (!(tmp.children.isEmpty())) {
            for (int i=0; i < tmp.children.count(); i++) {
                if (!(outputText->contains(name + " -> " + tmp.children[i] + " [label=\"Родитель\"];\r\n")))
                    outputText->insert(outputText->length()," " + name + " -> " + tmp.children[i] + " [label=\"Родитель\"];\r\n");
            }
        }
        //Получить индексы начала и конца следующего имени в списке членов семьи
        idStartName = idEndName + 1;
        idEndName = familyList->indexOf("\n", idStartName);
    } while (idEndName != -1);
    outputText->insert(outputText->length(),"}"); //Добавить строки для конца инициализации графа в выходной текст
}
