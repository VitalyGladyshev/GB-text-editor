/**********************************************
* Команда 2
* Гипертекстовый редактор
*
* Код класса окна диалога создания гиперссылки
***********************************************/

#include "hyperlinkdialog.h"
#include "ui_hyperlinkdialog.h"

HyperlinkDialog::HyperlinkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HyperlinkDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonHyperlink, SIGNAL(clicked(bool)),
            this, SLOT(SlotMakeHyperlink()));
}

HyperlinkDialog::~HyperlinkDialog()
{
    delete ui;
}

// Задать метку поля редактирования "Текст ссылки"
void HyperlinkDialog::SetLabelText(QString label)
{
    ui->labelLinkText->setText(label);
}

// Задать текст поля редактирования "Текст ссылки"
void HyperlinkDialog::SetLineEditText(QString text)
{
    ui->lineEditLinkText->setText(text);
}

// Задать метку поля редактирования "Цель ссылки"
void HyperlinkDialog::SetLabelTarget(QString label)
{
    ui->labelLinkTarget->setText(label);
}

// Задать надпись на кнопке "Создать ссылку"
void HyperlinkDialog::SetButtonLinkLabel(QString label)
{
    ui->pushButtonHyperlink->setText(label);
}

// Очистить поле текст ссылки
void HyperlinkDialog::ClearText()
{
    ui->lineEditLinkText->clear();
}

// Очистить поле "Цель ссылки"
void HyperlinkDialog::ClearTarget()
{
    ui->lineEditLinkTarget->clear();
}

// Слот добавить гиперссылку
void HyperlinkDialog::SlotMakeHyperlink()
{
    qDebug() << "Make link";
}
