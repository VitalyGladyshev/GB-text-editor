/****************************************
* Команда 2
* Текстовый редактор
*
* Код класса документа в шаблоне MDI
*****************************************/

#include "documentwindow.h"
#include <QFileDialog>

DocumentWindow::DocumentWindow(QWidget* pParent /* = nullptr */) :
    QTextEdit(pParent)
{
}

// Слот загрузки документа
void DocumentWindow::SlotLoad()
{
    // TODO us2_t-003 Спринт 1: Реализовать загрузку файла
    QString filename =  QFileDialog::getOpenFileName(
        this,
        "Open Document",
        QDir::currentPath(),
        "All files (*.*) ;; Text files (*.txt)");

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly)) {}

    QTextStream in(&file);

    QString txt;

    while(!in.atEnd())
    {
        QString line = in.readLine();
        txt = txt + line + "\n";
    }

    file.close();

    this -> setText(txt);
    this -> setWindowTitle(filename);
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
