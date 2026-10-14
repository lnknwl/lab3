#include <cmath>
#include <iostream>
#include <stdexcept>
#include "Complex.h"

int Complex::objectCount = 0;

/**
 * @brief Создает комплексное число с нулевыми частями
 */
Complex::Complex()
    : Complex(0.0, 0.0)
{
}

/**
 * @brief Создает комплексное число с заданными частями
 */
Complex::Complex(double realPart, double imaginaryPart)
    : realPart(realPart), imaginaryPart(imaginaryPart)
{
    if (!std::isfinite(realPart) || !std::isfinite(imaginaryPart) ||
        !std::isfinite(std::hypot(realPart, imaginaryPart)))
    {
        throw std::invalid_argument("Части числа и его модуль должны быть конечными");
    }

    ++objectCount;
}

/**
 * @brief Создает копию комплексного числа
 *
 * @param other Число для копирования
 */
Complex::Complex(const Complex& other)
    : realPart(other.realPart), imaginaryPart(other.imaginaryPart)
{
    ++objectCount;
}

/**
 * @brief Завершает время жизни числа и уменьшает счетчик
 */
Complex::~Complex()
{
    --objectCount;
}

/**
 * @brief Изменяет действительную часть, проверяя результат
 */
void Complex::changeRealPart(double change)
{
    double newRealPart = realPart + change;

    if (!std::isfinite(newRealPart) || newRealPart == realPart ||
        !std::isfinite(std::hypot(newRealPart, imaginaryPart)))
    {
        throw std::overflow_error("Изменение действительной части нельзя сохранить");
    }

    realPart = newRealPart;
}

/**
 * @brief Увеличивает действительную часть на один
 */
Complex& Complex::operator++()
{
    changeRealPart(1.0);
    return *this;
}

/**
 * @brief Увеличивает действительную часть и возвращает старое число
 */
Complex Complex::operator++(int)
{
    Complex oldValue(*this);
    ++(*this);
    return oldValue;
}

/**
 * @brief Уменьшает действительную часть на один
 */
Complex& Complex::operator--()
{
    changeRealPart(-1.0);
    return *this;
}

/**
 * @brief Уменьшает действительную часть и возвращает старое число
 */
Complex Complex::operator--(int)
{
    Complex oldValue(*this);
    --(*this);
    return oldValue;
}

/**
 * @brief Возвращает действительную часть
 *
 * @return Действительная часть числа
 */
double Complex::getRealPart() const
{
    return realPart;
}

/**
 * @brief Возвращает мнимую часть
 *
 * @return Мнимая часть числа
 */
double Complex::getImaginaryPart() const
{
    return imaginaryPart;
}

/**
 * @brief Возвращает модуль комплексного числа
 *
 * @return Модуль числа
 */
double Complex::getModulus() const
{
    return std::hypot(realPart, imaginaryPart);
}

/**
 * @brief Возвращает количество существующих чисел
 *
 * @return Количество объектов Complex
 */
int Complex::getObjectCount()
{
    return objectCount;
}

/**
 * @brief Выводит комплексное число на экран
 */
void Complex::printInfo() const
{
    std::cout << realPart;

    if (imaginaryPart < 0.0)
    {
        std::cout << " - " << -imaginaryPart << "i\n";
    }
    else
    {
        std::cout << " + " << imaginaryPart << "i\n";
    }
}
