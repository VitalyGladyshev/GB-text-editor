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
#include <QCommandLineParser>
//#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication appl(argc, argv);
    QCoreApplication::setApplicationName("Text Editor");
    QCoreApplication::setOrganizationName("GB command 2");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    // Средства интернационализации
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale: uiLanguages)
    {
        const QString baseName = "TextEditor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            appl.installTranslator(&translator);
            break;
        }
    }

    // Инициализация парсера параметров командной строки
    QCommandLineParser parser;
    parser.setApplicationDescription("Text Editor");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(appl);

    MainWindow textEditor;  // Создание экземпляра класса главного окна

    // Открываем файлы, переданные в командной строке
    const QStringList posArgs = parser.positionalArguments();
    for (const QString &fileName : posArgs)
        textEditor.OpenFile(fileName);

    // Задание оптимальных размеров окна
    const QRect availableGeometry = textEditor.screen()->availableGeometry();
    textEditor.resize(availableGeometry.width() / 2,
                      (availableGeometry.height() * 2) / 3);
    textEditor.move((availableGeometry.width() - textEditor.width()) / 2,
            (availableGeometry.height() - textEditor.height()) / 2);

    textEditor.show();
    return appl.exec();
}
