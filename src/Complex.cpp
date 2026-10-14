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
