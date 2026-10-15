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
 * @brief Проверяет отказ унарной операции без изменения числа
 *
 * @param number Изменяемое число
 * @param operation Номер операции: 0 — ++a, 1 — a++, 2 — --a, 3 — a--
 * @return true если операция отклонена и состояние сохранено
 */
bool rejectsStep(Complex& number, int operation)
{
    double oldReal = number.getRealPart();
    double oldImaginary = number.getImaginaryPart();
    int count = Complex::getObjectCount();

    try
    {
        switch (operation)
        {
            case 0: ++number; break;
            case 1: number++; break;
            case 2: --number; break;
            case 3: number--; break;
            default: return false;
        }
    }
    catch (const std::overflow_error&)
    {
        return number.getRealPart() == oldReal &&
               number.getImaginaryPart() == oldImaginary &&
               Complex::getObjectCount() == count;
    }

    return false;
}

/**
 * @brief Проверяет унарные операции комплексного числа
 *
 * @param errors Количество ошибок
 */
void testUnaryOperations(int& errors)
{
    std::cout << "\nУнарные операции:\n";
    int initialCount = Complex::getObjectCount();

    {
        Complex number(3.0, 4.0);
        Complex& afterIncrement = ++number;
        check(&afterIncrement == &number && number.getRealPart() == 4.0 &&
              number.getImaginaryPart() == 4.0,
              "Префикс ++ изменил число и вернул ссылку на него", errors);
        check(Complex::getObjectCount() == initialCount + 1,
              "Префикс ++ не создал копию", errors);

        Complex beforeIncrement = number++;
        check(beforeIncrement.getRealPart() == 4.0 && beforeIncrement.getImaginaryPart() == 4.0 &&
              number.getRealPart() == 5.0 && number.getImaginaryPart() == 4.0,
              "Постфикс ++ вернул старое значение и увеличил число", errors);
        check(Complex::getObjectCount() == initialCount + 2,
              "Сохраненный результат постфикса считается отдельным объектом", errors);
        ++beforeIncrement;
        ++beforeIncrement;
        check(number.getRealPart() == 5.0 && beforeIncrement.getRealPart() == 6.0,
              "Изменение возвращенной копии не изменило исходное число", errors);

        Complex& afterDecrement = --number;
        check(&afterDecrement == &number && number.getRealPart() == 4.0 &&
              number.getImaginaryPart() == 4.0,
              "Префикс -- изменил число и вернул ссылку на него", errors);

        Complex beforeDecrement = number--;
        check(beforeDecrement.getRealPart() == 4.0 && beforeDecrement.getImaginaryPart() == 4.0 &&
              number.getRealPart() == 3.0 && number.getImaginaryPart() == 4.0,
              "Постфикс -- вернул старое значение и уменьшил число", errors);
        check(Complex::getObjectCount() == initialCount + 3,
              "Временные копии постфиксных операций уничтожены", errors);

        std::cout << "После ++a, a++, --a, a--: ";
        number.printInfo();
        ++(++number);
        check(number.getRealPart() == 5.0 && number.getImaginaryPart() == 4.0,
              "Префиксные операции можно выполнять последовательно", errors);
        --(--number);
        check(number.getRealPart() == 3.0 && number.getImaginaryPart() == 4.0,
              "Два уменьшения вернули исходное значение", errors);

        int beforeDiscardedResult = Complex::getObjectCount();
        number++;
        number--;
        check(Complex::getObjectCount() == beforeDiscardedResult && number.getRealPart() == 3.0,
              "Несохраненные результаты постфикса уничтожены", errors);
    }

    check(Complex::getObjectCount() == initialCount,
          "После унарных операций счетчик вернулся к исходному значению", errors);

    {
        Complex number(-1.0, -2.0);
        ++number;
        check(number.getRealPart() == 0.0 && number.getImaginaryPart() == -2.0,
              "Инкремент переводит действительную часть через ноль", errors);
        --number;
        check(number.getRealPart() == -1.0 && number.getImaginaryPart() == -2.0,
              "Декремент работает с отрицательными частями", errors);
    }

    std::cout << "\nНепредставимые изменения:\n";
    {
        Complex large(std::numeric_limits<double>::max(), 0.0);
        Complex negativeLarge(-std::numeric_limits<double>::max(), 0.0);

        for (int operation = 0; operation < 4; ++operation)
        {
            check(rejectsStep(large, operation),
                  "Операция с большим числом отклонена, данные и счетчик сохранены", errors);
            check(rejectsStep(negativeLarge, operation),
                  "Операция с большим отрицательным числом отклонена", errors);
        }
    }

    check(Complex::getObjectCount() == initialCount,
          "Ошибки унарных операций не оставили лишних объектов", errors);
}

/**
 * @brief Проверяет результат с учетом погрешности вычислений
 *
 * @param number Проверяемое число
 * @param realPart Ожидаемая действительная часть
 * @param imaginaryPart Ожидаемая мнимая часть
 * @return true если части совпадают с заданной точностью
 */
bool hasParts(const Complex& number, double realPart, double imaginaryPart)
{
    return std::abs(number.getRealPart() - realPart) < 0.000001 &&
           std::abs(number.getImaginaryPart() - imaginaryPart) < 0.000001;
}

/**
 * @brief Проверяет сохранение состояния при ошибке арифметики
 *
 * @param number Изменяемое число
 * @param other Второй операнд
 * @param operation Номер операции: 0 — +=, 1 — -=, 2 — *=, 3 — /=
 * @param divisionByZero Ожидается ли ошибка деления на ноль
 * @return true если получена нужная ошибка и данные сохранены
 */
bool rejectsArithmetic(Complex& number, const Complex& other,
                       int operation, bool divisionByZero = false)
{
    double oldReal = number.getRealPart();
    double oldImaginary = number.getImaginaryPart();
    double otherReal = other.getRealPart();
    double otherImaginary = other.getImaginaryPart();
    int count = Complex::getObjectCount();
    bool rejected = false;

    try
    {
        switch (operation)
        {
            case 0: number += other; break;
            case 1: number -= other; break;
            case 2: number *= other; break;
            case 3: number /= other; break;
            default: return false;
        }
    }
    catch (const std::domain_error&)
    {
        rejected = divisionByZero;
    }
    catch (const std::overflow_error&)
    {
        rejected = !divisionByZero;
    }

    return rejected && number.getRealPart() == oldReal &&
           number.getImaginaryPart() == oldImaginary &&
           other.getRealPart() == otherReal && other.getImaginaryPart() == otherImaginary &&
           Complex::getObjectCount() == count;
}

/**
 * @brief Проверяет арифметические присваивания
 *
 * @param errors Количество ошибок
 */
void testArithmeticAssignment(int& errors)
{
    std::cout << "\nАрифметические присваивания:\n";
    int initialCount = Complex::getObjectCount();

    {
        Complex number(3.0, 4.0);
        const Complex other(1.0, -2.0);
        int count = Complex::getObjectCount();

        check(&(number += other) == &number && hasParts(number, 4.0, 2.0),
              "+= сложило части и вернуло ссылку на число", errors);
        check(&(number -= other) == &number && hasParts(number, 3.0, 4.0),
              "-= вычло части и вернуло ссылку на число", errors);
        check(&(number *= other) == &number && hasParts(number, 11.0, -2.0),
              "*= выполнило комплексное умножение", errors);
        check(&(number /= other) == &number && hasParts(number, 3.0, 4.0),
              "/= выполнило комплексное деление", errors);
        check(hasParts(other, 1.0, -2.0), "Правый операнд не изменился", errors);
        check(Complex::getObjectCount() == count,
              "Арифметические присваивания не создали новых объектов", errors);
        std::cout << "После +=, -=, *=, /=: ";
        number.printInfo();

        number += number;
        check(hasParts(number, 6.0, 8.0), "Число можно прибавить к самому себе", errors);
        number -= number;
        check(hasParts(number, 0.0, 0.0), "Вычитание самого себя дает ноль", errors);
        number = other;
        number *= number;
        check(hasParts(number, -3.0, -4.0), "Умножение самого на себя учитывает старые части", errors);
        number /= number;
        check(hasParts(number, 1.0, 0.0), "Ненулевое число делится само на себя", errors);

        Complex zero;
        number *= zero;
        check(hasParts(number, 0.0, 0.0), "Умножение на ноль дает ноль", errors);
        number /= other;
        check(hasParts(number, 0.0, 0.0), "Ноль делится на ненулевое число", errors);
        check(rejectsArithmetic(number, zero, 3, true),
              "Деление нуля на ноль отклонено", errors);
        number = other;
        check(rejectsArithmetic(number, zero, 3, true),
              "Деление ненулевого числа на ноль отклонено", errors);
        check(rejectsArithmetic(zero, zero, 3, true),
              "Деление нулевого числа на себя не меняет данные", errors);

        Complex imaginary(0.0, 2.0);
        imaginary *= imaginary;
        check(hasParts(imaginary, -4.0, 0.0), "Квадрат мнимого числа действительный", errors);
        number = other;
        (number += other) -= other;
        check(hasParts(number, 1.0, -2.0), "Присваивания можно объединять в выражение", errors);

        double maximum = std::numeric_limits<double>::max();
        Complex large(maximum, 0.0);
        Complex negativeLarge(-maximum, 0.0);
        Complex twice(2.0, 0.0);
        Complex tiny(std::numeric_limits<double>::denorm_min(), 0.0);
        check(rejectsArithmetic(large, large, 0), "Переполнение += не изменило число", errors);
        check(rejectsArithmetic(large, negativeLarge, 1), "Переполнение -= не изменило число", errors);
        check(rejectsArithmetic(large, twice, 2), "Переполнение *= не изменило число", errors);
        check(rejectsArithmetic(large, tiny, 3), "Переполнение /= не изменило число", errors);

        Complex diagonal(maximum * 0.6, 0.0);
        const Complex vertical(0.0, maximum * 0.9);
        check(rejectsArithmetic(diagonal, vertical, 0),
              "Переполнение модуля отклонено, даже если части конечные", errors);

        large /= large;
        check(hasParts(large, 1.0, 0.0), "Большое число делится само на себя без переполнения квадратов", errors);
        tiny /= tiny;
        check(hasParts(tiny, 1.0, 0.0), "Очень маленькое число делится само на себя", errors);
    }

    check(Complex::getObjectCount() == initialCount,
          "После арифметических проверок счетчик восстановлен", errors);
}

/**
 * @brief Проверяет конструкторы, операции и время жизни чисел
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

    testUnaryOperations(errors);
    testArithmeticAssignment(errors);
    check(Complex::getObjectCount() == 0, "Деструкторы уменьшили счетчик до 0", errors);
    std::cout << "\nКоличество ошибок: " << errors << '\n';
    std::cout << (errors == 0 ? "Все проверки выполнены\n" : "Есть ошибки проверки\n");
    return errors == 0 ? 0 : 1;
}
