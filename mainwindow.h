/***********************************************************
* Команда 2
* Текстовый редактор
*
* Файл заголовков класса главного окна текстового редактора
************************************************************/

#pragma once

#include "filemanager.h"
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
    /*!
     * \brief GetMdi Геттер указателя на объект QMdiArea главного окна
     * \return Возвращает указатель на объект QMdiArea главного окна
     */
    QMdiArea* GetMdi() { return _pMdiArea; }
    /*!
     * \brief OpenFile Метод открытия файла в дочернем окне
     * \param fullFileName Полное имя файла: путь и имя
     * \return Признак успешного чтения файла
     */
    bool OpenFile(const QString& pathFileName);
    /*!
     * \brief CreateNewDocument Метод создаёт экземпляр дочернего MDI окна документа
     * \return Возвращает экземпляр дочернего MDI окна документа
     */
    DocumentWindow* CreateNewDocument();

protected:
    /*!
     * \brief closeEvent Перегруженный метод закрытия виджета
     * \param event Указатель на объект с параметрами события
     */
    void closeEvent(QCloseEvent *event) override;

private:
    /*!
     * \brief GetActiveDocumentWindow Возвращает указатель на текущий активный документ
     * \return Указатель на текущий активный документ
     */
    DocumentWindow* GetActiveDocumentWindow() const;

public slots:
    /*!
     * \brief SlotStatusBarMessage Слот - вывод сообщения в статусбаре главного окна
     */
    void SlotStatusBarMessage(const QString&);

private slots:
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
    /// Слот вырезать текст
    void SlotCut();
    /// Слот копировать текст
    void SlotCopy();
    /// Слот вставить текст
    void SlotPaste();
    /// Слот сделать активными/не активными эементы интерфеса, если документ открыт
    void SlotUpdateMenus();
    /*!
     * \brief SlotSetActiveSubWindow Слот делает дочернего MDI окно активным
     * \param QObject* Указатель на виджет дочернего MDI окна документа
     */
    void SlotSetActiveSubWindow(QObject*);

private:
    QMdiArea* _pMdiArea;            // указатель на MDI виджет
    QMenu* _pMenuWindows;           // указатель на виджет меню
    QSignalMapper* _pSignalMapper;  // указатель на мапер сигналов
    QToolBar* _pToolBar;            // указатель на Toolbar
    FileManager* _pFileManager;     // указатель на FileManager - файловый менеджер
    QDockWidget* _pDocWidget;       // указатель на DocWidget файлового менеджера

    QAction* _pSaveAct;             // указатель на действие "Сохранить"
    QAction* _pSaveAsAct;           // указатель на действие "Сохранить как"
    QAction* _pCutAct;              // указатель на действие "Вырезать"
    QAction* _pCopyAct;             // указатель на действие "Копировать"
    QAction* _pPasteAct;            // указатель на действие "Вставить"
};

