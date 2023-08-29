/***********************************************************
* Команда 2
* Текстовый редактор
*
* Файл заголовков класса главного окна текстового редактора
************************************************************/

#pragma once

#include <QMainWindow>

class QMenu;
class QMdiArea;
class QSignalMapper;
class DocumentWindow;

/*!
 * \brief Класс главного окна текстового редактора
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// Конструктор
    MainWindow(QWidget *parent = nullptr);

private:
    /*!
     * \brief CreateNewDocument метод создаёт экземпляр дочернего MDI окна документа
     * \return DocumentWindow* возвращает экземпляр дочернего MDI окна документа
     */
    DocumentWindow* CreateNewDocument();

private slots:
    /*!
     * \brief SlotChangeWindowTitle слот для задания заголовка окна
     * \param QString& ссылка на строку с названием файла для заголовка окна
     */
    void SlotChangeWindowTitle(const QString&);
    /// Слот создания нового документа
    void SlotNewDoc();
    /// Слот загрузки документа
    void SlotLoad();
    /// Слот сохранения документа
    void SlotSave();
    /// Слот сохранить документ как
    void SlotSaveAs();
    /// Слот вызова окна "О программе"
    void SlotAbout();
    /// Слот меню "Окна"
    void SlotWindows();
    /*!
     * \brief SlotSetActiveSubWindow слот делает дочернего MDI окно активным
     * \param QObject* указатель на виджет дочернего MDI окна документа
     */
    void SlotSetActiveSubWindow(QObject*);

private:
    QMdiArea* _pMidiArea;           // указатель на MDI виджет
    QMenu* _pMenuWindows;           // указатель на виджет меню
    QSignalMapper* _pSignalMapper;  // указатель на мапер сигналов
};
