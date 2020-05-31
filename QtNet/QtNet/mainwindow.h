#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void GetLocalIPAddress();
    QByteArray HexStringToByteArray(QString HexString);

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpClient;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> lstClient;
    QTcpSocket *currentClient;
    bool check_frame(QString frame_str);
       void process_frame(QString frame_str);

       QSerialPort *serial;

private slots:
    void ReadData();
    void ReadError(QAbstractSocket::SocketError);

    void NewConnectionSlot();
    void disconnectedSlot();
    void ServerReadData();

    void on_BtnConn_clicked();
    void on_BtnClearRecv_clicked();
    void on_BtnSend_clicked();
    void on_radioClient_clicked();
    void on_radioServer_clicked();
    void on_btnOpen_clicked();

       void on_btnSend_clicked();

       void Read_Data();

       void on_btnClear_clicked();

       void on_textRecv_2_copyAvailable(bool b);

       void on_cmd1_clicked();

       void on_cmd2_clicked();
};

#endif // MAINWINDOW_H
