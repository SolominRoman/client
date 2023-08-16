#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();//connect

    void on_pushButton_2_clicked();//->

    void on_lineEdit_returnPressed();//enter ввод сообщения

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    void SendToServer(QString str);
    qint16 nextBlokSize;//число размаром 2 байта или 16 бит

public slots:
    void slotReadyRead();
};
#endif // MAINWINDOW_H
