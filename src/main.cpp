#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <windows.h>
#include "Complex.h"

/**
 * @brief Выводит результат проверки
 *
 * @param condition Результат проверяемого условия
 * @param message Название проверки
 * @param errors Количество ошибок
 */
void check(bool condition, const std::string& message, int& errors)
{
    std::cout << (condition ? "[OK] " : "[Ошибка] ") << message << '\n';

    if (!condition)
    {
        ++errors;
    }
}

/**
 * @brief Проверяет отказ при неправильных частях числа
 *
 * @param realPart Действительная часть
 * @param imaginaryPart Мнимая часть
 * @return true если объект не создан и счетчик сохранен
 */
bool rejectsNumber(double realPart, double imaginaryPart)
{
    int count = Complex::getObjectCount();

    try
    {
        Complex wrong(realPart, imaginaryPart);
    }
    catch (const std::invalid_argument&)
    {
        return Complex::getObjectCount() == count;
    }

    return false;
}

/**
 * @brief Проверяет конструкторы и время жизни комплексных чисел
 *
 * @return 0 если все проверки прошли, иначе 1
 */
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int errors{};
    std::cout << "Лабораторная работа №3. Вариант 4\n\n";
    check(Complex::getObjectCount() == 0, "До создания чисел счетчик равен 0", errors);

    {
        Complex first;
        Complex second(3.0, 4.0);
        Complex third(second);

        std::cout << "\nНачальные числа:\n";
        first.printInfo();
        second.printInfo();
        third.printInfo();
        std::cout << '\n';

        check(Complex::getObjectCount() == 3, "Созданы три числа разными конструкторами", errors);
        check(first.getRealPart() == 0.0 && first.getImaginaryPart() == 0.0,
              "Число без аргументов равно нулю", errors);
        check(second.getRealPart() == 3.0 && second.getImaginaryPart() == 4.0,
              "Параметризованный конструктор сохранил обе части", errors);
        check(third.getRealPart() == second.getRealPart() &&
              third.getImaginaryPart() == second.getImaginaryPart(),
              "Конструктор копирования сохранил данные", errors);

        const Complex& constNumber = second;
        check(constNumber.getModulus() == 5.0, "Модуль числа 3 + 4i равен 5", errors);
        check(first.getModulus() == 0.0, "Модуль нулевого числа равен 0", errors);

        {
            Complex negative(-3.0, -4.0);
            std::cout << "Число с отрицательными частями: ";
            negative.printInfo();
            check(negative.getModulus() == 5.0, "Модуль не зависит от знаков частей", errors);
            check(Complex::getObjectCount() == 4, "Внутри блока существуют четыре числа", errors);

            third = negative;
            check(third.getRealPart() == -3.0 && third.getImaginaryPart() == -4.0,
                  "Присваивание скопировало данные", errors);
            check(Complex::getObjectCount() == 4, "Присваивание не создало новый объект", errors);
            check(second.getRealPart() == 3.0 && second.getImaginaryPart() == 4.0,
                  "Изменение копии не изменило оригинал", errors);
        }

        check(Complex::getObjectCount() == 3, "После выхода из внутреннего блока осталось три числа", errors);

        std::cout << "\nНеправильные данные:\n";
        double infinity = std::numeric_limits<double>::infinity();
        double notNumber = std::numeric_limits<double>::quiet_NaN();
        double maximum = std::numeric_limits<double>::max();
        check(rejectsNumber(infinity, 0.0), "Бесконечная действительная часть отклонена", errors);
        check(rejectsNumber(0.0, -infinity), "Бесконечная мнимая часть отклонена", errors);
        check(rejectsNumber(notNumber, 0.0), "NaN в действительной части отклонен", errors);
        check(rejectsNumber(0.0, notNumber), "NaN в мнимой части отклонен", errors);
        check(rejectsNumber(maximum, maximum), "Переполнение модуля отклонено", errors);
        check(Complex::getObjectCount() == 3, "Неудачные конструкторы не изменили счетчик", errors);

        {
            Complex large(maximum, 0.0);
            check(std::isfinite(large.getModulus()) && large.getModulus() == maximum,
                  "Максимальное конечное число с нулевой мнимой частью допустимо", errors);
        }
    }

    check(Complex::getObjectCount() == 0, "Деструкторы уменьшили счетчик до 0", errors);
    std::cout << "\nКоличество ошибок: " << errors << '\n';
    std::cout << (errors == 0 ? "Все проверки выполнены\n" : "Есть ошибки проверки\n");
    return errors == 0 ? 0 : 1;
}
