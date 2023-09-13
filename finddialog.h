/*************************************************
* Команда 2
* Текстовый редактор
*
* Файл заголовков класса окна диалога поиска
**************************************************/

#pragma once

#include <QDialog>

class MainWindow;

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

    /*!
     * \brief GetSearchRequest Возвращает поисковый запрос
     * \return Строка поиска
     */
    QString GetSearchRequest();

    /*!
     * \brief GetWTCheckBoxStatus Проверка флага "Слова целиком"
     * \return Флаг "Слова целиком"
     */
    bool GetWTCheckBoxStatus();

    /*!
     * \brief GetCSCheckBoxStatus Проверка флага "С учётом регистра"
     * \return Флаг "С учётом регистра"
     */
    bool GetCSCheckBoxStatus();

    /*!
     * \brief ClearRequest Очистка строки поиска
     */
    void ClearRequest();

protected:
    /*!
     * \brief closeEvent Перегруженный метод закрытия виджета
     * \param event Указатель на объект с параметрами события
     */
    void closeEvent(QCloseEvent *event) override;

private slots:
    /// Слот - нажатие клавиши "Поиск"
    void SlotButtonFind();

private:
    Ui::FindDialog *ui;             // Указатель на пользовательский интерфейс
    MainWindow* _mainWindow;        // указатель на главное окно
};
