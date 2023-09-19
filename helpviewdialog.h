/*************************************************
* Команда 2
* Гипертекстовый редактор
*
* Файл заголовков класса окна диалога помощи
**************************************************/

#pragma once

#include <QDialog>

namespace Ui {
class HelpViewDialog;
}

/*!
 * \brief The HelpViewDialog Класс диалога помощи
 */
class HelpViewDialog : public QDialog
{
    Q_OBJECT

public:
    /// Конструктор
    explicit HelpViewDialog(QWidget *parent = nullptr);

    /// Деструктор
    ~HelpViewDialog();

private:
    Ui::HelpViewDialog *ui;
};
