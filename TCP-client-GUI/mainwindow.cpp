#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TCP-client/tcpHandler.h"
#include "TCP-client/FileHandeling.h"
#include "TCP-client/FileEncryption.h"
#include "TCP-client/tableHandler.h"
#include "TCP-client/authenticationpopup.h"

QByteArray key = "EncryptionKey";
QString unencryptedFile = "received_file_unencrypted";
QString encryptedFile = "receive_file";

int Socket = -1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (Socket != -1){
        tcpHandler::getInstance()->disconnectTCPsocket(Socket);
    }
    Socket = tcpHandler::getInstance()->connectToServer();
    QString displaySocket = QString::number(Socket);
    ui->textBrowser->setText(displaySocket);
}

void MainWindow::on_pushButton_2_clicked()
{
    tcpHandler::getInstance()->disconnectTCPsocket(Socket);
    ui->textBrowser->setText("Disconnected.");
}

void MainWindow::on_pushButton_3_clicked()
{
    QString text = ui->textEdit->toPlainText();
    tcpHandler::getInstance()->sendCommand(Socket,text.toStdString());
    QString receivedMsg = QString::fromStdString(tcpHandler::getInstance()->recevieCommand(Socket));
    ui->textBrowser->setText(receivedMsg);
}

void MainWindow::on_sendFileButton_clicked()
{
    tcpHandler::getInstance()->sendCommand(Socket,"SEND_FILE");
    bool send = FileHandeling::getInstance()->sendFile(Socket,unencryptedFile);
    QString boolString = (send) ? "true" : "false";
    ui->textBrowser->setText(boolString);
}

void MainWindow::on_receiveFileButton_clicked()
{
    tcpHandler::getInstance()->sendCommand(Socket,"RECEIVE_FILE");
    bool receive = FileHandeling::getInstance()->receiveFile(Socket,unencryptedFile);
    QString boolString = (receive) ? "true" : "false";
    ui->textBrowser->setText(boolString);
}

void MainWindow::on_pushButton_4_clicked()
{
    AuthenticationDialog popup;
    popup.exec();
    QString authCommand = "AUTH " + popup.getUsername() + " " + popup.getPassword();
    tcpHandler::getInstance()->sendCommand(Socket, authCommand.toStdString());
    QString receivedMsg = QString::fromStdString(tcpHandler::getInstance()->recevieCommand(Socket));
    ui->textBrowser->setText(receivedMsg);
}

void MainWindow::on_pushButton_5_clicked()
{
    FileEncryption::getInstance()->encryptFile(unencryptedFile,encryptedFile, key);

}

void MainWindow::on_pushButton_6_clicked()
{
    FileEncryption::getInstance()->decryptFile(encryptedFile,unencryptedFile, key);
}

void MainWindow::on_pushButton_7_clicked()
{
    tableHandler::getInstance()->displayCSVTable(unencryptedFile, ui->tableWidget);
}

void MainWindow::on_makeNewTableButton_clicked()
{
    tableHandler::getInstance()->createCsvTable(unencryptedFile);
    tableHandler::getInstance()->displayCSVTable(unencryptedFile, ui->tableWidget);
}

void MainWindow::on_commitToCSVFileButton_clicked()
{
    tableHandler::getInstance()->commitTableToCSV(unencryptedFile, ui->tableWidget);
}

void MainWindow::on_addRowToTableButton_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);

}

