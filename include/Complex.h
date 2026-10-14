#ifndef COMPLEX_H
#define COMPLEX_H

/**
 * @brief Класс для хранения комплексного числа
 */
class Complex
{
private:
    double realPart;
    double imaginaryPart;
    static int objectCount;

    /**
     * @brief Изменяет действительную часть, проверяя результат
     *
     * @param change Изменение действительной части
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    void changeRealPart(double change);

public:
    /**
     * @brief Создает комплексное число с нулевыми частями
     */
    Complex();

    /**
     * @brief Создает комплексное число с заданными частями
     *
     * @param realPart Действительная часть
     * @param imaginaryPart Мнимая часть
     * @throws std::invalid_argument Если части или модуль не являются конечными
     */
    Complex(double realPart, double imaginaryPart);

    /**
     * @brief Создает копию комплексного числа
     *
     * @param other Число для копирования
     */
    Complex(const Complex& other);

    /**
     * @brief Копирует данные в существующее число
     *
     * @param other Число для копирования
     * @return Ссылка на измененный объект
     */
    Complex& operator=(const Complex& other) = default;

    /**
     * @brief Завершает время жизни числа и уменьшает счетчик
     */
    ~Complex();

    /**
     * @brief Увеличивает действительную часть на один
     *
     * @return Ссылка на измененное число
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex& operator++();

    /**
     * @brief Увеличивает действительную часть и возвращает старое число
     *
     * @param unused Признак постфиксной формы, значение не используется
     * @return Копия числа до увеличения
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex operator++(int unused);

    /**
     * @brief Уменьшает действительную часть на один
     *
     * @return Ссылка на измененное число
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex& operator--();

    /**
     * @brief Уменьшает действительную часть и возвращает старое число
     *
     * @param unused Признак постфиксной формы, значение не используется
     * @return Копия числа до уменьшения
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex operator--(int unused);

    /**
     * @brief Возвращает действительную часть
     *
     * @return Действительная часть числа
     */
    double getRealPart() const;

    /**
     * @brief Возвращает мнимую часть
     *
     * @return Мнимая часть числа
     */
    double getImaginaryPart() const;

    /**
     * @brief Возвращает модуль комплексного числа
     *
     * @return Модуль числа
     */
    double getModulus() const;

    /**
     * @brief Возвращает количество существующих чисел
     *
     * @return Количество объектов Complex
     */
    static int getObjectCount();

    /**
     * @brief Выводит комплексное число на экран
     */
    void printInfo() const;
};

#endif
