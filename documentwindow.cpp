/****************************************
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
