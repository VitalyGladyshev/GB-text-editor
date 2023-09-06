#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QFileSystemModel>
#include <QTreeView>
#include <QMouseEvent>
class MainWindow;


class FileManager : public QTreeView{
    Q_OBJECT
public:
    /*!
     * \brief FileManager
     * \param parent
     */
    explicit FileManager(QWidget *parent = nullptr);
    /*!
     * \brief mouseDoubleClickEvent переопределенный метод двойного нажатия левой кнопки мыши
     * \param event открывает файл в TextEditor
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    QFileSystemModel * fileSystem;  //указатель на модель системы
    MainWindow *window;            // указатель на главное окно
};

#endif // FILEMANAGER_H
