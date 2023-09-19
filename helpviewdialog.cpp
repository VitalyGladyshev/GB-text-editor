/****************************************
* Команда 2
* Гипертекстовый редактор
*
* Код класса диалога помощи
*****************************************/

#include "helpviewdialog.h"
#include "ui_helpviewdialog.h"

HelpViewDialog::HelpViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpViewDialog)
{
    ui->setupUi(this);
}

HelpViewDialog::~HelpViewDialog()
{
    delete ui;
}
