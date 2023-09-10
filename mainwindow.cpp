/************************************************
* Команда 2
* Текстовый редактор
*
* Код класса главного окна текстового редактора
************************************************/

#include <QtWidgets>

#include "mainwindow.h"
#include "documentwindow.h"

MainWindow::MainWindow(QWidget *parent /* = nullptr */)
    : QMainWindow(parent)
{
    // Создание действия "Новый файл"
    QAction* pactNew = new QAction(tr("New File"), 0);
    pactNew->setText(tr("&New"));
//    pactNew->setShortcut(QKeySequence("CTRL+N"));
    pactNew->setToolTip(tr("New Document"));
    pactNew->setStatusTip(tr("Create a new file"));
    pactNew->setWhatsThis(tr("Create a new file"));
    pactNew->setIcon(QPixmap(":/images/icons/filenew.png"));
    connect(pactNew, SIGNAL(triggered()), SLOT(SlotNewDoc()));

    // Создание действия "Открыть файл"
    QAction* pactOpen = new QAction(tr("Open File"), 0);
    pactOpen->setText(tr("&Open..."));
//    pactOpen->setShortcut(QKeySequence("CTRL+O"));
    pactOpen->setToolTip(tr("Open Document"));
    pactOpen->setStatusTip(tr("Open an existing file"));
    pactOpen->setWhatsThis(tr("Open an existing file"));
    pactOpen->setIcon(QPixmap(":/images/icons/fileopen.png"));
    connect(pactOpen, SIGNAL(triggered()), SLOT(SlotLoad()));

    // Создание действия "Сохранить файл"
    QAction* pactSave = new QAction(tr("Save File"), 0);
    pactSave->setText(tr("&Save"));
//    pactSave->setShortcut(QKeySequence("CTRL+S"));
    pactSave->setToolTip(tr("Save Document"));
    pactSave->setStatusTip(tr("Save the file to disk"));
    pactSave->setWhatsThis(tr("Save the file to disk"));
    pactSave->setIcon(QPixmap(":/images/icons/filesave.png"));
    connect(pactSave, SIGNAL(triggered()), SLOT(SlotSave()));

    // Создание действия "Сохранить файл как"
    QAction* pactSaveAs = new QAction(tr("Save File As..."), 0);
    pactSaveAs->setText(tr("Save &As..."));
    pactSaveAs->setToolTip(tr("Save Document As..."));
    pactSaveAs->setStatusTip(tr("Save the file to disk as..."));
    pactSaveAs->setWhatsThis(tr("Save the file to disk as..."));
    pactSaveAs->setIcon(QPixmap(":/images/icons/filesaveas.png"));
    connect(pactSaveAs, SIGNAL(triggered()), SLOT(SlotSaveAs()));

    // Создаём пункт меню "Файл" главного окна
    QMenu* pmnuFile = new QMenu(tr("&File"));
    pmnuFile->addAction(pactNew);
    pmnuFile->addAction(pactOpen);
    pmnuFile->addAction(pactSave);
    pmnuFile->addAction(pactSaveAs);
    pmnuFile->addSeparator();
    pmnuFile->addAction(tr("&Quit"),
                        QKeySequence("CTRL+Q"),
                        qApp,
                        SLOT(closeAllWindows()));
    menuBar()->addMenu(pmnuFile);

    // Создаём пункт меню "Окна" главного окна
    _pMenuWindows = new QMenu("&Windows");
    menuBar()->addMenu(_pMenuWindows);
    connect(_pMenuWindows, SIGNAL(aboutToShow()), SLOT(SlotWindows()));
    menuBar()->addSeparator();

    // Создаём пункт меню "Помощь" главного окна
    QMenu* pMenuHelp = new QMenu(tr("&Help"));
    pMenuHelp->addAction(tr("&About"), Qt::Key_F1, this, SLOT(SlotAbout()));
    menuBar()->addMenu(pMenuHelp);

    // Cоздание виджета MDI
    _pMidiArea = new QMdiArea;
    _pMidiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _pMidiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(_pMidiArea);

    //Создаём мапер сигналов дочених окон
    _pSignalMapper = new QSignalMapper(this);
    connect(_pSignalMapper,
            SIGNAL(mappedObject(QObject*)),
            this,
            SLOT(SlotSetActiveSubWindow(QObject*)));
        
    // us2_t-001 Спринт 1 Алексей:   Реализовать тулбар
    QToolBar *bar = new QToolBar;
    bar->addAction(pactNew);
    bar->addAction(pactOpen);
    bar->addAction(pactSave);
    bar->addAction(pactSaveAs);
    addToolBar(bar);

    SetupTextActions();
    _pCurrentDocument = nullptr;
    connect (_pMidiArea, &QMdiArea::subWindowActivated, this, &MainWindow::SetupActiveDocument);
        
    // Создаём статусбар
    statusBar()->showMessage("Ready", 3000);
}

// Метод создаёт экземпляр дочернего MDI окна документа
DocumentWindow* MainWindow::CreateNewDocument()
{
    DocumentWindow* pDocument = new DocumentWindow;
    _pMidiArea->addSubWindow(pDocument);
    pDocument->setAttribute(Qt::WA_DeleteOnClose);
    pDocument->setWindowTitle(tr("Unnamed Document"));
    pDocument->setWindowIcon(QPixmap(":/images/icons/filenew.png"));
    connect(pDocument,
            SIGNAL(SignalChangeTitle(const QString&)),
            SLOT(SlotChangeWindowTitle(const QString&)));

    return pDocument;
}

// Слот для задания заголовка окна
void MainWindow::SlotChangeWindowTitle(const QString& name)
{
    qobject_cast<DocumentWindow*>(sender())->setWindowTitle(name);
}

// Слот создания нового документа
void MainWindow::SlotNewDoc()
{
    CreateNewDocument()->show();
}

// Слот загрузки документа
void MainWindow::SlotLoad()
{
    DocumentWindow* pDocument = CreateNewDocument();
    pDocument->SlotLoad();
    pDocument->show();
}

// Слот сохранения документа
void MainWindow::SlotSave()
{
    DocumentWindow* pDocument = qobject_cast<DocumentWindow*>(_pMidiArea->activeSubWindow());
    if (pDocument)
        pDocument->SlotSave();
}

// Слот сохранить документ как
void MainWindow::SlotSaveAs()
{
    DocumentWindow* pDocument = qobject_cast<DocumentWindow*>(_pMidiArea->activeSubWindow());
    if (pDocument)
        pDocument->SlotSaveAs();
}

// Слот вызова окна "О программе"
void MainWindow::SlotAbout()
{
    QMessageBox::about(this, tr("TextEditor"), tr("Command 2 Text Editor"));
}

// Слот меню "Окна"
void MainWindow::SlotWindows()
{
    _pMenuWindows->clear();

    QAction* pAction = _pMenuWindows->addAction(tr("&Cascade"),
                                                _pMidiArea,
                                                SLOT(cascadeSubWindows()));
    pAction->setEnabled(!_pMidiArea->subWindowList().isEmpty());

    pAction = _pMenuWindows->addAction(tr("&Tile"),
                                    _pMidiArea,
                                    SLOT(tileSubWindows()));
    pAction->setEnabled(!_pMidiArea->subWindowList().isEmpty());

    _pMenuWindows->addSeparator();

    QList<QMdiSubWindow*> listDocuments = _pMidiArea->subWindowList();
    for(auto& document : listDocuments)
    {
        pAction = _pMenuWindows->addAction(document->windowTitle());
        pAction->setCheckable(true);
        pAction->setChecked(_pMidiArea->activeSubWindow() == document);
        connect(pAction, SIGNAL(triggered()), _pSignalMapper, SLOT(map()));
        _pSignalMapper->setMapping(pAction, document);
    }
}

// Слот делает дочернего MDI окно активным
void MainWindow::SlotSetActiveSubWindow(QObject* pMdiSubWindow)
{
    if (pMdiSubWindow)
        _pMidiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(pMdiSubWindow));
}

// Формирование экшена для жирного шрифта
void MainWindow::SetupBoldActions(QToolBar* toolBar, QMenu* menu)
{
    actionTextBold = menu -> addAction(tr("&Bold"));
    actionTextBold -> setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold -> setPriority(QAction::LowPriority);
    actionTextBold -> setIcon(QPixmap(":/images/icons/text_bold.png"));
    QFont bold;
    bold.setBold(true);
    actionTextBold -> setFont(bold);
    toolBar -> addAction(actionTextBold);
    actionTextBold -> setCheckable(true);
}

// Формирование экшена для курсивного шрифта
void MainWindow::SetupItalicActions(QToolBar* toolBar, QMenu* menu)
{
    actionTextItalic = menu -> addAction (tr("&Italic"));
    actionTextItalic -> setPriority (QAction::LowPriority);
    actionTextItalic -> setShortcut (Qt::CTRL + Qt::Key_I);
    actionTextItalic -> setIcon (QPixmap(":/images/icons/text_italic.png"));
    QFont italic;
    italic.setItalic (true);
    actionTextItalic -> setFont (italic);
    toolBar -> addAction (actionTextItalic);
    actionTextItalic -> setCheckable (true);
}

// Формирование экшена для подчеркнутого шрифта
void MainWindow::SetupUnderLineActions(QToolBar* toolBar, QMenu* menu)
{
    actionTextUnderline = menu -> addAction(tr("&Underline"));
    actionTextUnderline -> setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline -> setPriority(QAction::LowPriority);
    actionTextUnderline -> setIcon(QPixmap(":/images/icons/text_under.png"));
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    toolBar->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);
}

// инициализация тулбара для шрифта
void MainWindow::SetupTextActions()
{
    QToolBar *toolBar = addToolBar(tr("Format Actions"));
    QMenu *menu = menuBar()->addMenu(tr("F&ormat"));
    SetupBoldActions (toolBar, menu);
    SetupItalicActions (toolBar, menu);
    SetupUnderLineActions (toolBar, menu);
    toolBar = addToolBar(tr("Format Actions"));
    toolBar -> setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(toolBar);
    comboFont = new QFontComboBox(toolBar);
    toolBar->addWidget(comboFont);
}

// Отображение настроек измененного шрифта
void MainWindow::FontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

// Получение указателя на активное окно редактирования, удаляет старые соединения, устанавливает новые
void MainWindow :: SetupActiveDocument (QMdiSubWindow* window)
{
    DisonnectFromDocument ();
    _pCurrentDocument = dynamic_cast <DocumentWindow*> (window->widget());
    CurrentCharFormatChanged(_pCurrentDocument->currentCharFormat());
    ConnectToActiveDocument ();
}

// устанавливает новые соединения с окном редактирования
void MainWindow :: ConnectToActiveDocument ()
{
    connect (comboFont, &QComboBox::textActivated, _pCurrentDocument, &DocumentWindow::TextFamily);
    connect (actionTextBold     , &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextBold);
    connect (actionTextItalic   , &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextItalic);
    connect (actionTextUnderline, &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextUnderline);
    connect (_pCurrentDocument, &QTextEdit::currentCharFormatChanged, this, &MainWindow::CurrentCharFormatChanged);

}

// удаляет текущие соединения с  окном редактирования
void MainWindow :: DisonnectFromDocument ()
{
    if (_pCurrentDocument!= nullptr)
    {
        disconnect(comboFont, &QComboBox::textActivated, _pCurrentDocument, &DocumentWindow::TextFamily);
        disconnect (actionTextBold     , &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextBold);
        disconnect (actionTextItalic   , &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextItalic);
        disconnect (actionTextUnderline, &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextUnderline);
        disconnect (_pCurrentDocument, &QTextEdit::currentCharFormatChanged, this, &MainWindow::CurrentCharFormatChanged);
    }
}

//устанавливает настрйки, соответствующие формату шрифта
void MainWindow::CurrentCharFormatChanged(const QTextCharFormat &format)
{
    FontChanged(format.font());
    //colorChanged(format.foreground().color());
}
