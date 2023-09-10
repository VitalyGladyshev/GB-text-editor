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
    QAction* pactNew = new QAction(tr("New File"), this);
    pactNew->setText(tr("&New"));
//    pactNew->setShortcut(QKeySequence("CTRL+N"));
    pactNew->setToolTip(tr("New Document"));
    pactNew->setStatusTip(tr("Create a new file"));
    pactNew->setWhatsThis(tr("Create a new file"));
    pactNew->setIcon(QPixmap(":/images/icons/filenew.png"));
    connect(pactNew, SIGNAL(triggered()), SLOT(SlotNewDoc()));

    // Создание действия "Открыть файл"
    QAction* pactOpen = new QAction(tr("Open File"), this);
    pactOpen->setText(tr("&Open..."));
//    pactOpen->setShortcut(QKeySequence("CTRL+O"));
    pactOpen->setToolTip(tr("Open Document"));
    pactOpen->setStatusTip(tr("Open an existing file"));
    pactOpen->setWhatsThis(tr("Open an existing file"));
    pactOpen->setIcon(QPixmap(":/images/icons/fileopen.png"));
    connect(pactOpen, SIGNAL(triggered()), SLOT(SlotLoad()));

    // Создание действия "Сохранить файл"
    _pSaveAct = new QAction(tr("Save File"), this);
    _pSaveAct->setText(tr("&Save"));
//    _pSaveAct->setShortcut(QKeySequence("CTRL+S"));
    _pSaveAct->setToolTip(tr("Save Document"));
    _pSaveAct->setStatusTip(tr("Save the file to disk"));
    _pSaveAct->setWhatsThis(tr("Save the file to disk"));
    _pSaveAct->setIcon(QPixmap(":/images/icons/filesave.png"));
    connect(_pSaveAct, SIGNAL(triggered()), SLOT(SlotSave()));

    // Создание действия "Сохранить файл как"
    _pSaveAsAct = new QAction(tr("Save File As..."), this);
    _pSaveAsAct->setText(tr("Save &As..."));
    _pSaveAsAct->setToolTip(tr("Save Document As..."));
    _pSaveAsAct->setStatusTip(tr("Save the file to disk as..."));
    _pSaveAsAct->setWhatsThis(tr("Save the file to disk as..."));
    _pSaveAsAct->setIcon(QPixmap(":/images/icons/filesaveas.png"));
    connect(_pSaveAsAct, SIGNAL(triggered()), SLOT(SlotSaveAs()));

    // Создание действия "Вырезать"
    _pCutAct = new QAction(tr("Cut"), this);
    _pCutAct->setText(tr("Cu&t"));
//    _pCutAct->setShortcuts(QKeySequence::Cut);
    _pCutAct->setToolTip(tr("Cut text"));
    _pCutAct->setStatusTip(
        tr("Cut the current selection's contents to the clipboard"));
    _pCutAct->setWhatsThis(
        tr("Cut the current selection's contents to the clipboard"));
    _pCutAct->setIcon(QPixmap(":/images/icons/editcut.png"));
    connect(_pCutAct, SIGNAL(triggered()), SLOT(SlotCut()));

    // Создание действия "Копировать"
    _pCopyAct = new QAction(tr("Copy"), this);
    _pCopyAct->setText(tr("&Copy"));
//    _pCopyAct->setShortcuts(QKeySequence::Copy);
    _pCopyAct->setToolTip(tr("Copy text"));
    _pCopyAct->setStatusTip(
        tr("Copy the current selection's contents to the clipboard"));
    _pCopyAct->setWhatsThis(
        tr("Copy the current selection's contents to the clipboard"));
    _pCopyAct->setIcon(QPixmap(":/images/icons/editcopy.png"));
    connect(_pCopyAct, SIGNAL(triggered()), SLOT(SlotCopy()));

    // Создание действия "Вставить"
    _pPasteAct = new QAction(tr("Paste"), this);
    _pPasteAct->setText(tr("&Paste"));
//    _pPasteAct->setShortcuts(QKeySequence::Paste);
    _pPasteAct->setToolTip(tr("Paste text"));
    _pPasteAct->setStatusTip(
        tr("Paste the clipboard's contents into the current selection"));
    _pPasteAct->setWhatsThis(
        tr("Paste the clipboard's contents into the current selection"));
    _pPasteAct->setIcon(QPixmap(":/images/icons/editpaste.png"));
    connect(_pPasteAct, SIGNAL(triggered()), SLOT(SlotPaste()));

    // Создаём пункт меню "Файл" главного окна
    QMenu* pmnuFile = new QMenu(tr("&File"));
    pmnuFile->addAction(pactNew);
    pmnuFile->addAction(pactOpen);
    pmnuFile->addAction(_pSaveAct);
    pmnuFile->addAction(_pSaveAsAct);
    pmnuFile->addSeparator();
    pmnuFile->addAction(tr("&Quit"),
                        QKeySequence("CTRL+Q"),
                        qApp,
                        SLOT(closeAllWindows()));
    menuBar()->addMenu(pmnuFile);

    // Создаём пункт меню "Редактировать" главного окна
    QMenu* pMenuEdit = new QMenu(tr("&Edit"));
    pMenuEdit->addAction(_pCutAct);
    pMenuEdit->addAction(_pCopyAct);
    pMenuEdit->addAction(_pPasteAct);
    menuBar()->addMenu(pMenuEdit);

    // Создаём пункт меню "Вкладки" главного окна
    _pMenuWindows = new QMenu(tr("&Tabs"));
    menuBar()->addMenu(_pMenuWindows);
    connect(_pMenuWindows, SIGNAL(aboutToShow()), SLOT(SlotWindows()));
    menuBar()->addSeparator();

    // Создаём пункт меню "Помощь" главного окна
    QAction* pAboutAct = new QAction(tr("About"), 0);
    pAboutAct->setText(tr("&About"));
//    pAboutAct->setShortcut(Qt::Key_F1);
    pAboutAct->setToolTip(tr("Save Document"));
    pAboutAct->setStatusTip(tr("Show the application's About box"));
    pAboutAct->setWhatsThis(tr("Show the application's About box"));
    connect(pAboutAct, SIGNAL(triggered()), SLOT(SlotAbout()));
    QMenu* pMenuHelp = new QMenu(tr("&Help"));
    pMenuHelp->addAction(pAboutAct);
    menuBar()->addMenu(pMenuHelp);

    // Cоздание виджета MDI
    _pMdiArea = new QMdiArea;
    _pMdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _pMdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(_pMdiArea);
    connect(_pMdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(SlotUpdateMenus()));

    //Создаём мапер сигналов дочених окон
    _pSignalMapper = new QSignalMapper(this);
    connect(_pSignalMapper,
            SIGNAL(mappedObject(QObject*)),
            this,
            SLOT(SlotSetActiveSubWindow(QObject*)));
        
    // us2_t-001 Спринт 1 Алексей:   Реализовать тулбар
    _pToolBar = new QToolBar(this);
    _pToolBar->addAction(pactNew);
    _pToolBar->addAction(pactOpen);
    _pToolBar->addAction(_pSaveAct);
    _pToolBar->addAction(_pSaveAsAct);
    addToolBar(_pToolBar);

    QToolBar* pEditToolBar = new QToolBar(this);
    pEditToolBar->addAction(_pCutAct);
    pEditToolBar->addAction(_pCopyAct);
    pEditToolBar->addAction(_pPasteAct);
    addToolBar(pEditToolBar);

    //us6_t-001 Спринт 2 Алексей: Реализовать доквиджет для быстрого доступа к файлам на текущем диске
    _pFileManager = new FileManager(this);
    _pDocWidget = new QDockWidget("FileManager", this);
    _pDocWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _pDocWidget->setWidget(_pFileManager);
    addDockWidget(Qt::LeftDockWidgetArea,_pDocWidget);
    
    SetupTextActions();
    _pCurrentDocument = nullptr;
    connect (_pMidiArea, &QMdiArea::subWindowActivated, this, &MainWindow::SetupActiveDocument);
        
    
    // Создаём статусбар
    statusBar()->showMessage("Ready", 3000);

    SlotUpdateMenus();
}

// Метод открытия файла в дочернем окне
bool MainWindow::OpenFile(const QString &pathFileName)
{
    DocumentWindow* pDocument = CreateNewDocument();
    if (pDocument->OpenFile(pathFileName))
    {
        pDocument->show();
        return true;
    }
    else
    {
        pDocument->close();
        return false;
    }
}

// Перегруженный метод закрытия виджета
void MainWindow::closeEvent(QCloseEvent *event)
{
    _pMdiArea->closeAllSubWindows();
    if (_pMdiArea->currentSubWindow())
        event->ignore();
    else
        event->accept();
}

// Возвращает указатель на текущий активный документ
DocumentWindow* MainWindow::GetActiveDocumentWindow() const
{
    auto mdiChild = _pMdiArea->activeSubWindow();
    if (mdiChild)
        return qobject_cast<DocumentWindow*>(mdiChild->widget());
    else
        return nullptr;
}

// Метод создаёт экземпляр дочернего MDI окна документа
DocumentWindow* MainWindow::CreateNewDocument()
{
    DocumentWindow* pDocument = new DocumentWindow;
    _pMdiArea->addSubWindow(pDocument);
    pDocument->setAttribute(Qt::WA_DeleteOnClose);
    pDocument->setWindowTitle(tr("Unnamed Document"));
    pDocument->setWindowIcon(QPixmap(":/images/icons/filenew.png"));
    connect(pDocument, SIGNAL(SignalStatusBarMessage(const QString&)),
            this, SLOT(SlotStatusBarMessage(const QString&)));
    connect(pDocument, &QTextEdit::copyAvailable, _pCutAct, &QAction::setEnabled);
    connect(pDocument, &QTextEdit::copyAvailable, _pCopyAct, &QAction::setEnabled);
    return pDocument;
}

// Слот - вывод сообщения в статусбаре главного окна
void MainWindow::SlotStatusBarMessage(const QString& Message)
{
    statusBar()->showMessage(Message);
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
    if (pDocument->Load())
        pDocument->show();
    else
        pDocument->close();
}

// Слот сохранения документа
void MainWindow::SlotSave()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->Save();
}

// Слот сохранить документ как
void MainWindow::SlotSaveAs()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->SaveAs();
}

// Слот вызова окна "О программе"
void MainWindow::SlotAbout()
{
    QMessageBox::about(this, tr("TextEditor"), tr("<b>Command 2 Text Editor</b>"));
}

// Слот меню "Окна"
void MainWindow::SlotWindows()
{
    _pMenuWindows->clear();

//    QAction* pAction = _pMenuWindows->addAction(tr("&Cascade"),
//                                                _pMdiArea,
//                                                SLOT(cascadeSubWindows()));
//    pAction->setEnabled(!_pMdiArea->subWindowList().isEmpty());

//    pAction = _pMenuWindows->addAction(tr("&Tile"),
//                                       _pMdiArea,
//                                    SLOT(tileSubWindows()));
//    pAction->setEnabled(!_pMdiArea->subWindowList().isEmpty());

//    _pMenuWindows->addSeparator();

    QList<QMdiSubWindow*> listDocuments = _pMdiArea->subWindowList();
    for(auto& document: listDocuments)
    {
        QAction* pAction = _pMenuWindows->addAction(document->windowTitle());
        pAction->setCheckable(true);
        pAction->setChecked(_pMdiArea->activeSubWindow() == document);
        connect(pAction, SIGNAL(triggered()), _pSignalMapper, SLOT(map()));
        _pSignalMapper->setMapping(pAction, document);
    }
}

// Слот вырезать текст
void MainWindow::SlotCut()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->cut();
}

// Слот копировать текст
void MainWindow::SlotCopy()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->copy();
}

// Слот вставить текст
void MainWindow::SlotPaste()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->paste();
}

// Слот сделать активными/не активными эементы интерфеса, если документ открыт
void MainWindow::SlotUpdateMenus()
{
    bool hasDocumentWindow = GetActiveDocumentWindow();
    _pSaveAct->setEnabled(hasDocumentWindow);
    _pSaveAsAct->setEnabled(hasDocumentWindow);
    _pPasteAct->setEnabled(hasDocumentWindow);

    bool textSelection = (GetActiveDocumentWindow() &&
                         GetActiveDocumentWindow()->textCursor().hasSelection());
    _pCutAct->setEnabled(textSelection);
    _pCopyAct->setEnabled(textSelection);
}

// Слот делает дочернего MDI окно активным
void MainWindow::SlotSetActiveSubWindow(QObject* pMdiSubWindow)
{
    if (pMdiSubWindow)
        _pMdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(pMdiSubWindow));
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
