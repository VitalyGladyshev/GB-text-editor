/*************************************************
* Команда 2
* Текстовый редактор
*
* Файл заголовков класса документа в шаблоне MDI
**************************************************/

#pragma once

#include <QTextEdit>

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
};
