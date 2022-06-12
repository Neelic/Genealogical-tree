#include <QCoreApplication>
#include "header.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Проверка на существование файла
    if (!(QFile::exists(argv[1]))) {
        qDebug() << "Файл не сущетсвует";
        return 0;
    }

    QString familyText;
    QFile fileIn(argv[1]);
    //Считать файл, если он открыт для чтения
    if (fileIn.open(QIODevice::ReadOnly)) {
        familyText = fileIn.readAll();
        fileIn.close();
    }
    else {
        qDebug() << "Не удалось считать файл";
        return 0;
    }

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

    error = 0;
    error = buildStructScheme(&familyText, &familyList);

    QString outputText;
    buildSchemeTree(&outputText, &familyList);

    return 0;
}

int testInputData (QString* familyText) {
    if (familyText->isEmpty())
        return 1;
    int idNewStr = familyText->indexOf("\r\n");
    int idOldStr = 0;
    QString tmpStr;
    do {
        if (idNewStr > 0)
            tmpStr = familyText->mid(idOldStr,idNewStr - idOldStr);
        else
            tmpStr = QString(tmpStr + familyText);
        if (tmpStr.indexOf("\"") != -1 || tmpStr.indexOf("@") != -1 || tmpStr.indexOf("!") != -1 || tmpStr.indexOf("#") != -1 || tmpStr.indexOf("$") != -1
                || tmpStr.indexOf(";") != -1 || tmpStr.indexOf("%") != -1 || tmpStr.indexOf("^") != -1 || tmpStr.indexOf(":") != -1 || tmpStr.indexOf("&") != -1
                || tmpStr.indexOf("?") != -1 || tmpStr.indexOf("*") != -1 || tmpStr.indexOf("(") != -1 || tmpStr.indexOf(")") != -1 || tmpStr.indexOf("\\") != -1
                || tmpStr.indexOf("'") != -1 || tmpStr.indexOf("/") != -1 || tmpStr.indexOf(",") != -1 || tmpStr.indexOf(".") != -1 || tmpStr.indexOf("`") != -1
                || tmpStr.indexOf("~") != -1)
            return 2;
        if ((tmpStr.split(" ").count()) != 7)
            return 3;
        if (!(tmpStr.contains("мать") || tmpStr.contains("отец") || tmpStr.contains("сын") || tmpStr.contains("дочь") || tmpStr.contains("бабушка") || tmpStr.contains("дед") ||
              tmpStr.contains("дедушка") || tmpStr.contains("брат") || tmpStr.contains("сестра") || tmpStr.contains("внук") || tmpStr.contains("внучка")))
            return 4;
        idOldStr += idNewStr + 2;
        idNewStr = familyText->indexOf("\r\n", idOldStr);
    } while (idNewStr != -1);
    return 0;
}

int buildStructScheme(QString* familyText, QString* familyList) {
    int idNewStr = familyText->indexOf("\r\n");
    int idOldStr = 0;
    do {
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
        QString name1 = familyText->mid(idOldStr, idKinship - idOldStr - 1);
        QString name2 = familyText->mid(familyText->indexOf(" ",idKinship) + 1,idNewStr - familyText->indexOf(" ",idKinship) - 1);
        QString kinship = familyText->mid(idKinship, familyText->indexOf(" ", idKinship) - idKinship);
        human tmp;
        if (!(allPeople.contains(name1))) {
            familyList->insert(familyList->lastIndexOf("\n") + 1,name1 + "\n");
        }
        else
            tmp = allPeople.value(name1);

        //Первый человек
        if (kinship == "мать" || kinship == "отец") {
             int count = tmp.children.count();
             for (int i = 1; i <= count; i++) {
                 if (tmp.children[i - 1] == name2)
                     return 1;
             }
             tmp.children << name2;
        } else if (kinship == "сын" || kinship == "дочь") {
                if (tmp.parent1.contains(name2) || tmp.parent2.contains(name2))
                    return 1;
                if (!(tmp.parent1.isEmpty())) {
                   if (!(tmp.parent2.isEmpty()))
                      return 2;
                    else
                      tmp.parent2 = name2;
            } else
                tmp.parent1 = name2;
        } else if (kinship == "бабушка" || kinship == "дед" || kinship == "дедушка") {
            int count = tmp.grandChildren.count();
            for (int i = 1; i <= count; i++) {
                if (tmp.grandChildren[i - 1] == name2)
                    return 1;
            }
            tmp.grandChildren << name2;
        } else if (kinship == "брат" || kinship == "сестра") {
                int count = tmp.sibling.count();
                for (int i = 1; i <= count; i++) {
                    if (tmp.sibling[i - 1] == name2)
                        return 1;
                }
                tmp.sibling << name2;
        } else if (kinship == "внук" || kinship == "внучка") {
            if (tmp.grandParent1.contains(name2) || tmp.grandParent2.contains(name2) || tmp.grandParent3.contains(name2) || tmp.grandParent4.contains(name2))
                return 1;
            if (!(tmp.grandParent1.isEmpty())) {
                if (!(tmp.grandParent2.isEmpty())) {
                    if (!(tmp.grandParent3.isEmpty())) {
                        if (!(tmp.grandParent4.isEmpty()))
                            return 2;
                        else
                            tmp.grandParent4 = name2;
                    } else
                        tmp.grandParent3 = name2;
                }
                else
                    tmp.grandParent2 = name2;
            } else
                tmp.grandParent1 = name2;
        }
        allPeople.insert(name1,tmp);

        //Второй человек
        human tmp1;
        if (!(allPeople.contains(name2))) {
                familyList->insert(familyList->lastIndexOf("\n") + 1,name2 + "\n");
         }
            else
                tmp1 = allPeople.value(name2);

            if (kinship == "мать" || kinship == "отец") {
                 if (tmp1.parent1.contains(name1) || tmp1.parent2.contains(name1))
                     return 1;
                 if (!(tmp1.parent1.isEmpty())) {
                    if (!(tmp1.parent2.isEmpty()))
                       return 2;
                     else
                       tmp1.parent2 = name1;
                 } else
                        tmp1.parent1 = name1;
                } else if (kinship == "сын" || kinship == "дочь") {
                    int count = tmp1.children.count();
                    for (int i = 1; i <= count; i++) {
                        if (tmp1.children[i - 1] == name1)
                            return 1;
                    }
                    tmp1.children << name1;
                } else if (kinship == "бабушка" || kinship == "дед" || kinship == "дедушка") {
                    if (tmp1.grandParent1.contains(name1) || tmp1.grandParent2.contains(name1) || tmp1.grandParent3.contains(name1) || tmp1.grandParent4.contains(name1))
                        return 1;
                    if (!(tmp1.grandParent1.isEmpty())) {
                        if (!(tmp1.grandParent2.isEmpty())) {
                            if (!(tmp1.grandParent3.isEmpty())) {
                                if (!(tmp1.grandParent4.isEmpty()))
                                    return 2;
                                else
                                    tmp1.grandParent4 = name1;
                            } else
                                tmp1.grandParent3 = name1;
                        }
                        else
                            tmp1.grandParent2 = name1;
                    } else
                        tmp1.grandParent1 = name1;
                } else if (kinship == "брат" || kinship == "сестра") {
                    int count = tmp1.sibling.count();
                    for (int i = 1; i <= count; i++) {
                        if (tmp1.sibling[i - 1] == name2)
                            return 1;
                    }
                    tmp1.sibling << name2;
                } else if (kinship == "внук" || kinship == "внучка") {
                int count = tmp.grandChildren.count();
                for (int i = 1; i <= count; i++) {
                    if (tmp.grandChildren[i - 1] == name2)
                        return 1;
                }
                tmp.grandChildren << name2;
            }
        allPeople.insert(name2,tmp1);
        idOldStr = idNewStr + 2;
        idNewStr = familyText->indexOf("\r\n",idOldStr);

    } while (idNewStr != -1);

    int error = 0;
    error = testLogic(familyList);

    if (error == 0)
        completingSchema(familyList);

    return 0;
}

int testLogic(QString* familyList) {
    int idStartName = 0;
    int idEndName = familyList->indexOf("\n");
    int error = 0;
    do {
        human tmp;
        QString name = familyList->mid(idStartName, idEndName - idStartName);
        QString patr = name.right(name.length() - name.lastIndexOf(" ") - 1);
        patr.remove(patr.length() - 3, 3);
        tmp = allPeople.value(name);

        //Проверка, чтобы у родственников была только уникальная связь
        error = tmp.humanRepetition(name, &tmp);

        //Проверка отчества у братьев/сестер, если они есть
        if (!(tmp.sibling.isEmpty()) && error == 0) {
            for (int i = 0; i < tmp.sibling.count() && error == 0; i++) {
                QString siblingPatr = tmp.sibling[i].right(tmp.sibling[i].length() - tmp.sibling[i].lastIndexOf(" "));
                if (!(siblingPatr.contains(patr)))
                    error = 3;
            }
        }

        idStartName = idEndName + 1;
        idEndName = familyList->indexOf("\n", idStartName);
    } while (idEndName != -1);

    return error;
}

void completingSchema(QString* familyList) {
}

void buildSchemeTree(QString* outputText, QString* familyList) {
}

