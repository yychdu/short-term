#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);
    tcpClient->abort();
    connect(tcpClient, SIGNAL(readyRead()), SLOT(ReadData()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ReadError(QAbstractSocket::SocketError)));

    QFile file("conf.ini");
    if (!file.exists())
    {
        FILE *fp = fopen("conf.ini", "w");
        if (fp)
            fclose(fp);
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray t = file.readAll();
        QString str(t);
        file.close();
        if (!t.isEmpty())
        {
            QStringList lst = str.split(":");
            if (lst.size() == 2)
            {
                ui->EditIP->setText(lst[0]);
                ui->EditPort->setText(lst[1]);
            }
        }
    }

//    tcpClient->connectToHost(ui->EditIP->text(), ui->EditPort->text().toUShort());
//    if (tcpClient->waitForConnected(1000))  // 连接成功则进入if{}
//    {
//        ui->BtnConn->setText("断开连接");
//        ui->BtnSend->setEnabled(true);
//    }
//    else
//    {
//        ui->BtnConn->setText("连接服务器");
//        ui->BtnSend->setEnabled(false);
//    }

    ui->BtnConn->setText("连接服务器");
    ui->BtnSend->setEnabled(false);
    ui->radioClient->setChecked(true);

    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
    ui->cbLstClients->setVisible(false);

//    showMaximized();

    //查找可用的串口
    ui->comboBox->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(serial.portName());
            serial.close();
        }
    }
    ui->comboBox_2->setCurrentIndex(4);
    ui->btnSend->setEnabled(false);
}

MainWindow::~MainWindow()
{
    if (tcpServer->isListening()) {
        for(int i = lstClient.length() - 1; i >= 0; --i) //断开所有连接
        {
            lstClient[i]->disconnectFromHost();
            lstClient.removeAt(i);  //从保存的客户端列表中取去除
        }
        tcpServer->close();     //不再监听端口
    }

    if (tcpClient->state() == QAbstractSocket::ConnectedState)
    {
        tcpClient->abort();
        tcpClient->disconnectFromHost();
        tcpClient->close();
    }
    delete ui;
}

void MainWindow::GetLocalIPAddress()
{
    QList<QHostAddress> lst = QNetworkInterface().allAddresses();
    for (int i = 0; i < lst.size(); ++i)
    {
        QHostAddress tha = lst[i];
        QString tstr = tha.toString();
        if (tha.isNull() || tha.isLoopback() || tha.protocol() != QAbstractSocket::IPv4Protocol)
            continue;
        else
        {
            ui->EditIP->setText(lst[i].toString());   // 显示本地IP地址
            break;
        }
    }
}

QByteArray MainWindow::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl = HexString.split(" ");

    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            uint32_t td = s.toUInt(&ok, 16);
            int pos = ret.size();
            if (ok)
            {
                do {
                    ret.insert(pos, td & 0xFF);
//                    ret.append(td & 0xFF);
                    td >>= 8;
                }while(td > 0);
            }
        }
    }
    qDebug()<<ret;
    return ret;
}

void MainWindow::ReadData()
{
    QByteArray buffer = tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        ui->EditRecv->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
//        ui->EditRecv->append(buffer);
        if (ui->chkHexDisp->isChecked())
        {
            QString tstr, t;
            for (int i = 0; i < buffer.size(); ++i)
            {
                t.sprintf("%02X ", buffer.data()[i]);
                tstr += t;
            }
            ui->EditRecv->insertPlainText(tstr);
        }
        else
            ui->EditRecv->insertPlainText(QString(buffer));
    }
}

void MainWindow::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    ui->BtnConn->setText(tr("连接服务器"));
    ui->EditRecv->append(tr("服务器连接错误：%1").arg(tcpClient->errorString()));
    ui->BtnSend->setEnabled(false);
}

void MainWindow::NewConnectionSlot()
{
    currentClient = tcpServer->nextPendingConnection();
    lstClient.append(currentClient);
    connect(currentClient, SIGNAL(readyRead()), this, SLOT(ServerReadData()));
    connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));

    if (ui->cbLstClients->count() == 0)
        ui->cbLstClients->addItem("全部连接");
    ui->cbLstClients->addItem(currentClient->peerAddress().toString());
    if (ui->cbLstClients->count() > 0)
        ui->BtnSend->setEnabled(true);
}

void MainWindow::disconnectedSlot()
{
    for(int i = lstClient.length() - 1; i >= 0; --i)
    {
        if(lstClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在combox中的客户端信息
            ui->cbLstClients->removeItem(ui->cbLstClients->findText(lstClient[i]->peerAddress().toString()));
            // 删除存储在tcpClient列表中的客户端信息
            lstClient[i]->destroyed();
            lstClient.removeAt(i);
        }
    }
    if (ui->cbLstClients->count() == 1)
    {
        ui->cbLstClients->clear();
        ui->BtnSend->setEnabled(false);
    }
}

void MainWindow::ServerReadData()
{
    // 由于readyRead信号并未提供SocketDecriptor，所以需要遍历所有客户端
    static QString IP_Port, IP_Port_Pre;
    for(int i = 0; i < lstClient.length(); ++i)
    {
        QByteArray buffer = lstClient[i]->readAll();
        if(buffer.isEmpty())
            continue;

        ui->EditRecv->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        IP_Port = tr("[%1:%2]:").arg(lstClient[i]->peerAddress().toString()).arg(lstClient[i]->peerPort());

        // 若此次消息的地址与上次不同，则需显示此次消息的客户端地址
        if(IP_Port != IP_Port_Pre)
            ui->EditRecv->append(IP_Port);

        if (ui->chkHexDisp->isChecked())
        {
            QString tstr, t;
            for (int i = 0; i < buffer.size(); ++i)
            {
                t.sprintf("%02X ", buffer.data()[i]);
                tstr += t;
            }
            ui->EditRecv->insertPlainText(tstr);
        }
        else
            {
             ui->EditRecv->insertPlainText(QString(buffer));
        }


        //更新ip_port
        IP_Port_Pre = IP_Port;
    }
}

void MainWindow::on_BtnConn_clicked()
{
    if (ui->radioClient->isChecked())
    {
        if (tcpClient->state() == QAbstractSocket::ConnectedState)
        {
            tcpClient->disconnectFromHost();    // 断开连接
            if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000))
            {
                ui->BtnConn->setText("连接服务器");
                ui->BtnSend->setEnabled(false);
            }
        }
        else
        {
            tcpClient->connectToHost(ui->EditIP->text(), ui->EditPort->text().toUShort());
            if (tcpClient->waitForConnected(1000))  // 连接成功则进入if{}
            {
                QFile file("conf.ini");
                if (file.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    file.write((ui->EditIP->text() + ":" + ui->EditPort->text()).toUtf8());
                    file.close();
                }
                ui->BtnConn->setText("断开连接");
                ui->BtnSend->setEnabled(true);
            }
            else
            {
                ui->EditRecv->append(tr("服务器连接错误：%1").arg(tcpClient->errorString()));
            }
        }
    }
    else {
        ui->cbLstClients->clear();
        if (tcpServer->isListening()) {
            for(int i = lstClient.length() - 1; i >= 0; --i) //断开所有连接
            {
                QTcpSocket *tt = lstClient.at(i);
                tt->disconnectFromHost();
                if (tt->state() == QAbstractSocket::UnconnectedState || tt->waitForDisconnected(1000))
                {
                 // 处理异常
                }
                lstClient.removeAt(i);  //从保存的客户端列表中取去除
            }
            tcpServer->close();     //不再监听端口
            ui->cbLstClients->clear();
            ui->BtnConn->setText("开始侦听");
            ui->BtnSend->setEnabled(false);
        }
        else {
            bool ok = tcpServer->listen(QHostAddress::AnyIPv4, ui->EditPort->text().toUShort());
            if(ok)
            {
                ui->BtnConn->setText("断开连接");
                ui->BtnSend->setEnabled(false);
            }
        }
    }
}

void MainWindow::on_BtnClearRecv_clicked()
{
    ui->EditRecv->clear();
}

void MainWindow::on_BtnSend_clicked()
{
    QString data = ui->EditSend->toPlainText();
    QByteArray tba;
    if (ui->chkHexSend->isChecked())
        tba = HexStringToByteArray(data);
    else
        tba = data.toLatin1();
    if (ui->radioClient->isChecked())
    {
        if(!data.isEmpty())
        {
            tcpClient->write(tba);
        }
    }
    else {
        //全部连接
        if(ui->cbLstClients->currentIndex() == 0)
        {
            for(int i=0; i < lstClient.length(); i++)
                lstClient[i]->write(tba);
        }
        else {
            QString clientIP = ui->cbLstClients->currentText();
            for(int i=0; i < lstClient.length(); i++)
            {
                if(lstClient[i]->peerAddress().toString() == clientIP)
                {
                    lstClient[i]->write(tba);
                    return; //ip:port唯一，无需继续检索
                }
            }
        }
    }
}

void MainWindow::on_radioClient_clicked()
{
    if (tcpClient->state() == QAbstractSocket::ConnectedState)
        return;

    // 服务器断开
    if (tcpServer->isListening())
    {
        for(int i = lstClient.size() - 1; i >= 0; --i) //断开所有连接
        {
            QTcpSocket *tt = lstClient.at(i);
            tt->disconnectFromHost();
            if (tt->state() == QAbstractSocket::UnconnectedState || tt->waitForDisconnected(1000))
            {

            }
            lstClient.removeAt(i);  //从保存的客户端列表中取去除
        }
        tcpServer->close();     //不再监听端口
    }
    ui->cbLstClients->clear();
    ui->cbLstClients->setVisible(false);
    ui->labelAddr->setText("服务器地址：");

    // 加载远程服务器地址、端口
    QFile file("conf.ini");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray t = file.readAll();
        QString str(t);
        file.close();
        if (!t.isEmpty())
        {
            QStringList lst = str.split(":");
            if (lst.size() == 2)
            {
                ui->EditIP->setText(lst[0]);
                ui->EditPort->setText(lst[1]);
            }
        }
    }

    if (tcpClient->state() == QAbstractSocket::UnconnectedState)
    {
        ui->BtnConn->setText("连接服务器");
        ui->BtnSend->setEnabled(false);
    }
    else {
        ui->BtnConn->setText("断开连接");
        ui->BtnSend->setEnabled(true);
    }
}

void MainWindow::on_radioServer_clicked()
{
    if (tcpServer->isListening())
        return;

    // 断开客户端
    tcpClient->disconnectFromHost();    // 断开连接
    if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000))
    {

    }

    // 获取本地IP地址
    GetLocalIPAddress();
    ui->BtnConn->setText("开始侦听");
    ui->BtnSend->setEnabled(false);

    ui->cbLstClients->clear();
    ui->cbLstClients->setVisible(true);
    ui->labelAddr->setText("本机地址：");
}





void MainWindow::on_btnOpen_clicked()
{
    int bps;
    if(ui->btnOpen->text() == "打开串口")
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->comboBox->currentText());
        //打开串口
         serial->open(QIODevice::ReadWrite);
        //设置波特率
        bps = ui->comboBox_2->currentText().toInt();
        serial->setBaudRate(bps);
        //设置数据位数
        serial->setDataBits(QSerialPort::Data8);
        //设置奇偶校验
        serial->setParity(QSerialPort::NoParity);
        //设置停止位
        serial->setStopBits(QSerialPort::OneStop);
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //关闭设置菜单使能
        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->btnOpen->setText("关闭串口");
        ui->btnSend->setEnabled(true);
        //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        //恢复设置使能
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->btnOpen->setText("打开串口");
        ui->btnSend->setEnabled(false);
    }
}

void MainWindow::on_btnSend_clicked()
{
    QString str = ui->textSend->text() + "\n";
    QByteArray buf = str.toLatin1();
    serial->write(buf);
}

void MainWindow::Read_Data()
{
    static QString frame_buff;
    QByteArray buf;
    buf = serial->readAll();
    #if 0
    if(!buf.isEmpty())
    {
        QString str = ui->textRecv->toPlainText();
       str += QString(buf);
        if (buf.size() > 4 && (buf[0] == '5' && buf[1] == 'M'))
        {
            if (buf[2] == 'Y' && buf[3] == 'D')
            {
                QString tstr = QString(buf.mid(4, 4));
                str += "AD:" + tstr + "\n";
            }
        }
        ui->textRecv->clear();
    ui->textRecv->append(str);
    }
#endif
    if(!buf.isEmpty())
    {
        frame_buff+=QString(buf);
        int len=frame_buff.indexOf("\n");
        while(len>=0)
        {
//             ui->textRecv->append(frame_buff.left(len));
            process_frame(frame_buff.left(len));
           frame_buff=frame_buff.mid(len+1);
           len=frame_buff.indexOf("\n");
        }
    }

    buf.clear();
}

void MainWindow::on_btnClear_clicked()
{
    ui->textRecv->clear();
    ui->textSend->clear();
}

bool MainWindow::check_frame(QString frame_str)
{
    QByteArray barry = frame_str.toLatin1();
    int len=barry.count();
    if (len>2)
    {
        bool bok;
        int chksum=frame_str.right(2).toInt(&bok,16);
        int sum=0;
        for(int i=0;i<len-2;++i)
            sum+=barry[i];
        sum%=256;
        if(sum==chksum)
            return true;

    }
    return false;
}

void MainWindow::process_frame(QString frame_str)
{
     frame_str=frame_str.trimmed();
     if(!check_frame(frame_str))
     {
         if(frame_str.indexOf("5M")==0){
         ui->textRecv->append(frame_str+"check sum error !\n");
         return;
         }
         else
         {
             ui->textRecv->append(frame_str+"\n");
             return;
         }
     }

     frame_str=frame_str.left(frame_str.length()-2);
     int i,j,k=0,m=0,n=0,sum=0,k1=0;
     int led_sta[4],btn_sta[4],data[6]={0};
     int adval=0,ax=0,ay=0,az=0,gx=0,gy=0,gz=0;
     double dataf[4]={0};
     QString tstr;
     if(frame_str.indexOf("5MBD")==0&&frame_str.length()>=12)
     {
         for(i=0;i<4;i++)
             led_sta[i]=(frame_str[4+i]=='0')?0:1;
         for (i=0;i<4;i++) {
             btn_sta[i]=(frame_str[8+i]=='U')?0:1;
         }
         tstr.sprintf("%s%s%s%s %s%s%s%s\n",
                        led_sta[0]?"○":"●",
                         led_sta[1]?"○":"●",
                         led_sta[2]?"○":"●",
                         led_sta[3]?"○":"●",
                         btn_sta[0]?"▂":"▅",
                         btn_sta[1]?"▂":"▅",
                         btn_sta[2]?"▂":"▅",
                         btn_sta[3]?"▂":"▅");
         ui->textRecv->append(tstr);
     }
     else if(frame_str.indexOf("5MY")==0&&frame_str.length()>9)
     {

             adval=frame_str.mid(4,4).toInt();

       for(i=8;i<frame_str.length();i++)
        {
            if(frame_str[i]==' '&&frame_str[i+1]!=' ')
            {
                 j=i+1;
                 k=frame_str.indexOf(' ',j);
                data[m++]=frame_str.mid(j,k-j).toInt();

            }

        }
        ax=data[0],ay=data[1],az=data[2],gx=data[3],gy=data[4],
                gz=data[5];
        tstr.sprintf("adval=%d\n ax=%d ay=%d az=%d\ngx=%d gy=%d gz=%d",
                    adval,ax,ay,az,gx,gy,gz );
            ui->textRecv->append(tstr);
            m=0;
     }
     else if(frame_str.indexOf("5MR")==0&&frame_str.length()>9)
     {
         for(i=5;i<frame_str.length();i++)
          {
             if(frame_str[i]==' '&&frame_str[i+1]!=' '&&frame_str[i+1]!='-')
             {//开头是空格
                  j=i+1;
                  k=frame_str.indexOf(' ',j);
                  k1=frame_str.indexOf('-',j);
                  if(k1<0)
                  {
                    k1=999;
                  }
                  k=k1<k?k1:k;
                 dataf[0]=frame_str.mid(j,k-j).toFloat();
                //vol
                 break;
             }
         }
            for(i=10;i<=15;i++)
            {
                if(frame_str[i]>='0'&&frame_str[i]<='9')
                {
                    if(frame_str[i-1]!='-')
                    dataf[1]=frame_str.mid(i,15-i+1).toFloat();
                    else
                    dataf[1]=frame_str.mid(i-1,15-i+1).toFloat();
                    break;
                }
            }
            for (i=16;i<21;i++) {
                if(frame_str[i]>='0'&&frame_str[i]<='9')
                {
                    if(frame_str[i-1]!='-')
                    dataf[2]=frame_str.mid(i,21-i+1).toFloat();
                    else
                     dataf[2]=frame_str.mid(i-1,21-i+1).toFloat();
                    break;
                }
            }
            for (i=22;i<28;i++) {
                if(frame_str[i]>='0'&&frame_str[i]<='9')
                {
                    if(frame_str[i-1]!='-')
                    dataf[3]=frame_str.mid(i,28-i+1).toFloat();
                    else
                     dataf[3]=frame_str.mid(i-1,28-i+1).toFloat();
                    break;
                }
            }

         tstr.sprintf("Vol:%.1f\n yaw:%.1f pitch:%.1f roll:%.1f",
                      dataf[0],dataf[1],dataf[2],dataf[3]);
             ui->textRecv->append(tstr);

     }
//     else
//     {
//         ui->textRecv->append(frame_str+"check sum error !\n");
//                return;
//     }
}



void MainWindow::on_textRecv_2_copyAvailable(bool b)
{

}
void addchecksum(char *str )
{
   const char TAB[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
         uint32_t sum=0;
          uint8_t i;
         int  len=strlen(str);
          for(i=0;i<len;++i)
          {
            if(str[i]!='\n'&&str[i]!=0)
              sum+=str[i];
            else break;
          }
          str[len]=TAB[(sum&0xff)>>4];
          str[len+1]=TAB[(sum&0x0f)];
          str[len+2]='\n';
          str[len+3]='\0';
}

void MainWindow::on_cmd1_clicked()
{
    int num,len;
    char str[]="5MHM";
    num=ui->frq->text().toInt();
    len=strlen(str);
    str[len]=num+'0';
    str[len+1]='\0';
    addchecksum(str);

    serial->write(str);
}

void MainWindow::on_cmd2_clicked()
{
    int num,len;
    char str[]="5MHS";
    num=ui->maxlight->text().toInt();
    len=strlen(str);
    str[len]=num+'0';
    str[len+1]='\0';
    addchecksum(str);

    serial->write(str);
}
