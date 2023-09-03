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
    // TODO us2_t-001 Спринт 1:   Реализовать тулбар

    // Создание действия "Новый файл"
    QAction* pactNew = new QAction(tr("New File"), 0);
    pactNew->setText(tr("&New"));
    pactNew->setShortcut(QKeySequence("CTRL+N"));
    pactNew->setToolTip(tr("New Document"));
    pactNew->setStatusTip(tr("Create a new file"));
    pactNew->setWhatsThis(tr("Create a new file"));
//    pactNew->setIcon(QPixmap(":/filenew.png"));
    connect(pactNew, SIGNAL(triggered()), SLOT(SlotNewDoc()));

    // Создание действия "Сохранить файл"
    QAction* pactOpen = new QAction(tr("Open File"), 0);
    pactOpen->setText(tr("&Open..."));
    pactOpen->setShortcut(QKeySequence("CTRL+O"));
    pactOpen->setToolTip(tr("Open Document"));
    pactOpen->setStatusTip(tr("Open an existing file"));
    pactOpen->setWhatsThis(tr("Open an existing file"));
//    pactOpen->setIcon(QPixmap(":/fileopen.png"));
    connect(pactOpen, SIGNAL(triggered()), SLOT(SlotLoad()));

    // Создание действия "Сохранить файл как"
    QAction* pactSave = new QAction(tr("Save File"), 0);
    pactSave->setText(tr("&Save"));
    pactSave->setShortcut(QKeySequence("CTRL+S"));
    pactSave->setToolTip(tr("Save Document"));
    pactSave->setStatusTip(tr("Save the file to disk"));
    pactSave->setWhatsThis(tr("Save the file to disk"));
//    pactSave->setIcon(QPixmap(":/filesave.png"));
    connect(pactSave, SIGNAL(triggered()), SLOT(SlotSave()));

    // Создаём пункт меню "Файл" главного окна
    QMenu* pmnuFile = new QMenu(tr("&File"));
    pmnuFile->addAction(pactNew);
    pmnuFile->addAction(pactOpen);
    pmnuFile->addAction(pactSave);
    pmnuFile->addAction(tr("Save &As..."), this, SLOT(SlotSaveAs()));
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
    addToolBar(bar);
        
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
//    pDocument->setWindowIcon(QPixmap(":/filenew.png"));
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
