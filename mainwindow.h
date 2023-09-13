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
class QMdiSubWindow;
class QSignalMapper;
class DocumentWindow;
class QFontComboBox;
class QTextCharFormat;
class QTextList;
class FindDialog;


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

    /*!
     * \brief GetActiveDocumentWindow Возвращает указатель на текущий активный документ
     * \return Указатель на текущий активный документ
     */
    DocumentWindow* GetActiveDocumentWindow();

protected:
    /*!
     * \brief closeEvent Перегруженный метод закрытия виджета
     * \param event Указатель на объект с параметрами события
     */
    void closeEvent(QCloseEvent *event) override;

private:

    /*!
     * \brief FontChanged метод формирует отобажение конфигурации шрифта в toolbar главного окна в соответствии с принятым шрифтом
     * \param f шрифт
     */
    void FontChanged(const QFont &f);

    /*!
     * \brief SetupTextActions метод создает панели и меню конфигурирования шрифта
     */
    void SetupTextActions();

    /*!
     * \brief SetupBoldActions метод создает панели и меню конфигурирования включения/выключения жирного шрифта
     * \param toolBar указатель на тулбар
     * \param menu указатель на меню
     */
    void SetupBoldActions(QToolBar* toolBar, QMenu* menu);

    /*!
     * \brief SetupBoldActions метод создает панели и меню конфигурирования включения/выключения курсивного шрифта
     * \param toolBar указатель на тулбар
     * \param menu указатель на меню
     */
    void SetupItalicActions(QToolBar* toolBar, QMenu* menu);

    /*!
     * \brief SetupBoldActions метод создает панели и меню конфигурирования включения/выключения подчеркнутого шрифта
     * \param toolBar указатель на тулбар
     * \param menu указатель на меню
     */
    void SetupUnderLineActions(QToolBar* toolBar, QMenu* menu);

    /*!
     * \brief SetupActiveDocument метод удаляет не актуальные соединения сигнал/слот, получает указатель на активное окно класса DocumentWindow и формирует актуальные соединения сигнал/слот
     * \param window указатель на активное окно класса QMdiSubWindow
     */
    void SetupActiveDocument (QMdiSubWindow* window);

    /*!
     * \brief ConnectToActiveDocument метод создания соединений панели и меню конигурирования шрифтов с активным окном класса DocumentWindow
     */
    void ConnectToActiveDocument ();

    /*!
     * \brief ConnectToActiveDocument метод удаления соединений панели и меню конигурирования шрифтов с уже неактивным окном класса DocumentWindow
     */
    void DisonnectFromDocument ();

    /*!
     * \brief CurrentCharFormatChanged метод формирует отобажение формата текста в toolbar главного окна в соответствии с форматированием
     * \param format ссылка на форматирование
     */
    void CurrentCharFormatChanged(const QTextCharFormat &format);

    /*!
     * \brief MergeFormatOnWordOrSelection установка формата текста в выделение или слово активного документа
     * \param format ссылка на форматирование
     */
    //void MergeFormatOnWordOrSelection(const QTextCharFormat &format);


public slots:
    /*!
     * \brief SlotStatusBarMessage Слот - вывод сообщения в статусбаре главного окна
     */
    void SlotStatusBarMessage(QString);

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

    /// Слот поиск в тексте
    void SlotFind();

    /// Слот сделать активными/не активными эементы интерфеса, если документ открыт
    void SlotUpdateMenus();

    /*!
     * \brief SlotSetActiveSubWindow Слот делает дочернего MDI окно активным
     * \param QObject* Указатель на виджет дочернего MDI окна документа
     */
    void SlotSetActiveSubWindow(QObject*);

private:
    QMdiArea* _pMdiArea;                // указатель на MDI виджет
    QMenu* _pMenuWindows;               // указатель на виджет меню
    QSignalMapper* _pSignalMapper;      // указатель на мапер сигналов
    QToolBar* _pToolBar;                // указатель на Toolbar
    FileManager* _pFileManager;         // указатель на FileManager - файловый менеджер
    QDockWidget* _pDocWidget;           // указатель на DocWidget файлового менеджера
    DocumentWindow* _pCurrentDocument;  // указатель на активный виджет класса DocumentWindow
    FindDialog* _pFindDialog;           // указатель на FindDialog - диалог поиска

    QAction *actionTextBold;            // включение жирного шрифта
    QAction *actionTextUnderline;       // включение жирного подчеркнутого шрифта
    QAction *actionTextItalic;          // включение жирного курсивного шрифта
    QFontComboBox *comboFont;           // выбор семейства шрифта
    QAction* _pSaveAct;                 // указатель на действие "Сохранить"
    QAction* _pSaveAsAct;               // указатель на действие "Сохранить как"
    QAction* _pCutAct;                  // указатель на действие "Вырезать"
    QAction* _pCopyAct;                 // указатель на действие "Копировать"
    QAction* _pPasteAct;                // указатель на действие "Вставить"
    QAction* _pFindAct;                 // указатель на действие "Поиск"
};
