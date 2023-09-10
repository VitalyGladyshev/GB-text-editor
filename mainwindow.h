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
class QMdiSubWindow;
class QSignalMapper;
class DocumentWindow;
class QFontComboBox;
class QTextCharFormat;
class QTextList;


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
    DocumentWindow* _pCurrentDocument; // указатель на активный виджет класса DocumentWindow

    QAction *actionTextBold;          // включение жирного шрифта
    QAction *actionTextUnderline;     // включение жирного подчеркнутого шрифта
    QAction *actionTextItalic;        // включение жирного курсивного шрифта
    QFontComboBox *comboFont;         // выбор семейства шрифта

};
