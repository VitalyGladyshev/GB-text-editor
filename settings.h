/**************************************
* Команда 2
* Гипертекстовый редактор
*
* Файл заголовков класса окна настроек
***************************************/

#pragma once

/*!
 * \brief The Languadge enum Перечисление доступных языков интерфейса
 */
enum class Languadge
{
    Russian,
    English
};

/*!
 * \brief The Theme enum Перечисление доступных тем интерфейса
 */
enum class Theme
{
    Light,
    Dark
};

/*!
 * \brief The settings Класс для загрузки, доступа и сохранения настроек
 */
class settings
{
public:
    /// Конструктор
    settings();
};

