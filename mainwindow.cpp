#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QApplication>
#include <QTranslator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setStyleSheet("QPushButton {background-color: #1ec94c;"
                  "border-style: outset;"
                  "border-width: 5px;"
                  "border-color: beige;"
                  "border-radius: 10px;"
                  "border-color: beige;"
                  "height: 20px;}");

    t.load(":/translation/QtLanguage_ru.qm");
    qApp->installTranslator(&t);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog :: getOpenFileName(this);
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        QByteArray ba = file.readAll();
        ui->plainTextEdit->setPlainText(ba.data());
        }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)){
            QString s = ui->plainTextEdit->toPlainText();
            QByteArray ba = s.toUtf8();
            file.write(ba, ba.size());
        }}
}

void MainWindow::on_pushButton_3_clicked()
{
    QFile file(":/txt/reference.txt");
    if (file.open(QIODevice::ReadOnly)){

        QByteArray ba = file.readAll();
        ui->plainTextEdit->setPlainText(ba.data());
    }

}




void MainWindow::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog :: getOpenFileName(this);
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        QByteArray ba = file.readAll();
        ui->plainTextEdit->setPlainText(ba.data());
        ui->plainTextEdit->setReadOnly(ba.data());
    }
}


void MainWindow::on_rB_white_clicked()
{

    qApp->setStyleSheet("QMainWindow {background-color: #ededed}");
}

void MainWindow::on_rB_black_clicked()
{
    qApp->setStyleSheet("QMainWindow {background-color: #3b3737}"
                        "QRadioButton {color : white}");

}



void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QString s = ":/translation/QtLanguage_ru.qm";
    QString s2 = ":/translation/QtLanguage_eng.qm";

    if(index == 0){
        t.load(s);
    }
    else {
        t.load(s2);
    };
    qApp->installTranslator(&t);
    ui->retranslateUi(this);
}
