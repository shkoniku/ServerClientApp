#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_12);
    if (in.status() == QDataStream::Ok)
    {
        ui->textBrowser->append("server is online");
        QStringList letter;
        in >> letter;
        AddElementsToTreeView(letter);
    }
    else
    {
        ui->textBrowser->append("server is unavailable");
    }
}


void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1", 8000);
    SendToServer("get");
}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << str;
    socket->write(Data);
}


void MainWindow::AddElementsToTreeView(QStringList &letter)
{
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->setHeaderLabels({"Block", "Board", "Port"});
    foreach (QString str, letter) {
        QStringList values = str.split('/');
        if (values[0] == "blocks;")
        {
            QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget);
            root->setText(0, "ID: "+values[1]);
            QTreeWidgetItem *Name = new QTreeWidgetItem(root), *Ip = new QTreeWidgetItem(root), *BoardCount = new QTreeWidgetItem(root),
                    *MtR = new QTreeWidgetItem(root), *MtC = new QTreeWidgetItem(root), *Description = new QTreeWidgetItem(root),
                    *Label = new QTreeWidgetItem(root);
            Name->setText(0, "Name: "+values[2]);
            Ip->setText(0, "IP: "+values[3]);
            BoardCount->setText(0, "BoardCount: "+values[4]);
            MtR->setText(0, "MtR: "+values[5]);
            MtC->setText(0, "MtC: "+values[6]);
            Description->setText(0, "Description: "+values[7]);
            Label->setText(0, "Label: "+values[8]);
            //root->addChildren({Name, Ip, BoardCount, MtR, MtC, Description, Label});
        }
        else if (values[0] == "boards;")
        {
            QTreeWidgetItem* item = ui->treeWidget->findItems("ID: "+values[1], Qt::MatchExactly, 0)[0];
            QTreeWidgetItem *Board = new QTreeWidgetItem(item), *Num = new QTreeWidgetItem(Board),
                    *Name = new QTreeWidgetItem(Board), *PortCount = new QTreeWidgetItem(Board), *IntLinks  = new QTreeWidgetItem(Board),
                    *Algoritms = new QTreeWidgetItem(Board);
            Board->setText(1, "ID: "+values[2]);
            Num->setText(1,"Num: "+ values[3]);
            Name->setText(1, "Name: "+values[4]);
            PortCount->setText(1, "PortCount: "+values[5]);
            IntLinks->setText(1, "IntLinks: "+values[6]);
            Algoritms->setText(1, "Algoritms: "+values[7]);
            Board->addChildren({Num, Name, PortCount, IntLinks, Algoritms});
            item->addChild(Board);
        }
        else if (values[0] == "ports;")
        {
            QTreeWidgetItem* item = ui->treeWidget->findItems("ID: "+values[1], Qt::MatchExactly | Qt::MatchRecursive, 1)[0];
            QTreeWidgetItem *Port = new QTreeWidgetItem(item), *Num = new QTreeWidgetItem(Port),
                    *Media = new QTreeWidgetItem(Port), *Signal = new QTreeWidgetItem(Port);
            Port->setText(2, "ID: "+values[2]);
            Num->setText(2, "Num: "+values[3]);
            Media->setText(2, "Media: "+values[4]);
            Signal->setText(2, "Signal: "+values[5]);
            Port->addChildren({Num, Media, Signal});
            item->addChild(Port);
        }
    }
}
