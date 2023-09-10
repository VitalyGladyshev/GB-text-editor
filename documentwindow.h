/*************************************************
* Команда 2
* Текстовый редактор
*
* Файл заголовков класса документа в шаблоне MDI
**************************************************/

#pragma once

#include <QTextEdit>
#include <QTextList>
/*!
 * \brief Класс документа в MDI шаблоне
 */
class DocumentWindow : public QTextEdit
{
    Q_OBJECT

public:
    /// Конструктор
    DocumentWindow(QWidget* pParent = nullptr);

signals:
    /*!
     * \brief signalChangeTitle сигнал для задания заголовка окна
     * \param QString& ссылка на строку с названием файла для заголовка окна
     */
    void SignalChangeTitle(const QString&);

public slots:
    /// Слот загрузки документа
    void SlotLoad();
    /// Слот сохранения документа
    void SlotSave();
    /// Слот сохранить документ как
    void SlotSaveAs();
    //=========================================
    /*!
     * \brief TextBold установка жирного шрифта
     * \param checked включение/выключение жирного шрифта
     */
    void TextBold(bool checked);

    /*!
     * \brief TextBold установка подчеркнутого шрифта
     * \param checked включение/выключение подчеркнутого шрифта
     */
    void TextUnderline(bool checked);

    /*!
     * \brief TextBold установка курсивного шрифта
     * \param checked включение/выключение курсивного шрифта
     */
    void TextItalic(bool checked);

    void TextFamily(const QString &f);
    /*!
     * \brief MergeFormatOnWordOrSelection установка формата текста в выделение или слово активного документа
     * \param format ссылка на форматирование
     */
    void MergeFormatOnWordOrSelection(const QTextCharFormat &format);


};
