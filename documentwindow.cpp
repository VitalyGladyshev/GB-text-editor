/****************************************4
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
    QTextBrowser(pParent), _prevSearchRequest("")
{
    setOpenExternalLinks(true);
    setReadOnly(false);
    setTextInteractionFlags(Qt::TextSelectableByMouse |
                            Qt::LinksAccessibleByMouse |
                            Qt::LinksAccessibleByKeyboard |
                            Qt::TextSelectableByMouse |
                            Qt::TextSelectableByKeyboard |
                            Qt::TextEditable);
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
    fileDialog.setMimeTypeFilters({"text/html", "text/markdown", "text/plain"});
    if (fileDialog.exec() != QDialog::Accepted)
        return false;

    const QString pathFileName = fileDialog.selectedFiles().constFirst();

    return OpenFile(pathFileName);
}

// Метод сохранения документа
bool DocumentWindow::Save()
{
    // Александр us2_t-002 Спринт 1: Реализовать сохранение файла
    if (_pathFileName.isEmpty())
        return SaveAs();
    else
        return SaveFile(_pathFileName);
}

// Метод сохранить документ как...
bool DocumentWindow::SaveAs()
{
    // Александр us2_t-002 Спринт 1: Реализовать сохранение файла
    QString pathFileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save As"), QDir::currentPath());
    if (pathFileName.isEmpty())
        return false;

    bool res = SaveFile(pathFileName);

    QFileInfo fi(pathFileName);
    QString fileName = fi.fileName();
    setWindowTitle(fileName);

    _pathFileName = pathFileName;

    return res;
}

// Метод сохранение файла
bool DocumentWindow::SaveFile(QString& pathFileName)
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    QSaveFile file(pathFileName);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << toPlainText();

        if (!file.commit())
        {
            errorMessage = tr("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(pathFileName), file.errorString());
        }
    }
    else
    {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                           .arg(QDir::toNativeSeparators(pathFileName), file.errorString());
    }

    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("MDI"), errorMessage);
        return false;
    }

    return true;
}

// устанавливает жирный шрифт
void DocumentWindow::TextBold(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);
    MergeFormatOnWordOrSelection(fmt);
}

// устанавливает подчеркнутый шрифт
void DocumentWindow::TextUnderline(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(checked);
    MergeFormatOnWordOrSelection(fmt);
}

// устанавливает курсивный шрифт
void DocumentWindow::TextItalic(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(checked);
    MergeFormatOnWordOrSelection(fmt);
}

// устанавливает семейство шрифта
void DocumentWindow::TextFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamilies({f});       // fmt.setFontFamily(f);
    MergeFormatOnWordOrSelection(fmt);
}

// устанавливает формат текста
void DocumentWindow::MergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    this->mergeCurrentCharFormat(format);
}

// устанавливает размер шрифта
void DocumentWindow::TextSize(const QString &size)
{
    float pointSize = size.toFloat();
    if (pointSize > 0)
    {
        QTextCharFormat format;
        format.setFontPointSize(pointSize);
        this->mergeCurrentCharFormat(format);
    }
}

// Поиск в тексте
void DocumentWindow::Find(QString searchRequest, bool wholeText, bool caseSensitive)
{
    bool found = false;

    QTextCursor highlightCurs(document());
    QTextCursor cursor = textCursor();//(document());

    cursor.beginEditBlock();

    QTextCharFormat plainF(highlightCurs.charFormat());
    QTextCharFormat colorF = plainF;
    colorF.setForeground(Qt::red);

    QTextDocument::FindFlags flags;
    if(wholeText)
        flags |= QTextDocument::FindWholeWords;
    if(caseSensitive)
        flags |= QTextDocument::FindCaseSensitively;

    while(!highlightCurs.isNull() && !highlightCurs.atEnd())
    {
        highlightCurs = document()->find(searchRequest, highlightCurs, flags);

        if(!highlightCurs.isNull())
        {
            found = true;
            highlightCurs.movePosition(QTextCursor::WordRight, QTextCursor::KeepAnchor);
            highlightCurs.mergeCharFormat(colorF);
        }
    }

    cursor.endEditBlock();

    if (!found)
        QMessageBox::information(this, tr("Not found"), tr("Sequence not found!"));
}
