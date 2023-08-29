/****************************************
* Команда 2
* Текстовый редактор
*
* Точка входа приложения - функция main
*****************************************/

#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("Text Editor");
    QCoreApplication::setOrganizationName("GB command 2");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    // Средства интернационализации
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "TextEditor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow textEditor;  // Создание экземпляра класса главного окна

    // Задание оптимальных размеров окна
    const QRect availableGeometry = textEditor.screen()->availableGeometry();
    textEditor.resize(availableGeometry.width() / 2,
                      (availableGeometry.height() * 2) / 3);
    textEditor.move((availableGeometry.width() - textEditor.width()) / 2,
            (availableGeometry.height() - textEditor.height()) / 2);

    textEditor.show();
    return a.exec();
}
