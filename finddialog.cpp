/****************************************
* Команда 2
* Текстовый редактор
*
* Код класса диалога поиска
*****************************************/

#include <QMessageBox>

#include "finddialog.h"
#include "ui_finddialog.h"
#include "mainwindow.h"
#include "documentwindow.h"

// Конструктор
FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);

    _mainWindow = dynamic_cast<MainWindow*>(parent);

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

// Возвращает поисковый запрос
QString FindDialog::GetSearchRequest()
{
    return ui->lineEdit->text();
}

// Проверка флага "Слова целиком"
bool FindDialog::GetWTCheckBoxStatus()
{
    return ui->checkBoxWholeText->isChecked();
}

// Проверка флага "С учётом регистра"
bool FindDialog::GetCSCheckBoxStatus()
{
    return ui->checkBoxCaseSensitive->isChecked();
}

// Очистка строки поиска
void FindDialog::ClearRequest()
{
    ui->lineEdit->clear();
}

// Слот - нажатие клавиши "Поиск"
void FindDialog::SlotButtonFind()
{
    if(!GetSearchRequest().isEmpty())
    {
        if(_mainWindow)
        {
            auto doc =_mainWindow->GetActiveDocumentWindow();
            if (doc)
                doc->Find(GetSearchRequest(),
                          GetWTCheckBoxStatus(),
                          GetCSCheckBoxStatus());
        }
    }
    else
        QMessageBox::warning(this, tr("MDI"), tr("Enter a search query"));
}
