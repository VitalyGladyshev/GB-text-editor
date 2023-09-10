/****************************************4
* Команда 2
* Текстовый редактор
*
* Код класса документа в шаблоне MDI
*****************************************/

#include "documentwindow.h"

DocumentWindow::DocumentWindow(QWidget* pParent /* = nullptr */) :
    QTextEdit(pParent)
{
}

// Слот загрузки документа
void DocumentWindow::SlotLoad()
{
    // TODO us2_t-003 Спринт 1: Реализовать загрузку файла
}

// Слот сохранения документа
void DocumentWindow::SlotSave()
{
    // TODO us2_t-002 Спринт 1: Реализовать сохранение файла
}

// Слот сохранить документ как...
void DocumentWindow::SlotSaveAs()
{
    // TODO us2_t-002 Спринт 1: Реализовать сохранение файла
}



//устанавливает жирный шрифт
void DocumentWindow::TextBold(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);
    MergeFormatOnWordOrSelection(fmt);
}

//устанавливает подчеркнутый шрифт
void DocumentWindow::TextUnderline(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(checked);
    MergeFormatOnWordOrSelection(fmt);
}

//устанавливает курсиывный шрифт
void DocumentWindow::TextItalic(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(checked);
    MergeFormatOnWordOrSelection(fmt);
}


//устанавливает семейство шрифта
void DocumentWindow::TextFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
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


