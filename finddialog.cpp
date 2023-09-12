#include "finddialog.h"
#include "ui_finddialog.h"

// Конструктор
FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);

    connect(ui->pushFindButton, SIGNAL(clicked(bool)), this, SLOT(SlotButtonFind()));
}

// Деструктор
FindDialog::~FindDialog()
{
    delete ui;
}

// Задать надпись на кнопке
void FindDialog::SetButtonLabel(QString label)
{
    ui->pushFindButton->setText(label);
}

// Задать метку чекбокса "Текст полностью"
void FindDialog::SetWTCheckBoxLabel(QString label)
{
    ui->checkBoxWholeText->setText(label);
}

// Слот - нажатие клавиши "Поиск"
void FindDialog::SlotButtonFind()
{
    qDebug() << "Find button";
}
