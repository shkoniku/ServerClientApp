#include <QDataStream>
#include <QDebug>
#include "xmlParser.h"

Parser::Parser()
{
}


void Parser::Parse(DbCreator &creator)
{
    QStringList files = directory.entryList(QStringList() << "*.xml", QDir::Files);    
    foreach (QString nameFile, files) {
        QFile* file = new QFile(filesPath+"/"+nameFile);
        QString blockId = "0", boardId = "0";
        if (file->open(QIODevice::ReadOnly | QIODevice::Text) && QFileInfo::exists(filesPath+"/"+nameFile) && QFileInfo(filesPath+"/"+nameFile).isFile())
        {
            qDebug() << "ok";
            QXmlStreamReader xml(file);
            while (!xml.atEnd() && !xml.hasError())
            {
                QXmlStreamReader::TokenType token = xml.readNext();
                if (token == QXmlStreamReader::StartDocument)
                {
                    continue;
                }
                if (token == QXmlStreamReader::StartElement)
                {

                    if (xml.name() == "block")
                    {
                        QString attributs[8] = {NULL};
                        for (int iterator = 0; iterator < xml.attributes().size(); iterator++)
                        {
                            if (iterator == 0)
                            {
                                blockId = xml.attributes()[0].value().toString();
                            }
                            attributs[iterator] = xml.attributes()[iterator].value().toString();
                        }
                        creator.InsertDataToBlocks(attributs[0], attributs[1], attributs[2],
                                            attributs[3], attributs[4], attributs[5],
                                            attributs[6], attributs[7]);
                    }

                    if (xml.name() == "board")
                    {
                        QString attributs[6] = {NULL};
                        for (int iterator = 0; iterator < xml.attributes().size(); iterator++)
                        {
                            if (iterator == 0)
                            {
                                boardId = xml.attributes()[0].value().toString();
                            }
                            attributs[iterator] = xml.attributes()[iterator].value().toString();
                        }
                        creator.InsertDataToBoards(blockId, attributs[0], attributs[1], attributs[2],
                                            attributs[3],  attributs[4],  attributs[5]);
                    }

                    if (xml.name() == "port")
                    {
                        QString attributs[4] = {NULL};
                        for (int iterator = 0; iterator < xml.attributes().size(); iterator++)
                        {
                            attributs[iterator] = xml.attributes()[iterator].value().toString();
                        }
                        creator.InsertDataToPorts(boardId, attributs[0], attributs[1],
                                          attributs[2], attributs[3]);
                    }
                }
            }
        }
        else
        {
            qDebug() << "cant open file";
        }
        file->close();
    }

}
