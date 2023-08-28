#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotReadyRead();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    void SendToServer(QString str);
    void AddElementsToTreeView(QStringList &letter);
};
#endif // MAINWINDOW_H
