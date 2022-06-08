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

    buildStructScheme(&familyText, &familyList);

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
        if (tmpStr.indexOf("\"@!#$;%^:&?*()\'/,.`~") != -1)
            return 2;
        if ((tmpStr.split(" ").count() + tmpStr.split("\t").count()) != 8)
            return 3;
        if (!(tmpStr.contains("мать") || tmpStr.contains("отец") || tmpStr.contains("сын") || tmpStr.contains("дочь") || tmpStr.contains("бабушка") || tmpStr.contains("дед") ||
              tmpStr.contains("дедушка") || tmpStr.contains("брат") || tmpStr.contains("сестра")))
            return 4;
        idOldStr += idNewStr + 2;
        idNewStr = familyText->indexOf("\r\n", idOldStr);
    } while (idNewStr != -1);
    return 0;
}

int buildStructScheme(QString* familyText, QString* familyList) {

}

int testLogic() {
    return 0;
}

void completingSchema() {
}

void buildSchemeTree(QString* outputText, QString* familyList) {
}
