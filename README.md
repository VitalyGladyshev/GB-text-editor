# Гипертекстовый редактор

**Курс: Командная разработка**

## Команда 2

1.  Беляев Алексей
2.  Гладышев Виталий
3.  Гребенюк Мария
4.  Замятин Александр
5.  Свечников Антон
6.  Палатов Анатолий
7.  Пименов Андрей
8.  Чекменёв Руслан

## Описание

**Гипертекстовый редактор** - программа позволяющая создавать и редактировать материалы с гипертекстовой разметкой, удобной для создания справочных систем программного обеспечения.

Гипертекстовый редактор выполнен как итоговый проект **Команды 2** факультета Гикбрейнс "Разработка на С++" по предмету "Командная разработка"

**Гипертекстовый редактор - главное окно**

![Главное окно](https://github.com/VitalyGladyshev/GB-text-editor/blob/documentation/docs/скрин.png)

## Установка

Гипертекстовый редактор - кроссплатформенное приложение для ОС Windows и Linux, совместимых с Qt6.

### Дистрибутив для Windows

Для ОС Windows поставляется в виде дистрибутива

[Дистрибутив](https://drive.google.com/file/d/1c6_23hIyasb7K5QCT9YKr3OJuXb9hawd/view?usp=drive_link)

**Примечание:** В ряде случаев может потребоваться установка [Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)

### Сборка из исходного кода

Гипертекстовый редактор может быть собран из исходного кода (Windows, Linux). Для сборки необходимы следующие компоненты:

- пакет компиляторов C++ 17 версии стандарта C++ ISO/IEC (или более поздней), например соответствующие версии MinGW, MSVC
- фреймворк Qt6 (сборка протестирована на Qt 6.5.2 ОС Windows 10 и 11, Qt 6.4.2 Linux Debian 12)
- QtCreator (совместимый с Qt6)
- qmake

**Примечание:** Разрядность ОС и соответствующих версий компонентов должны совпадать. Версии компонентов Qt должны соответствовать типу компилятора (MinGW, MSVC)

## Документация
- [Руководство оператора](https://vitalygladyshev.github.io/GB-text-editor/help/index.html)
- [Список документов проекта](https://github.com/VitalyGladyshev/GB-text-editor/blob/documentation/docs/Команда%202%20git.pdf)
- [User stories](https://github.com/VitalyGladyshev/GB-text-editor/blob/documentation/docs/User%20Stories.pdf)
- [Таскменеджер](https://gb-project.yougile.com/team/dd43605206dc/Текстовый-редактор?lang=ru)
- [Правила использования репозитория](https://github.com/VitalyGladyshev/GB-text-editor/blob/documentation/docs/Команда%202%20Репозиторий%20проекта.pdf)
- [Руководство по стилю С++](https://github.com/VitalyGladyshev/GB-text-editor/blob/documentation/docs/Руководство%20по%20стилю%20c%2B%2B.pdf)
- [Руководство программиста](https://vitalygladyshev.github.io/GB-text-editor/html/index.html)
- [Анализ рисков проекта](https://github.com/VitalyGladyshev/GB-text-editor/blob/documentation/docs/Команда%202%20Анализ%20рисков%20реализации%20проекта%20Гипертекстовый%20редактор.pdf)
- [Тестирование](https://github.com/VitalyGladyshev/GB-text-editor/blob/documentation/docs/Тестирование.pdf)

**Диаграмма классов**
![Диаграмма классов](https://github.com/VitalyGladyshev/GB-text-editor/blob/documentation/docs/diagram.png)
