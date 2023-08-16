#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead,this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlokSize = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1",2323);
}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out<< qint16(0) << str;//после добавления qint16(0), сообщения будут записываться с 17 бита, т.к. первые два байта резервируются
    out.device()->seek(0);//переходим в самое начало блока
    out<<qint16(Data.size()-sizeof(qint16));
    socket->write(Data);
    ui->lineEdit->clear();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if (in.status()== QDataStream::Ok)
    {
        /*QString str;
        in>>str;
        ui->textBrowser->append(str);*/
        for (;;)
        {
            if (nextBlokSize==0)//если размер блока не известен, т.е. не равен 0
            {
                if (socket->bytesAvailable()<2)//для чтения доступно не меньше 2 байт
                {
                    break;
                }
                in >> nextBlokSize;
            }
            if (socket->bytesAvailable()<nextBlokSize)
            {
                break;
            }
            QString str;
            in>>str;
            nextBlokSize = 0;
            ui->textBrowser->append(str);
        }
    } else
    {
        ui->textBrowser->append("read error");
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());
}


void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}

