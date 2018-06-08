#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    nTimers=0;

    connect(ui->pushButtonConnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));

    connect(ui->pushButtonDisconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpDisconnect()));

    connect(ui->pushButtonStart,
            SIGNAL(clicked(bool)),
            this,
            SLOT(startButtom()));

    connect(ui->pushButtonStop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(stopButtom()));
}

void MainWindow::startButtom(){
    if(nTimers==0){

        str="<font color=\"red\">Start</font></i>";
        ui->textBrowser->append(str);

        id=startTimer(1000/ui->horizontalSliderTiming->value());

        nTimers=1;
    }
}

void MainWindow::stopButtom(){
    if(nTimers==1){

        str="<font color=\"red\">Stop</font></i>";
        ui->textBrowser->append(str);

        killTimer(id);

        nTimers=0;
    }
}

void MainWindow::tcpConnect(){
    socket->connectToHost(ui->lineEdit->text(),1234);
    if(socket->waitForConnected(3000)){
        str= "<font color=\"red\">Connected</font></i>";
        qDebug() << "Connected";
        ui->textBrowser->append(str);
    }
    else{
        str="<font color=\"red\">Disconnected</font></i>";
        qDebug() << "Disconnected";
        ui->textBrowser->append(str);
    }
}

void MainWindow::tcpDisconnect(){
    socket->disconnectFromHost();
    if(socket->waitForConnected(3000)){
        str="<font color=\"red\">Connected</font></i>";
        qDebug() << "Disconnected";
        ui->textBrowser->append(str);
    }
    else{
        str="<font color=\"red\">Disconnected</font></i>";
        qDebug() << "Disconnected";
        ui->textBrowser->append(str);
    }
}

void MainWindow::putData(){
    QDateTime datetime;
    qint64 msecdate;

    if(socket->state()== QAbstractSocket::ConnectedState){

        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
        str = "set "+ QString::number(msecdate) + " " +
                QString::number(((double)qrand()/RAND_MAX)*(ui->horizontalSliderMax->value()-ui->horizontalSliderMin->value())+
                                ui->horizontalSliderMin->value());

        ui->textBrowser->append(str);
        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
        if(socket->waitForBytesWritten(3000)){
            qDebug() << "wrote";
        }
    }
}

MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    putData();
}
