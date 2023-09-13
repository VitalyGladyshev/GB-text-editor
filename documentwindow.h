/*************************************************
* Команда 2
* Текстовый редактор
*
* Файл заголовков класса документа в шаблоне MDI
**************************************************/

#pragma once

#include <QTextEdit>

class MainWindow;

/*!
 * \brief Класс документа в MDI шаблоне
 */
class DocumentWindow : public QTextEdit
{
    Q_OBJECT

public:
    /// Конструктор
    DocumentWindow(QWidget* pParent = nullptr);

    /*!
     * \brief OpenFile Метод загрузки фала и чтения из него текста
     * \param fullFileName Полное имя файла: путь и имя
     * \return Признак успешного чтения файла
     */
    bool OpenFile(const QString& pathFileName);

    /*!
     * \brief GetPathFileName Геттер полного имени файла: путь и имя
     * \return Полное имя файла: путь и имя
     */
    QString GetPathFileName() const { return _pathFileName; }

    /*!
     * \brief Load Метод загрузки документа
     * \return Признак успешного чтения
     */
    bool Load();

    /// Метод сохранение документа
    bool Save();

    /// Метод сохранить документ как
    bool SaveAs();

    /// Метод сохранение файла
    bool SaveFile(QString&);

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

    /*!
     * \brief TextFamily устанавливает семейство шрифта
     * \param f
     */
    void TextFamily(const QString &f);

    /*!
     * \brief MergeFormatOnWordOrSelection установка формата текста в выделение или слово активного документа
     * \param format ссылка на форматирование
     */
    void MergeFormatOnWordOrSelection(const QTextCharFormat &format);

    /*!
     * \brief Find Поиск в тексте
     * \param searchRequest Поисковый запрос
     * \param wholeText Флаг поиска "Слово целиком"
     * \param caseSensitive Флаг поиска "С учётом регистра"
     */
    void Find(QString searchRequest, bool wholeText, bool caseSensitive);

signals:
    /*!
     * \brief SignalStatusBarMessage Сигнал - выод сообщения в статусбаре главного окна
     */
    void SignalStatusBarMessage(QString);

protected:
    /*!
     * \brief closeEvent Перегруженный метод закрытия виджета
     * \param event Указатель на объект с параметрами события
     */
    void closeEvent(QCloseEvent *event) override;

private:
    QString _pathFileName;      // полное имя файла: путь и имя
};
