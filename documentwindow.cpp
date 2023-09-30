/****************************************4
* Команда 2
* Гипертекстовый редактор
*
* Код класса документа в шаблоне MDI
*****************************************/

#include <QtWidgets>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QColor>
#include <QColorDialog>
#include "documentwindow.h"
#include "qevent.h"

DocumentWindow::DocumentWindow(QWidget* pParent /* = nullptr */) :
    QTextBrowser(pParent)
{
    setOpenExternalLinks(true);
    setReadOnly(false);
    setTextInteractionFlags(Qt::TextSelectableByMouse |
                            Qt::LinksAccessibleByMouse |
                            Qt::LinksAccessibleByKeyboard |
                            Qt::TextSelectableByMouse |
                            Qt::TextSelectableByKeyboard |
                            Qt::TextEditable);
    setUndoRedoEnabled(true);
 }

// Метод загрузки файла и чтения из него текста
bool DocumentWindow::OpenFile(const QString &pathFileName)
{
    if (pathFileName.isEmpty())
    {
        emit SignalStatusBarMessage(tr("Path empty!"));
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

            QFileInfo fi(pathFileName);
            setSearchPaths({fi.path()});
            setSource(fi.fileName());
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
            tr("File opened: \"%1\"").arg(QDir::toNativeSeparators(pathFileName)));
        QGuiApplication::restoreOverrideCursor();
        emit IsOpen(pathFileName);
        return true;
    }
    else
    {
        emit SignalStatusBarMessage(
            tr("File could not open: \"%1\"").arg(QDir::toNativeSeparators(pathFileName)));
        return false;
    }
}

// Перегруженный метод закрытия виджета
void DocumentWindow::closeEvent(QCloseEvent *event)
{
    emit IsClose(_pathFileName);
    event->accept();
}

// Метод загрузки документа
QString DocumentWindow::Load()
{
    // Антон us2_t-003 Спринт 1: Реализовать загрузку файла
    QFileDialog fileDialog(nullptr, tr("Open File..."), QDir::currentPath());
    fileDialog.setOptions(QFileDialog::DontUseNativeDialog);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setMimeTypeFilters({"text/html", "text/markdown", "text/plain"});
    if (fileDialog.exec() != QDialog::Accepted)
        return "";

    const QString pathFileName = fileDialog.selectedFiles().constFirst();

    return pathFileName;
    //return OpenFile(pathFileName);
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
    QFileDialog fileDialog(this, tr("Save as..."), QDir::currentPath());
    fileDialog.setOptions(QFileDialog::DontUseNativeDialog);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList mimeTypes{"text/html",
                          "text/plain",
                          "application/vnd.oasis.opendocument.text",
                          "text/markdown"};
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.setDefaultSuffix("odt");
    if (fileDialog.exec() != QDialog::Accepted)
        return false;
    const QString pathFileName = fileDialog.selectedFiles().constFirst();

    if (pathFileName.isEmpty())
        return false;

    bool res = SaveFile(pathFileName);

    QFileInfo fi(pathFileName);
    QString fileName = fi.fileName();
    setWindowTitle(fileName);
    auto suffix = fi.suffix();
    if (suffix == "html" || suffix == "htm")
    {
        setSearchPaths({fi.path()});
        setSource(fi.fileName());
    }

    _pathFileName = pathFileName;

    return res;
}

// Метод сохранение файла
bool DocumentWindow::SaveFile(const QString& pathFileName)
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    QTextDocumentWriter writer(pathFileName);

//    QSaveFile file(pathFileName);
//    if(file.open(QFile::WriteOnly | QFile::Text))
//    {
//        QTextStream out(&file);
//        out << toPlainText();

    if(!writer.write(document())) //(!file.commit())
    {
        errorMessage = tr("Cannot save file %1")
                           .arg(QDir::toNativeSeparators(pathFileName));
    }

//    }
//    else
//    {
//        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
//                           .arg(QDir::toNativeSeparators(pathFileName), file.errorString());
//    }

    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("Save file"), errorMessage);
        return false;
    }

    return true;
}

// Метод сохранить документ как *.odt
void DocumentWindow::SaveAsOdt(const QString fileName)
{
    QTextDocumentWriter writer;
    writer.setFormat("odf");
//    QFileInfo fi(_pathFileName);
    writer.setFileName(fileName);   // fi.baseName() + ".odt");
    writer.write(document());
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

// устанавливает шрифт
void DocumentWindow :: TextFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamilies({f});       // fmt.setFontFamily(f);
    MergeFormatOnWordOrSelection(fmt);
}

// выбор и установка цвета шрифта
void DocumentWindow :: TextColor()
{
    QColor color = QColorDialog::getColor(this->textColor(), this);
    if (!color.isValid())
    {
        return;
    }
    QTextCharFormat format;
    format.setForeground(color);
    MergeFormatOnWordOrSelection(format);
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
void DocumentWindow::Find(QString searchRequest, bool wholeText, bool caseSensitive, bool backward)
{
    bool found = false;

    QTextDocument::FindFlags flags;
    if(wholeText)
        flags |= QTextDocument::FindWholeWords;
    if(caseSensitive)
        flags |= QTextDocument::FindCaseSensitively;
    if(backward)
        flags |= QTextDocument::FindBackward;

    QTextCursor cursor = textCursor();
    cursor.clearSelection();

    cursor = document()->find(searchRequest, cursor, flags);
    if(!cursor.isNull())
    {
        found = true;
        if (backward)
        {
            QTextCursor saveCursor = cursor;
            cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
            cursor.setPosition(saveCursor.position()-searchRequest.length(), QTextCursor::KeepAnchor);
            setTextCursor(cursor);
        }
        else
            setTextCursor(cursor);

        activateWindow();
    }

    if (!found)
        QMessageBox::information(this, tr("Find"), tr("Sequence not found!"));
}

// Возвращает выделенный текст
QString DocumentWindow::GetSelectedText()
{
    return textCursor().selectedText();
}

// Создание гипертекстовой ссылки
void DocumentWindow::MakeHyperlink(const QString linkText, QString linkTarget)
{
    QFileInfo fit(linkTarget);
    auto suffix = fit.suffix();

    if (suffix != "html" && suffix != "htm")
        linkTarget += ".html";

    //Конветируем абсолютные пути в относительные
    QFileInfo fi(_pathFileName);
    int pathPos = linkTarget.indexOf(fi.path());
    if (pathPos >= 0)
    {
        QString relativePath = linkTarget.mid((pathPos + fi.path().length())+1, linkTarget.length());
        if (!relativePath.isEmpty())
            linkTarget = relativePath;
    }

    insertHtml(QString("<a href=\"%1\">%2</a> ").arg(linkTarget, linkText));

    Save();
}

// Добавить изображение
void DocumentWindow::AddImage()
{
    QFileDialog fileDialog(this, tr("Add image"), QDir::currentPath());
    fileDialog.setOptions(QFileDialog::DontUseNativeDialog);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);

//    QStringList mimeTypeFilters({"image/jpeg", // "JPEG image (*.jpeg *.jpg *.jpe)
//                                 "image/png",  // "PNG image (*.png)"
//                                 "application/octet-stream" // "All files (*)"
//    });
//    QStringList mimeTypeFilters({"image/*"});
//    fileDialog.setMimeTypeFilters(mimeTypeFilters);
//    fileDialog.setDefaultSuffix("JPEG");

    fileDialog.setNameFilter("Images files (*.png *.jpeg *.jpg *.jpe *.xpm)");

    if (fileDialog.exec() != QDialog::Accepted)
        return;

    QString pathFileName = fileDialog.selectedFiles().constFirst();

    if (pathFileName.isEmpty())
        return;

//    insertHtml(QString("<img src=\"%1\" />").arg(pathFileName));

    //Конветируем абсолютные пути в относительные
    QFileInfo fi(_pathFileName);
    int pathPos = pathFileName.indexOf(fi.path());
    if (pathPos >= 0)
    {
        QString relativePath = pathFileName.mid((pathPos + fi.path().length())+1, pathFileName.length());
        if (!relativePath.isEmpty())
            pathFileName = relativePath;
    }

    //QUrl Uri(QString("file://%1").arg(pathFileName));
    QImage image = QImageReader(pathFileName).read();

    QTextDocument* textDocument = document();
    textDocument->addResource(QTextDocument::ImageResource, pathFileName/*Uri*/, QVariant(image));
    QTextCursor cursor = textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth(image.width());
    imageFormat.setHeight(image.height());
    imageFormat.setName(pathFileName/*Uri.toString()*/);
    cursor.insertImage(imageFormat);
}

void DocumentWindow::TextAlign(Qt::Alignment alignment)
{
    this->setAlignment(alignment);
}

void DocumentWindow::Indent()
{
    ModifyIndentation(1);
}

void DocumentWindow::Unindent()
{
    ModifyIndentation(-1);
}

void DocumentWindow::ModifyIndentation(int amount)
{
    QTextCursor cursor = this->textCursor();
    cursor.beginEditBlock();
    if (cursor.currentList()) {
        QTextListFormat listFmt = cursor.currentList()->format();
        QTextCursor above(cursor);
        above.movePosition(QTextCursor::Up);
        if (above.currentList() && listFmt.indent() + amount == above.currentList()->format().indent()) {
            above.currentList()->add(cursor.block());
        } else {
            listFmt.setIndent(listFmt.indent() + amount);
            cursor.createList(listFmt);
        }
    } else {
        QTextBlockFormat blockFmt = cursor.blockFormat();
        blockFmt.setIndent(blockFmt.indent() + amount);
        cursor.setBlockFormat(blockFmt);
    }
    cursor.endEditBlock();
}

