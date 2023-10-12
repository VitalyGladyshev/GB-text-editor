/**************************************
* Команда 2
* Гипертекстовый редактор
*
* Код класса настроек
***************************************/

#include <QApplication>
#include <QFile>
#include <QDomDocument>
#include <QDebug>

#include "settings.h"

Settings::Settings():
    _language(Language::Russian),
    _theme(Theme::Light)
{
    if (!LoadSettings())
        SaveSettings();
}

// Задать значение настройки текущего языка интерфейса
void Settings::SetLanguage(const Language language)
{
    _language = language;

    SaveSettings();
}

// Задать значение настройки текущей темы интерфейса
void Settings::SetTheme(const Theme theme)
{
    _theme = theme;

    SaveSettings();
}

// Загрузка настроек
bool Settings::LoadSettings()
{
    QFile settingsFile(QApplication::applicationDirPath() + "/settings.xml");
    if(settingsFile.open(QIODevice::ReadOnly))
    {
        bool error = false;
        QDomDocument docSet;
        if (docSet.setContent(&settingsFile))
        {
            QDomElement domEl = docSet.documentElement();
            QDomNode domNode = domEl.firstChild();
            QDomElement domGetEl = domNode.toElement();

            if (!domGetEl.isNull())
            {
                int iLng = domGetEl.text().toInt();
                if (iLng >= 0 && iLng <= 1)
                    _language = static_cast<Language>(iLng);
                else
                {
                    error = true;
                    _language = Language::Russian;
                }
            }
            domNode = domNode.nextSibling();
            domGetEl = domNode.toElement();
            if (!domGetEl.isNull())
            {
                int iThm = domGetEl.text().toInt();
                if (iThm >= 0 && iThm <= 1)
                    _theme = static_cast<Theme>(iThm);
                else
                {
                    error = true;
                    _theme = Theme::Light;
                }
            }
        }
        else
            error = true;
        settingsFile.close();

        if(error)
            qDebug() << "Read file structure error!";
        return !error;
    }
    else
    {
        qDebug() << "Read file error!";
        return false;
    }
}

// Сохранение настроек
bool Settings::SaveSettings()
{
    QFile settingsFile(QApplication::applicationDirPath() + "/settings.xml");

    if (settingsFile.open(QIODevice::WriteOnly))
    {
        QDomDocument doc("hypertext");
        QDomElement settings = doc.createElement("settings");

        doc.appendChild(settings);
        QDomElement language = doc.createElement("language");
        QDomText domTextW = doc.createTextNode(QString("%1").arg(static_cast<int>(_language)));
        language.appendChild(domTextW);

        settings.appendChild(language);
        QDomElement theme = doc.createElement("theme");
        QDomText domTextH = doc.createTextNode(QString("%1").arg(static_cast<int>(_theme)));
        theme.appendChild(domTextH);
        settings.appendChild(theme);

        doc.appendChild(settings);
        QTextStream(&settingsFile) << doc.toString();
        settingsFile.close();
        return true;
    }
    else
    {
        qDebug() << "Save file error!";
        return false;
    }
}
