/************************************************
* Команда 2
* Гипертекстовый редактор
*
* Код класса главного окна текстового редактора
************************************************/

#include <QtWidgets>
#include <QMdiArea>
#include <QAction>
#include <QPrintDialog>
#include <QPixmap>
#include <QColor>
#include <QPagedPaintDevice>
#include <QPrintPreviewDialog>

#include "mainwindow.h"
#include "documentwindow.h"
#include "settings.h"
#include "finddialog.h"
#include "hyperlinkdialog.h"
#include "helpviewdialog.h"

MainWindow::MainWindow(QWidget *parent /* = nullptr */)
    : QMainWindow(parent), _iUnnamedIndex(0)
{
    // Загружаем настройки
    if(Settings::GetInstance().GetLanguage() == Language::Russian)
    {
        if(_translator.load(":/translations/TextEditor_ru_RU.qm", ":/translations/"))
            qApp->installTranslator(&_translator);
    }
    else
    {
        if(_translator.load(":/translations/TextEditor_en_US.qm", ":/translations/"))
            qApp->installTranslator(&_translator);
    }
    setWindowTitle(tr("Hypertext editor"));

    if(Settings::GetInstance().GetTheme() == Theme::Light)
        qDebug() << "Тема: светлая";
    else
        qDebug() << "Тема: тёмная";

    // Создаём объекты действий
    CreateActions();

    // Создаём пункт меню "Файл" главного окна
    QMenu* pmnuFile = new QMenu(tr("&File"));
    pmnuFile->addAction(_pNewAct);
    pmnuFile->addAction(_pOpenAct);
    pmnuFile->addAction(_pSaveAct);
    pmnuFile->addAction(_pSaveAsAct);
    pmnuFile->addSeparator();
    pmnuFile->addAction(_pPrintAct);
    pmnuFile->addAction(_pPrintPreviewAct);
    pmnuFile->addAction(_pSaveAsOdt);
    pmnuFile->addAction(_pPrintPDFAct);
    pmnuFile->addSeparator();
    pmnuFile->addAction(tr("&Quit"),
                        QKeySequence("CTRL+Q"),
                        qApp,
                        SLOT(closeAllWindows()));
    menuBar()->addMenu(pmnuFile);

    // Создаём пункт меню "Правка" главного окна
    QMenu* pMenuEdit = new QMenu(tr("&Edit"));
    pMenuEdit->addAction(_pCutAct);
    pMenuEdit->addAction(_pCopyAct);
    pMenuEdit->addAction(_pPasteAct);
    pMenuEdit->addSeparator();
    pMenuEdit->addAction(_pUndoAct);
    pMenuEdit->addAction(_pRedoAct);
    pMenuEdit->addSeparator();
    pMenuEdit->addAction(_pMakeLinkAct);
    pMenuEdit->addAction(_pAddImageAct);
    pMenuEdit->addSeparator();
    pMenuEdit->addAction(_pFindAct);
    menuBar()->addMenu(pMenuEdit);

    // Создаём пункт меню "Форматирование" главного окна
    QMenu* pMenuFormat = new QMenu(tr("&Format"));
    auto toolBarFormat = SetupFormatActions(pMenuFormat);
    menuBar()->addMenu(pMenuFormat);

    // Создаём пункт меню "Вкладки" главного окна
    _pMenuWindows = new QMenu(tr("&Tabs"));
    menuBar()->addMenu(_pMenuWindows);
    connect(_pMenuWindows, SIGNAL(aboutToShow()), SLOT(SlotWindows()));

    // Создаём пункт меню "Настройки" главного окна
    QMenu* pMenuSettings = new QMenu(tr("Settings"));
    SetupLanguageActions(pMenuSettings);
    pMenuSettings->addSeparator();
    SetupThemeActions(pMenuSettings);
    menuBar()->addMenu(pMenuSettings);

    // Создаём пункт меню "Помощь" главного окна
    QMenu* pMenuHelp = new QMenu(tr("&Help"));
    pMenuHelp->addAction(_pHelpAct);
    pMenuHelp->addAction(_pAboutAct);
    menuBar()->addMenu(pMenuHelp);

    // Cоздание виджета MDI
    _pMdiArea = new QMdiArea(this);
    _pMdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _pMdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
// перенесено!  _pMdiArea->setViewMode(QMdiArea::TabbedView);
    _pMdiArea->setTabsClosable(true);
    setCentralWidget(_pMdiArea);
    connect(_pMdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(SlotUpdateMenus()));
    connect(_pNextAct, &QAction::triggered, _pMdiArea, &QMdiArea::activateNextSubWindow);
    connect(_pPreviousAct, &QAction::triggered, _pMdiArea, &QMdiArea::activatePreviousSubWindow);

    // Создание действия закрыть дочернее окно
    _pCloseAct = new QAction(tr("Cl&ose"), this);
    _pCloseAct->setStatusTip(tr("Close the active window"));
    connect(_pCloseAct, &QAction::triggered,
            _pMdiArea, &QMdiArea::closeActiveSubWindow);

    // Создание действия закрыть всё дочерние окна
    _pCloseAllAct = new QAction(tr("Close &All"), this);
    _pCloseAllAct->setStatusTip(tr("Close all the windows"));
    connect(_pCloseAllAct, &QAction::triggered, _pMdiArea, &QMdiArea::closeAllSubWindows);

    //Создаём мапер сигналов дочених окон
    _pSignalMapper = new QSignalMapper(this);
    connect(_pSignalMapper,
            SIGNAL(mappedObject(QObject*)),
            this,
            SLOT(SlotSetActiveSubWindow(QObject*)));
        
    // us2_t-001 Спринт 1 Алексей:   Реализовать тулбар
    _pToolBar = new QToolBar(this);
    _pToolBar->addAction(_pNewAct);
    _pToolBar->addAction(_pOpenAct);
    _pToolBar->addAction(_pSaveAct);
    _pToolBar->addAction(_pSaveAsAct);
    _pToolBar->addSeparator();
    _pToolBar->addAction(_pPrintAct);
    _pToolBar->addAction(_pPrintPreviewAct);
    _pToolBar->addAction(_pSaveAsOdt);
    _pToolBar->addAction(_pPrintPDFAct);
    _pToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBar(_pToolBar);

    QToolBar* pToolBarNavigation = new QToolBar(this);
    pToolBarNavigation->addAction(_pBackwardAct);
    pToolBarNavigation->addAction(_pHomeAct);
    pToolBarNavigation->addAction(_pForwardAct);
    pToolBarNavigation->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBar(pToolBarNavigation);

    QToolBar* pEditToolBar = new QToolBar(this);
    pEditToolBar->addAction(_pCutAct);
    pEditToolBar->addAction(_pCopyAct);
    pEditToolBar->addAction(_pPasteAct);
    pEditToolBar->addSeparator();
    pEditToolBar->addAction(_pUndoAct);
    pEditToolBar->addAction(_pRedoAct);
    pEditToolBar->addSeparator();
    pEditToolBar->addAction(_pFindAct);
    pEditToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBar(pEditToolBar);

    QToolBar* pHelpToolBar = new QToolBar(this);
    pHelpToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    pHelpToolBar->addAction(_pHelpAct);
    addToolBar(pHelpToolBar);
    addToolBarBreak();

    addToolBar(toolBarFormat);
    addToolBarBreak();

    auto toolbarFont = SetupFontActions();
    addToolBar(toolbarFont);

    //us6_t-001 Спринт 2 Алексей: Реализовать доквиджет для быстрого доступа к файлам на текущем диске
    _pFileManager = new FileManager(this);
    connect(_pFileManager, SIGNAL(SignalSetActive(QString)),
            this, SLOT(SlotSetActiveSubWindowByPath(QString)));
    _pDocWidget = new QDockWidget(tr("File manager"), this);
    _pDocWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _pDocWidget->setWidget(_pFileManager);
    addDockWidget(Qt::LeftDockWidgetArea,_pDocWidget);

    _pListPath = new QList<QString>;

    _pCurrentDocument = nullptr;
    connect(_pMdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::SetupActiveDocument);
    
    // Создаём статусбар
    statusBar()->showMessage("Ready", 3000);

    setWindowIcon(QPixmap(":/images/icons/khexedit.png"));

    // Создаём диалог поиска
    _pFindDialog  = new FindDialog(this);
    _pFindDialog->setWindowTitle(tr("Find text"));
//    _pFindDialog->SetButtonLabel(tr("Find"));
    _pFindDialog->SetWTCheckBoxLabel(tr("Find whole text"));

    // Создаём диалог добавления гиперссылки
    _pMakeLinkDialog  = new HyperlinkDialog(this);
    _pMakeLinkDialog->setWindowTitle(tr("Make hyperlink"));
//    _pMakeLinkDialog->SetButtonLinkLabel(tr("Make hyperlink"));
//    _pMakeLinkDialog->SetLabelText(tr("Link text"));
//    _pMakeLinkDialog->SetLabelTarget(tr("Link target"));

    // Создаём диалог показа помощи
    _pShowHelpDialog  = new HelpViewDialog("startpage.html",
                                          {":/documentation/"}, this);
    _pShowHelpDialog->setWindowTitle(tr("Help view"));

    // Открываем стартовый файл
    QString startFileName = QDir(QDir::currentPath()).filePath(":/documentation/startpage.html");
    QFile file(startFileName);
    if(file.exists())
        OpenFile(startFileName);
    else
        SlotNewDoc();

    SlotUpdateMenus();
    _pMdiArea->setViewMode(QMdiArea::TabbedView);
}

// Метод открытия файла в дочернем окне
bool MainWindow::OpenFile(const QString &pathFileName)
{
    if (!_pListPath->contains(pathFileName))
    {
        DocumentWindow* pDocument = CreateNewDocument();
        if (pDocument->OpenFile(pathFileName))
        {

            pDocument->show();      // showMaximized()
            return true;
        }
        else
        {
            pDocument->close();
            return false;
        }
    }
    return false;
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
DocumentWindow* MainWindow::GetActiveDocumentWindow()
{
    auto mdiChild = _pMdiArea->activeSubWindow();
    if (mdiChild)
        return qobject_cast<DocumentWindow*>(mdiChild->widget());
    else
    {
        _pCurrentDocument = nullptr;
        return nullptr;
    }
}

// Метод создаёт экземпляр дочернего MDI окна документа
DocumentWindow* MainWindow::CreateNewDocument()
{
    DocumentWindow* pDocument = new DocumentWindow(this);
    _pMdiArea->addSubWindow(pDocument);
    pDocument->setAttribute(Qt::WA_DeleteOnClose);
    pDocument->setWindowTitle(tr("Unnamed") + "_" + QString::number(_iUnnamedIndex++));
    pDocument->setWindowIcon(QPixmap(":/images/icons/filenew.png"));

    connect(pDocument, SIGNAL(SignalStatusBarMessage(QString)),
            this, SLOT(SlotStatusBarMessage(QString)));
    connect(pDocument, &DocumentWindow::IsOpen,this,&MainWindow::SlotOnOpen);
    connect(pDocument, &DocumentWindow::IsClose,this,&MainWindow::SlotDelPath);

    return pDocument;
}

// Слот - вывод сообщения в статусбаре главного окна
void MainWindow::SlotStatusBarMessage(QString Message)
{
    statusBar()->showMessage(Message);
}

// Слот создания нового документа
void MainWindow::SlotNewDoc()
{
    CreateNewDocument()->show();    // showMaximized()
}

// Слот загрузки документа
void MainWindow::SlotLoad()
{
    QString path = DocumentWindow::Load();
    if (!_pListPath->contains(path))
    {
// перенесено!        _pListPath->append(path);
        DocumentWindow* pDocument = CreateNewDocument();
        pDocument->OpenFile(path);
    }
}

// Слот сохранения документа
void MainWindow::SlotSave()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->Save();
}

// Слот активации действия сохранения документа
void MainWindow::SlotSaveEnable()
{
    _pSaveAct->setEnabled(true);
}

// Слот сохранить документ как
void MainWindow::SlotSaveAs()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
    {
        auto index = _pListPath->indexOf(pDocument->GetPathFileName());
        if (index > 0)
            _pListPath->remove(index);
        pDocument->SaveAs();
        _pListPath->append(pDocument->GetPathFileName());
    }
}

// Слот вызова окна "О программе"
void MainWindow::SlotAbout()
{
    QMessageBox::about(this, tr("Hypertext editor"), tr("<b>Team 2 Hypertext editor</b>"));
}

// Слот меню "Окна"
void MainWindow::SlotWindows()
{
    _pMenuWindows->clear();

    _pMenuWindows->addAction(_pCloseAct);
    _pMenuWindows->addAction(_pCloseAllAct);
    _pMenuWindows->addSeparator();
    _pMenuWindows->addAction(_pNextAct);
    _pMenuWindows->addAction(_pPreviousAct);
    _pMenuWindows->addSeparator();

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

// Слот навигация назад
void MainWindow::SlotBackward()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->backward();
}

// Слот навигация Домой
void MainWindow::SlotHome()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->home();
}

// Слот навигация вперёд
void MainWindow::SlotForward()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->forward();
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

// Слот печати документа
void MainWindow::SlotPrint()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();

    if (!pDocument) return;
    QPrinter *printer = new QPrinter;
    QPrintDialog dlg = QPrintDialog(printer);
    dlg.setWindowTitle(tr("Print document"));

    if(dlg.exec() != QDialog::Accepted) return;

    pDocument->print(printer);
}

// Слот предварительного просмотра перед печатью документа
void MainWindow::SlotPrintPreview()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
    {
        QPrinter* printer = new QPrinter;
        QPrintPreviewDialog prPreviewDlg(printer, this);
        connect(&prPreviewDlg, SIGNAL(paintRequested(QPrinter*)), SLOT(SlotPrintPreviewDraw(QPrinter*)));

        if(prPreviewDlg.exec())
            pDocument->print(printer);
    }
}

// Слот отображения для предварительного просмотра
void MainWindow::SlotPrintPreviewDraw(QPrinter* printer)
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->print(printer);
}

// Слот сохранить документ в ODT
void MainWindow::SlotSaveAsOdt()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
    {
        QFileDialog fileDialog(this, tr("Save as ODT"), QDir::currentPath());
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        QStringList mimeTypes{"application/vnd.oasis.opendocument.text"};
        fileDialog.setMimeTypeFilters(mimeTypes);
        fileDialog.setDefaultSuffix("odt");
        if (fileDialog.exec() != QDialog::Accepted)
            return;
        const QString pathFileName = fileDialog.selectedFiles().constFirst();

        if (pathFileName.isEmpty())
            return;

        pDocument->SaveAsOdt(pathFileName);
    }
}

// Слот сохранить документ в PDF
void MainWindow::SlotPrintPDF()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (!pDocument)
        return;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save document to pdf"), "", tr("PDF files (*.pdf)"));

    QPrinter *printer = new QPrinter;
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOutputFileName(fileName);
    pDocument->print(printer);
}

// Слот поиск в тексте
void MainWindow::SlotFind()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
    {
        _pFindDialog->ClearRequest();
        _pFindDialog->show();
    }
}

// Слот добавление гиперссылки
void MainWindow::SlotMakeHyperlink()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
    {
        _pMakeLinkDialog->ClearTarget();
        auto selectedText = pDocument->GetSelectedText();
        if (selectedText.isEmpty())
            _pMakeLinkDialog->ClearText();
        else
            _pMakeLinkDialog->SetLineEditText(selectedText);
        _pMakeLinkDialog->show();
    }
}

// Слот добавить изображение
void MainWindow::SlotAddImage()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    if (pDocument)
        pDocument->AddImage();
}

// Слот показать справку
void MainWindow::SlotHelp()
{
    _pShowHelpDialog->show();
}

// Слот сделать активными/не активными эементы интерфеса, если документ открыт
void MainWindow::SlotUpdateMenus()
{
    DocumentWindow* pDocument = GetActiveDocumentWindow();
    _pSaveAsAct->setEnabled(pDocument);
    _pPasteAct->setEnabled(pDocument);
    _pPrintAct->setEnabled(pDocument);
    _pPrintPreviewAct->setEnabled(pDocument);
    _pSaveAsOdt->setEnabled(pDocument);
    _pPrintPDFAct->setEnabled(pDocument);
    _pFindAct->setEnabled(pDocument);
    _pMakeLinkAct->setEnabled(pDocument);
    _pAddImageAct->setEnabled(pDocument);
    actionTextBold->setEnabled(pDocument);
    actionTextUnderline->setEnabled(pDocument);
    actionTextItalic->setEnabled(pDocument);
    _pCloseAct->setEnabled(pDocument);
    _pCloseAllAct->setEnabled(pDocument);
    _pNextAct->setEnabled(pDocument);
    _pPreviousAct->setEnabled(pDocument);

    _pSaveAct->setEnabled(false);
    _pUndoAct->setEnabled(false);
    _pRedoAct->setEnabled(false);
    if (pDocument)
    {
        _pSaveAct->setEnabled(pDocument->document()->isModified());
        _pUndoAct->setEnabled(pDocument->document()->isUndoAvailable());
        _pRedoAct->setEnabled(pDocument->document()->isRedoAvailable());
    }

    _pBackwardAct->setEnabled(false);
    _pHomeAct->setEnabled(false);
    _pForwardAct->setEnabled(false);
    if (pDocument)
    {
        QFileInfo fi(pDocument->GetPathFileName());
        auto suffix = fi.suffix();

        if (suffix == "html" || suffix == "htm")
        {
            _pBackwardAct->setEnabled(true);
            _pHomeAct->setEnabled(true);
            _pForwardAct->setEnabled(true);
        }
    }

    bool textSelection = (pDocument &&
                          pDocument->textCursor().hasSelection());
    _pCutAct->setEnabled(textSelection);
    _pCopyAct->setEnabled(textSelection);
}

// Слот делает дочернее MDI окно активным
void MainWindow::SlotSetActiveSubWindow(QObject* pMdiSubWindow)
{
    if (pMdiSubWindow)
        _pMdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(pMdiSubWindow));
}

// Слот делает дочернее MDI окно активным по имени
void MainWindow::SlotSetActiveSubWindowByPath(QString path)
{

    QList<QMdiSubWindow*> listDocuments = _pMdiArea->subWindowList();
    for(auto& document: listDocuments)
    {
        auto doc = qobject_cast<DocumentWindow*>(document->widget());
        if(doc && (doc->GetPathFileName() == path))
        {
            _pMdiArea->setActiveSubWindow(document);
            return;
        }
    }
}

// Слот устанавливаем русский язык
void MainWindow::SlotSetupRussianLanguage()
{
    Settings::GetInstance().SetLanguage(Language::Russian);
    qDebug() << "Устанавливаем русский язык";
}

// Слот устанавливаем английский язык
void MainWindow::SlotSetupEnglishLanguage()
{
    Settings::GetInstance().SetLanguage(Language::English);
    qDebug() << "Устанавливаем английский язык";
}

// Слот устанавливаем светлую тему
void MainWindow::SlotSetupLightTheme()
{
    Settings::GetInstance().SetTheme(Theme::Light);
    qDebug() << "Устанавливаем светлую тему";
}

// Слот устанавливаем тёмную тему
void MainWindow::SlotSetupDarkTheme()
{
    Settings::GetInstance().SetTheme(Theme::Dark);
    qDebug() << "Устанавливаем тёмную тему";
}

// Слот добавления пути в список путей
void MainWindow::SlotOnOpen(QString path)
{
    _pListPath->append(path);
}

// Слот удаления пути из списка путей
void MainWindow::SlotDelPath(QString path)
{
//    for (int var = 0; var < _pListPath->size(); ++var) {
//        qDebug()<<_pListPath->at(var);
//    }
//    qDebug()<<_pListPath;
//    qDebug()<<_pListPath->indexOf(path);

    if(!path.isEmpty() && (_pListPath->indexOf(path) >= 0))
        _pListPath->remove(_pListPath->indexOf(path,0));

//    qDebug()<<"\n";
//    for (int var = 0; var < _pListPath->size(); ++var) {
//        qDebug()<<_pListPath->at(var);
    //    }
}

// Создаём объекты действий
void MainWindow::CreateActions()
{
    // Создание действия "Новый файл"
    _pNewAct = new QAction(tr("New File"), this);
    _pNewAct->setText(tr("&New"));
    _pNewAct->setShortcut(QKeySequence("CTRL+N"));
    _pNewAct->setToolTip(tr("New document"));
    _pNewAct->setStatusTip(tr("Create a new file"));
    _pNewAct->setWhatsThis(tr("Create a new file"));
    _pNewAct->setIcon(QPixmap(":/images/icons/filenew.png"));
    connect(_pNewAct, SIGNAL(triggered()), SLOT(SlotNewDoc()));

    // Создание действия "Открыть файл"
    _pOpenAct = new QAction(tr("Open file"), this);
    _pOpenAct->setText(tr("&Open..."));
    _pOpenAct->setShortcut(QKeySequence("CTRL+O"));
    _pOpenAct->setToolTip(tr("Open document"));
    _pOpenAct->setStatusTip(tr("Open an existing file"));
    _pOpenAct->setWhatsThis(tr("Open an existing file"));
    _pOpenAct->setIcon(QPixmap(":/images/icons/fileopen.png"));
    connect(_pOpenAct, SIGNAL(triggered()), SLOT(SlotLoad()));

    // Создание действия "Сохранить файл"
    _pSaveAct = new QAction(tr("Save file"), this);
    _pSaveAct->setText(tr("&Save"));
    _pSaveAct->setShortcut(QKeySequence("CTRL+S"));
    _pSaveAct->setToolTip(tr("Save document"));
    _pSaveAct->setStatusTip(tr("Save the file to disk"));
    _pSaveAct->setWhatsThis(tr("Save the file to disk"));
    _pSaveAct->setIcon(QPixmap(":/images/icons/filesave.png"));
    connect(_pSaveAct, SIGNAL(triggered()), SLOT(SlotSave()));

    // Создание действия "Сохранить файл как"
    _pSaveAsAct = new QAction(tr("Save file as..."), this);
    _pSaveAsAct->setText(tr("Save &As..."));
    _pSaveAsAct->setToolTip(tr("Save document as..."));
    _pSaveAsAct->setStatusTip(tr("Save the file to disk as..."));
    _pSaveAsAct->setWhatsThis(tr("Save the file to disk as..."));
    _pSaveAsAct->setIcon(QPixmap(":/images/icons/filesaveas.png"));
    connect(_pSaveAsAct, SIGNAL(triggered()), SLOT(SlotSaveAs()));

    // Создание действия "Печать файла"
    _pPrintAct = new QAction(tr("Print"), this);
    _pPrintAct->setText(tr("Print file"));
    _pPrintAct->setToolTip(tr("Print file"));
    _pPrintAct->setStatusTip(tr("Print file"));
    _pPrintAct->setWhatsThis(tr("Print file"));
    _pPrintAct->setIcon(QPixmap(":/images/icons/fileprint.png"));
    _pPrintAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
    connect(_pPrintAct, SIGNAL(triggered()), SLOT(SlotPrint()));

    // Создание действия "Предпросмотр перед печатю файла"
    _pPrintPreviewAct = new QAction(tr("Print preview"), this);
    _pPrintPreviewAct->setText(tr("Print preview"));
    _pPrintPreviewAct->setToolTip(tr("Print preview"));
    _pPrintPreviewAct->setStatusTip(tr("Print preview"));
    _pPrintPreviewAct->setWhatsThis(tr("Print preview"));
    _pPrintPreviewAct->setIcon(QPixmap(":/images/icons/frameprint.png"));
    connect(_pPrintPreviewAct, SIGNAL(triggered()), SLOT(SlotPrintPreview()));

    // Создание действия "Сохранить как *.odt"
    _pSaveAsOdt = new QAction(tr("Save as ODT"), this);
    _pSaveAsOdt->setText(tr("Save as ODT file"));
    _pSaveAsOdt->setToolTip(tr("Save as ODT file"));
    _pSaveAsOdt->setStatusTip(tr("Save as ODT file"));
    _pSaveAsOdt->setWhatsThis(tr("Save as ODT file"));
    _pSaveAsOdt->setIcon(QPixmap(":/images/icons/ooo_gulls.png"));
    connect(_pSaveAsOdt, SIGNAL(triggered()), SLOT(SlotSaveAsOdt()));

    // Создание действия "Сохранить как PDF файл"
    _pPrintPDFAct = new QAction(tr("Save as PDF"), this);
    _pPrintPDFAct->setText(tr("Save as PDF file"));
    _pPrintPDFAct->setToolTip(tr("Save as PDF file"));
    _pPrintPDFAct->setStatusTip(tr("Save as PDF file"));
    _pPrintPDFAct->setWhatsThis(tr("Save as PDF file"));
    _pPrintPDFAct->setIcon(QPixmap(":/images/icons/acroread.png"));
    connect(_pPrintPDFAct, SIGNAL(triggered()), SLOT(SlotPrintPDF()));

    // Создание действия "Назад"
    _pBackwardAct = new QAction(tr("Backward"), this);
    _pBackwardAct->setText(tr("Backward navigation"));
    _pBackwardAct->setStatusTip(tr("Backward navigation"));
    _pBackwardAct->setWhatsThis(tr("Backward navigation"));
    _pBackwardAct->setIcon(QPixmap(":/images/icons/back.png"));
    _pBackwardAct->setEnabled(false);
    connect(_pBackwardAct, SIGNAL(triggered()), SLOT(SlotBackward()));

    // Создание действия "Домой"
    _pHomeAct = new QAction(tr("Home"), this);
    _pHomeAct->setText(tr("Home navigation"));
    _pHomeAct->setStatusTip(tr("Home navigation"));
    _pHomeAct->setWhatsThis(tr("Home navigation"));
    _pHomeAct->setIcon(QPixmap(":/images/icons/gohome.png"));
    connect(_pHomeAct, SIGNAL(triggered()), SLOT(SlotHome()));

    // Создание действия "Впрёд"
    _pForwardAct = new QAction(tr("Forward"), this);
    _pForwardAct->setText(tr("Forward navigation"));
    _pForwardAct->setStatusTip(tr("Forward navigation"));
    _pForwardAct->setWhatsThis(tr("Forward navigation"));
    _pForwardAct->setIcon(QPixmap(":/images/icons/forward.png"));
    _pForwardAct->setEnabled(false);
    connect(_pForwardAct, SIGNAL(triggered()), SLOT(SlotForward()));

    // Создание действия "Вырезать"
    _pCutAct = new QAction(tr("Cut"), this);
    _pCutAct->setText(tr("Cu&t"));
    _pCutAct->setShortcuts(QKeySequence::Cut);
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
    _pCopyAct->setShortcuts(QKeySequence::Copy);
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
    _pPasteAct->setShortcuts(QKeySequence::Paste);
    _pPasteAct->setToolTip(tr("Paste text"));
    _pPasteAct->setStatusTip(
        tr("Paste the clipboard's contents into the current selection"));
    _pPasteAct->setWhatsThis(
        tr("Paste the clipboard's contents into the current selection"));
    _pPasteAct->setIcon(QPixmap(":/images/icons/editpaste.png"));
    connect(_pPasteAct, SIGNAL(triggered()), SLOT(SlotPaste()));

    // Создание действия "Отменить"
    _pUndoAct = new QAction(tr("Undo"), this);
    _pUndoAct->setText(tr("Undo"));
    _pUndoAct->setShortcuts(QKeySequence::Undo);
    _pUndoAct->setToolTip(tr("Undo changes"));
    _pUndoAct->setStatusTip(tr("Undo changes"));
    _pUndoAct->setWhatsThis(tr("Undo changes"));
    _pUndoAct->setIcon(QPixmap(":/images/icons/undo.png"));
//    connect(_pUndoAct, SIGNAL(triggered()), SLOT(SlotCut()));

    // Создание действия "Вернуть"
    _pRedoAct = new QAction(tr("Redo"), this);
    _pRedoAct->setText(tr("Redo"));
    _pRedoAct->setShortcuts(QKeySequence::Redo);
    _pRedoAct->setToolTip(tr("Redo changes"));
    _pRedoAct->setStatusTip(tr("Redo changes"));
    _pRedoAct->setWhatsThis(tr("Redo changes"));
    _pRedoAct->setIcon(QPixmap(":/images/icons/redo.png"));
//    connect(_pRedoAct, SIGNAL(triggered()), SLOT(SlotCopy()));

    // указатель на действие "Следующая вкладка"
    _pNextAct = new QAction(tr("Ne&xt"), this);
    _pNextAct->setShortcuts(QKeySequence::NextChild);
    _pNextAct->setStatusTip(tr("Move the focus to the next window"));

    // указатель на действие "Предыдующая вкладка"
    _pPreviousAct = new QAction(tr("Pre&vious"), this);
    _pPreviousAct->setShortcuts(QKeySequence::PreviousChild);
    _pPreviousAct->setStatusTip(tr("Move the focus to the previous window"));

    // Создание действия "Поиск"
    _pFindAct = new QAction(tr("Find"), this);
    _pFindAct->setText(tr("&Find"));
    _pFindAct->setToolTip(tr("Find text"));
    _pFindAct->setStatusTip(tr("Find text in current window"));
    _pFindAct->setWhatsThis(tr("Find text in current window"));
    _pFindAct->setIcon(QPixmap(":/images/icons/find.png"));
    connect(_pFindAct, SIGNAL(triggered()), SLOT(SlotFind()));

    // Создание действия добавить гиперссылку
    _pMakeLinkAct = new QAction(tr("Hyperlink"), this);
    _pMakeLinkAct->setText(tr("Hyperlink"));
    _pMakeLinkAct->setToolTip(tr("Make hyperlink"));
    _pMakeLinkAct->setStatusTip(tr("Make hyperlink"));
    _pMakeLinkAct->setWhatsThis(tr("Make hyperlink"));
    _pMakeLinkAct->setIcon(QPixmap(":/images/icons/cache.png"));
    connect(_pMakeLinkAct, SIGNAL(triggered()), SLOT(SlotMakeHyperlink()));

    // Создание действия добавить изображение
    _pAddImageAct = new QAction(tr("Image"), this);
    _pAddImageAct->setText(tr("Image"));
    _pAddImageAct->setToolTip(tr("Add image"));
    _pAddImageAct->setStatusTip(tr("Add image"));
    _pAddImageAct->setWhatsThis(tr("Add image"));
    _pAddImageAct->setIcon(QPixmap(":/images/icons/thumbnail.png"));
    connect(_pAddImageAct, SIGNAL(triggered()), SLOT(SlotAddImage()));

    // Создание действия "О программе"
    _pAboutAct = new QAction(tr("About"), 0);
    _pAboutAct->setText(tr("&About"));
    _pAboutAct->setToolTip(tr("Show About box"));
    _pAboutAct->setStatusTip(tr("Show the application's About box"));
    _pAboutAct->setWhatsThis(tr("Show the application's About box"));
    connect(_pAboutAct, SIGNAL(triggered()), SLOT(SlotAbout()));

    // Создание действия "О программе"
    _pHelpAct = new QAction(tr("&Help"), 0);
    _pHelpAct->setText(tr("&Help"));
    _pHelpAct->setShortcut(Qt::Key_F1);
    _pHelpAct->setToolTip(tr("Show help"));
    _pHelpAct->setStatusTip(tr("Show help"));
    _pHelpAct->setWhatsThis(tr("Show help"));
    _pHelpAct->setIcon(QPixmap(":/images/icons/help.png"));
    connect(_pHelpAct, SIGNAL(triggered()), SLOT(SlotHelp()));
}

// Создаём меню переключения языков
void MainWindow::SetupLanguageActions(QMenu *menu)
{
    QActionGroup* langGroup = new QActionGroup(this);
    langGroup->setExclusive(true);

    QAction* pSetRussian = new QAction(tr("Russian language"), this);
    pSetRussian->setText(tr("Russian language"));
    pSetRussian->setCheckable(true);
    langGroup->addAction(pSetRussian);
    menu->addAction(pSetRussian);

    QAction* pSetEnglish = new QAction(tr("English language"), this);
    pSetEnglish->setText(tr("English language"));
    pSetEnglish->setCheckable(true);
    langGroup->addAction(pSetEnglish);
    menu->addAction(pSetEnglish);

    if(Settings::GetInstance().GetLanguage() == Language::Russian)
        pSetRussian->setChecked(true);
    else
        pSetEnglish->setChecked(true);

    connect(pSetRussian, SIGNAL(triggered(bool)), SLOT(SlotSetupRussianLanguage()));
    connect(pSetEnglish, SIGNAL(triggered(bool)), SLOT(SlotSetupEnglishLanguage()));
}

// Создаём меню переключения тем интерфейса
void MainWindow::SetupThemeActions(QMenu *menu)
{
    QActionGroup* themeGroup = new QActionGroup(this);
    themeGroup->setExclusive(true);

    QAction* pSetLightTheme = new QAction(tr("Light theme"), this);
    pSetLightTheme->setText(tr("Light theme"));
    pSetLightTheme->setCheckable(true);
    themeGroup->addAction(pSetLightTheme);
    menu->addAction(pSetLightTheme);

    QAction* pSetDarkTheme = new QAction(tr("Dark theme"), this);
    pSetDarkTheme->setText(tr("Dark theme"));
    pSetDarkTheme->setCheckable(true);
    themeGroup->addAction(pSetDarkTheme);
    menu->addAction(pSetDarkTheme);

    if(Settings::GetInstance().GetTheme() == Theme::Light)
        pSetLightTheme->setChecked(true);
    else
        pSetDarkTheme->setChecked(true);

    connect(pSetLightTheme, SIGNAL(triggered(bool)), SLOT(SlotSetupLightTheme()));
    connect(pSetDarkTheme, SIGNAL(triggered(bool)), SLOT(SlotSetupDarkTheme()));
}

// Формирование экшена для жирного шрифта
void MainWindow::SetupBoldActions(QToolBar* toolBar, QMenu* menu)
{
    actionTextBold = menu->addAction(tr("&Bold"));
//    actionTextBold->setShortcut(Qt::CTRL | Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    actionTextBold->setIcon(QPixmap(":/images/icons/text_bold.png"));
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    toolBar->addAction(actionTextBold);
    actionTextBold->setCheckable(true);
}

// Формирование экшена для курсивного шрифта
void MainWindow::SetupItalicActions(QToolBar* toolBar, QMenu* menu)
{
    actionTextItalic = menu->addAction(tr("&Italic"));
    actionTextItalic->setPriority(QAction::LowPriority);
//    actionTextItalic->setShortcut(Qt::CTRL | Qt::Key_I);
    actionTextItalic->setIcon(QPixmap(":/images/icons/text_italic.png"));
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    toolBar->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);
}

// Формирование экшена для подчеркнутого шрифта
void MainWindow::SetupUnderLineActions(QToolBar* toolBar, QMenu* menu)
{
    actionTextUnderline = menu->addAction(tr("&Underline"));
//    actionTextUnderline->setShortcut(Qt::CTRL | Qt::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    actionTextUnderline->setIcon(QPixmap(":/images/icons/text_under.png"));
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    toolBar->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);
}

// Формирование экшена для изменения размера шрифта
void MainWindow::SetupSizeActions(QToolBar* toolBar)
{
    comboSize = new QComboBox(toolBar);
    comboSize->setObjectName("comboSize");
    toolBar->addWidget(comboSize);
    comboSize->setEditable(true);
    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes)
    {
        comboSize->addItem(QString::number(size));
    }
    comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));
}

// Mетод создает панели и меню форматирования текста
QToolBar* MainWindow::SetupFormatActions(QMenu* menu)
{
    QToolBar* toolBar = addToolBar(tr("Format Actions"));
    SetupBoldActions(toolBar, menu);
    SetupItalicActions(toolBar, menu);
    SetupUnderLineActions(toolBar, menu);
    menu->addSeparator();
    SetupFontColorActions(toolBar, menu);

    toolBar->addSeparator();
    toolBar->addAction(_pMakeLinkAct);
    toolBar->addAction(_pAddImageAct);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    return toolBar;
}

// Метод создает панели и меню конфигурирования шрифта
QToolBar *MainWindow::SetupFontActions()
{
    QToolBar* toolBar = addToolBar(tr("Font Actions"));
    comboFont = new QFontComboBox(toolBar);
    toolBar->addWidget(comboFont);
    SetupSizeActions(toolBar);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    return toolBar;
}

// Отображение настроек измененного шрифта
void MainWindow::FontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
}

//Mетод создает панель для изменения цвета шрифта
void MainWindow::SetupFontColorActions(QToolBar* toolBar, QMenu* menu)
{
    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = menu->addAction(pix, tr("&Color..."));
    toolBar->addAction(actionTextColor);
}

// Получение указателя на активное окно редактирования, удаляет старые соединения, устанавливает новые
void MainWindow::SetupActiveDocument(QMdiSubWindow* window)
{
    DisonnectFromDocument();
    if(window)
    {
        _pCurrentDocument = dynamic_cast<DocumentWindow*>(window->widget());
        if(_pCurrentDocument)
        {
            CurrentCharFormatChanged(_pCurrentDocument->currentCharFormat());
            ConnectToActiveDocument();
        }
    }
}

// Устанавливает новые соединения с окном редактирования
void MainWindow::ConnectToActiveDocument()
{
    if (_pCurrentDocument)
    {
        connect(comboFont, &QComboBox::textActivated, _pCurrentDocument, &DocumentWindow::TextFamily);
        connect(actionTextBold, &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextBold);
        connect(actionTextItalic, &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextItalic);
        connect(actionTextUnderline, &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextUnderline);
        connect(_pCurrentDocument, &QTextEdit::currentCharFormatChanged, this, &MainWindow::CurrentCharFormatChanged);
        connect (comboSize, &QComboBox::textActivated, _pCurrentDocument, &DocumentWindow ::TextSize);
        connect (actionTextColor, &QAction::triggered, _pCurrentDocument, &DocumentWindow::TextColor);

        connect(_pCurrentDocument, &QTextEdit::copyAvailable, _pCutAct, &QAction::setEnabled);
        connect(_pCurrentDocument, &QTextEdit::copyAvailable, _pCopyAct, &QAction::setEnabled);
        connect(_pCurrentDocument, &QTextEdit::textChanged, this, &MainWindow::SlotSaveEnable);
        connect(_pCurrentDocument, SIGNAL(backwardAvailable(bool)),
                _pBackwardAct, SLOT(setEnabled(bool)));
        connect(_pCurrentDocument, SIGNAL(forwardAvailable(bool)),
                _pForwardAct, SLOT(setEnabled(bool)));

        connect(_pUndoAct, &QAction::triggered,
                _pCurrentDocument, &DocumentWindow::undo);
        connect(_pRedoAct, &QAction::triggered,
                _pCurrentDocument, &DocumentWindow::redo);
        connect(_pCurrentDocument->document(), &QTextDocument::undoAvailable,
                _pUndoAct, &QAction::setEnabled);
        connect(_pCurrentDocument->document(), &QTextDocument::redoAvailable,
                _pRedoAct, &QAction::setEnabled);
    }
}

// Удаляет текущие соединения с  окном редактирования
void MainWindow::DisonnectFromDocument()
{
    if (!_pMdiArea->activeSubWindow())
        _pCurrentDocument = nullptr;

    if (_pCurrentDocument)
    {
        disconnect(comboFont, &QComboBox::textActivated, _pCurrentDocument, &DocumentWindow::TextFamily);
        disconnect(actionTextBold, &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextBold);
        disconnect(actionTextItalic, &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextItalic);
        disconnect(actionTextUnderline, &QAction::triggered, _pCurrentDocument, &DocumentWindow ::TextUnderline);
        disconnect(_pCurrentDocument, &QTextEdit::currentCharFormatChanged, this, &MainWindow::CurrentCharFormatChanged);
        disconnect (actionTextColor, &QAction::triggered, _pCurrentDocument, &DocumentWindow::TextColor);

        disconnect(_pCurrentDocument, &QTextEdit::copyAvailable, _pCutAct, &QAction::setEnabled);
        disconnect(_pCurrentDocument, &QTextEdit::copyAvailable, _pCopyAct, &QAction::setEnabled);
        disconnect(_pCurrentDocument, &QTextEdit::textChanged, this, &MainWindow::SlotSaveEnable);
        disconnect(_pCurrentDocument, SIGNAL(backwardAvailable(bool)),
                   _pBackwardAct, SLOT(setEnabled(bool)));
        disconnect(_pCurrentDocument, SIGNAL(forwardAvailable(bool)),
                   _pForwardAct, SLOT(setEnabled(bool)));

        disconnect(_pUndoAct, &QAction::triggered,
                   _pCurrentDocument, &DocumentWindow::undo);
        disconnect(_pRedoAct, &QAction::triggered,
                   _pCurrentDocument, &DocumentWindow::redo);
        disconnect(_pCurrentDocument->document(), &QTextDocument::undoAvailable,
                   _pUndoAct, &QAction::setEnabled);
        disconnect(_pCurrentDocument->document(), &QTextDocument::redoAvailable,
                   _pRedoAct, &QAction::setEnabled);
    }
}

// Устанавливает настройки, соответствующие формату шрифта
void MainWindow::CurrentCharFormatChanged(const QTextCharFormat &format)
{
    FontChanged(format.font());
    ColorChanged(format.foreground().color());
}
// Устанавливает настройки, соответствующие цвету шрифта
void MainWindow::ColorChanged(const QColor &color)
{
    QPixmap pix(16, 16);
    pix.fill(color);
    actionTextColor->setIcon(pix);
}
