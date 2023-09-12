/*************************************************
* Команда 2
* Текстовый редактор
*
* Файл заголовков класса окна диалога поиска
**************************************************/

#pragma once

#include <QDialog>

namespace Ui
{
class FindDialog;
}

/*!
 * \brief FindDialog класс диалога поиска
 */
class FindDialog : public QDialog
{
    Q_OBJECT

public:
    /// Конструктор
    explicit FindDialog(QWidget *parent = nullptr);

    /// Деструктор
    ~FindDialog();

    /*!
     * \brief SetButtonLabel Задать надпись на кнопке
     * \param label Надпись на кнопке
     */
    void SetButtonLabel(QString label);

    /*!
     * \brief SetWTCheckBoxLabel Задать метку чекбокса "Текст полностью"
     * \param label Метка чекбокса
     */
    void SetWTCheckBoxLabel(QString label);

private:
    Ui::FindDialog *ui;     // Указатель на пользовательский интерфейс

private slots:
    /// Слот - нажатие клавиши "Поиск"
    void SlotButtonFind();
};
