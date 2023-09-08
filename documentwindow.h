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
    /// Метод сохранения документа
    void Save();
    /// Метод сохранить документ как
    void SaveAs();

signals:
    /*!
     * \brief SignalStatusBarMessage Сигнал - выод сообщения в статусбаре главного окна
     */
    void SignalStatusBarMessage(const QString&);

protected:
    /*!
     * \brief closeEvent Перегруженный метод закрытия виджета
     * \param event Указатель на объект с параметрами события
     */
    void closeEvent(QCloseEvent *event) override;

private:
    QString _pathFileName;      // полное имя файла: путь и имя
};
