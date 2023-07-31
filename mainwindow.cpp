#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clientsocket = new QTcpSocket (this);
    connectedclient = new QTcpSocket(this);
    connect(clientsocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()),this, SLOT(newConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//server
void MainWindow::on_serverlisten_clicked()
{
    ipserver=ui->serverip->text();
    portserver=ui->serverport->text().toInt();
    if(server->isListening())
    {
        server->close();
        ui->serverdisplay->clear();
        ui->serverhexa->clear();
        connectedclient->close();
    }
    if(!server->listen(QHostAddress(ipserver), portserver))
    {
        ui->serverstate->setText("IP : "+ipserver+" Port : "+QString::number(portserver)+" server couldn't start");
        ui->serverstate->setStyleSheet("color:red");
        return;
    }
    else
    {

        ui->serverstate->setText("Server started!");
        ui->serverstate->setStyleSheet("color:green");
    }
}

void MainWindow::newConnection()
{
    // need to grab the socket
    connectedclient = server->nextPendingConnection();
    ui->serverstate->setText("server connected with IP : "+ipserver+" Port : "+QString::number(portserver));
    connect(connectedclient,SIGNAL(readyRead()),this,SLOT(readyReadS()));
}


void MainWindow::on_serverclear_clicked()
{
    ui->serverip->clear();
    ui->serverport->clear();
    if(connectedclient->isOpen())
    {
        connectedclient->close();
        ui->serverdisplay->clear();
        ui->serverhexa->clear();
        ui->serverstate->setText("Server disconnected");
        ui->serverstate->setStyleSheet("color:red")
    }
}

void MainWindow::on_serversend_clicked()
{
    if(ui->servermessage->text()=="")
    {
        ui->servermessage->setPlaceholderText(" Please enter a message");
        ui->servermessage->setStyleSheet("color:red");
        return;
    }


    char buff[1024];
    if(connectedclient->isWritable())
    {
        QString msg=ui->servermessage->text();
        QByteArray bytearr = msg.toUtf8();
        memset(buff,'\0',sizeof(buff)+1);
        memcpy(buff,bytearr,msg.length());
        connectedclient->write(buff);
        connectedclient->flush();
        connectedclient->waitForBytesWritten(1000);
        ui->serverdisplay->append("Server : " + QString(buff));

        ui->serverhexa->append("Server : " +bytearr.toHex());
        ui->servermessage->clear();
    }
}
void MainWindow::readyReadS()
{
    QByteArray datarecievedfromclient;
    datarecievedfromclient = connectedclient->readAll();
    ui->serverdisplay->append("Client : " +QString(datarecievedfromclient));
    ui->serverhexa->append("Client : " +datarecievedfromclient.toHex());
}


//client
void MainWindow::on_clientconnect_clicked()
{
    ipclient=ui->clientip->text();
    portclient=ui->clientport->text().toInt();

    if(ipclient=="" || QString::number(portclient)=="")
    {
        ui->clientstate->setText("Please check your IP and Port addresses!");
        ui->clientstate->setStyleSheet("color:red");
        return;
    }

    ui->clientdisplay->clear();
    ui->clienthexa->clear();
    clientsocket->connectToHost(ipclient,portclient);

    if(clientsocket->waitForConnected(300))
    {
        ui->clientstate->setText("IP : "+ipclient+"Port : "+QString::number(portclient)+" connection established");
        ui->clientstate->setStyleSheet("color:green");
    }
    else
    {
        ui->clientstate->setText("IP : "+ipclient+" Port : "+QString::number(portclient)+" Connection could not be established");
        ui->clientstate->setStyleSheet("color:red");
    }
}

void MainWindow::on_clientdis_clicked()
{
    if(clientsocket->isOpen())
    {
        clientsocket->close();
        ui->clientstate->setText("IP : "+ipclient+"Port : "+QString::number(portclient)+" Disconnected");
        ui->clientstate->setStyleSheet("color:red");
    }

}


void MainWindow::on_clientsend_clicked()
{
    if(ui->clientmesage->text()=="")
    {
        ui->clientmesage->setPlaceholderText(" Please enter a message");
        ui->clientmesage->setStyleSheet("color:red");
        return;
    }


    char buf[1024];
    if(clientsocket->isWritable())
    {
        QString messag=ui->clientmesage->text();
        QByteArray bytearr = messag.toUtf8();
        memset(buf,'\0',sizeof(buf)+1);
        memcpy(buf,bytearr,messag.length());
        clientsocket->write(buf);
        clientsocket->flush();
        clientsocket->waitForBytesWritten(1000);
        ui->clientdisplay->append("Client : " + QString(buf));

        ui->clienthexa->append("Client : "+bytearr.toHex());
        ui->clientmesage->clear();
    }
}

void MainWindow::readyRead()
{
    QByteArray datarecievedfromserver;
    datarecievedfromserver = clientsocket->readAll();
    ui->clientdisplay->append("Server : "+QString(datarecievedfromserver));
    ui->clienthexa->append("Server : "+datarecievedfromserver.toHex());

}

