#include "documentwindow.h"
#include "filemanager.h"
#include "mainwindow.h"
#include "qmdiarea.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QMdiArea>


FileManager::FileManager(QWidget *pParent) : QTreeView(pParent){
    window = dynamic_cast<MainWindow*>(parent());
    fileSystem = new QFileSystemModel;
    fileSystem->setRootPath(QDir::currentPath());
    setModel(fileSystem);
    hideColumn(1);
    hideColumn(2);
    hideColumn(3);
    setMinimumWidth(240);
    setMinimumHeight(480);
    QModelIndex index = fileSystem->index(QDir::currentPath());
    expand(index);
    scrollTo(index);
    resizeColumnToContents(0);
    setIndentation(5);

}

void FileManager::mouseDoubleClickEvent(QMouseEvent *event)
{
    DocumentWindow *doc = new DocumentWindow();
    if(window) window->getMdi()->addSubWindow(doc);
    doc->setAttribute(Qt::WA_DeleteOnClose);
    if(event->button() == Qt::LeftButton) {
        QModelIndex index = indexAt(event->pos());
        if (fileSystem->fileInfo(index).isFile()) {
            QString pathToFile = fileSystem->fileInfo(index).absoluteFilePath();
            setColumnWidth(1, 100);
            QFile file(pathToFile);
            if(file.open(QIODevice::ReadOnly)) {
                QTextStream in(&file);
                QString txt;
                while(!in.atEnd())
                {
                    QString line = in.readLine();
                    txt = txt + line + "\n";
                }
                file.close();
                QFileInfo fi(pathToFile);
                QString fileName = fi.fileName();
                doc->setText(txt);
                doc -> setWindowTitle(fileName);
                doc->show();
            }
        } else {
            if (isExpanded(index)) setExpanded(index, false);
            else setExpanded(index, true);
        }
    }

}
