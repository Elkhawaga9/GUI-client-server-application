#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qglobal.h>
#include <QTcpSocket>
#include <QTcpServer>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString ipclient,ipserver;
    int portclient,portserver;

private slots:
    void on_clientconnect_clicked();

    void on_clientdis_clicked();

    void on_clientsend_clicked();

    void readyRead();

    void readyReadS();

    void on_serverlisten_clicked();

    void on_serverclear_clicked();

    void on_serversend_clicked();

public slots:
    void newConnection();

private:
    Ui::MainWindow *ui;
    QTcpSocket *clientsocket,*connectedclient;
    QTcpServer *server;
};
#endif // MAINWINDOW_H
