/****************************************
* Команда 2
* Текстовый редактор
*
* Код класса документа в шаблоне MDI
*****************************************/

#include <QtWidgets>
#include <QFileDialog>
#include <QTextDocumentWriter>

#include "documentwindow.h"
#include "qevent.h"

DocumentWindow::DocumentWindow(QWidget* pParent /* = nullptr */) :
    QTextEdit(pParent)
{
}

bool DocumentWindow::OpenFile(const QString &pathFileName)
{
    if (pathFileName.isEmpty())
    {
        emit SignalStatusBarMessage(
            tr("Could not open \"%1\"").arg(QDir::toNativeSeparators(pathFileName)));
        return false;
    }

    QFile file(pathFileName);

    if(file.open(QIODevice::ReadOnly))
    {
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);

        QByteArray data = file.readAll();
        file.close();
        QMimeDatabase db;
        const QString &mimeTypeName = db.mimeTypeForFileNameAndData(pathFileName, data).name();
        if (mimeTypeName == u"text/html")
        {
            auto encoding = QStringDecoder::encodingForHtml(data);
            QString str = QStringDecoder(encoding ? *encoding : QStringDecoder::Utf8)(data);
            QUrl fileUrl =
                pathFileName.startsWith(u':') ? QUrl(pathFileName) : QUrl::fromLocalFile(pathFileName);
            document()->setBaseUrl(fileUrl.adjusted(QUrl::RemoveFilename));
            setHtml(str);
        }
        else if (mimeTypeName == u"text/markdown")
            setMarkdown(QString::fromUtf8(data));
        else
            setPlainText(QString::fromUtf8(data));

        QFileInfo fi(pathFileName);
        QString fileName = fi.fileName();
        setWindowTitle(fileName);

        _pathFileName = pathFileName;

        emit SignalStatusBarMessage(
            tr("Opened \"%1\"").arg(QDir::toNativeSeparators(pathFileName)));
        QGuiApplication::restoreOverrideCursor();
        return true;
    }
    else
    {
        emit SignalStatusBarMessage(
            tr("Could not open \"%1\"").arg(QDir::toNativeSeparators(pathFileName)));
        return false;
    }
}

void DocumentWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

// Метод загрузки документа
bool DocumentWindow::Load()
{
    // Антон us2_t-003 Спринт 1: Реализовать загрузку файла
    QFileDialog fileDialog(this, tr("Open File..."), QDir::currentPath());
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setMimeTypeFilters({"text/plain", "text/html", "text/markdown"});
    if (fileDialog.exec() != QDialog::Accepted)
        return false;

    const QString pathFileName = fileDialog.selectedFiles().constFirst();

    return OpenFile(pathFileName);
}

// Метод сохранения документа
void DocumentWindow::Save()
{
    // TODO us2_t-002 Спринт 1: Реализовать сохранение файла

}

// Метод сохранить документ как...
void DocumentWindow::SaveAs()
{
    // TODO us2_t-002 Спринт 1: Реализовать сохранение файла

}
