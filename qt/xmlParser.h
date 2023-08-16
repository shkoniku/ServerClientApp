#ifndef XMLPARSER_H
#define XMLPARSER_H


#include <QXmlStreamReader>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include "DbCreator.h"


class Parser
{
private:
    QString filesPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/XMLS";
    QDir directory = QDir(filesPath);


public:
    Parser();
    void Parse(DbCreator &creator);
};

#endif // XMLPARSER_H
