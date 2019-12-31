/*
 * В этой задаче разрешается подключать <iostream>, <vector> и <string> и только их.
 *
 * Напишите класс BigInteger для работы с длинными целыми числами. Должны поддерживаться операции:
 * сложение, вычитание, умножение, деление, остаток по модулю, работающие так же, как и для int;
 * составное присваивание с этими операциями;
 * унарный минус, префиксный и постфиксный инкремент и декремент;
 * операторы сравнения == != < > <= >=;
 * вывод в поток и ввод из потока (возможен ввод из пустого потока);
 * метод toString(), возвращающий строковое представление числа;
 * конструирование из int (в том числе неявное преобразование, когда это надо);
 * неявное преобразование в bool, когда это надо (должно работать в условных выражениях).
 *
 * Асимптотические ограничения на время работы операторов в контесте не проверяются, но реализация должна работать за:
 * сложение, вычитание, унарный минус, операторы сравнения – O(n);
 * умножение, деление, остаток по модулю – o(n^1.6), где n – количество разрядов большего числа (по модулю).
 *
 * В вашем файле должна отсутствовать функция main(), а сам файл должен называться biginteger.h.
 * В качестве компилятора необходимо указывать Make.
 * Ваш код будет вставлен посредством команды #include<biginteger.h> в программу, содержащую тесты;
 * вследствие этого, код необходимо отправлять в файле со строго соответствующим именем!
 */

#include <iostream>
#include <vector>
#include <string>
#include "biginteger.h"



int main() {
    // Creation tests.
    BigInteger big_int1;
    std::cin >> big_int1;
    BigInteger big_int2(1000000);

    BigInteger big_int3(big_int1);
    BigInteger big_int4(std::move(big_int2));
    std::cout << "After initializing: " << big_int3 << ' ' << big_int4 << '\n';

    // Arithmetic unary operations tests.
    /*std::cout << "Arithmetic unary operations tests.\n";
    std::cout << "-: " << -big_int3 << '\n';
    std::cout << "a++: " << big_int3++ << ' ' << big_int3 << '\n';
    std::cout << "a--: " << big_int3-- << ' ' << big_int3 << '\n';
    std::cout << "++a: " << ++big_int3 << '\n';
    std::cout << "--a: " << --big_int3 << '\n';

    // Arithmetic binary operations tests.
    std::cout << "Arithmetic binary operations tests.\n";
    big_int3 = big_int1;
    big_int4 = 10;
    big_int3 /= big_int4;
    std::cout << "/= 10: " << big_int3 << '\n';
    big_int3 += big_int4;
    std::cout << "+= 10: " << big_int3 << '\n';
    big_int3 *= big_int4;
    std::cout << "*= 10: " << big_int3 << '\n';
    big_int3 -= big_int4;
    std::cout << "-= 10: "  << big_int3 << '\n';
    big_int3 %= big_int4;
    std::cout << "%= 10: "  << big_int3 << '\n';*/

    std::cin >> big_int3 >> big_int4;
    std::cout << big_int3.toString() << '\n';
    std::cout << "a / b: " << big_int3 / big_int4 << '\n';
    std::cout << "a + b: " << big_int3 + big_int4 << '\n';
    std::cout << "a * b: " << big_int3 * big_int4 << '\n';
    std::cout << "a - b: " << big_int3 - big_int4 << '\n';
    std::cout << "a % b: " << big_int3 % big_int4 << '\n';

    // Logical binary operators and bool conversation tests.
    std::cout << "Logical binary operators and bool conversation tests.\n";
    std::cin >> big_int3 >> big_int4;
    std::cout << "For numbers: " << big_int3 << " and " << big_int4 << ":\n";
    std::cout << "<: " << (big_int3 < big_int4) << '\n';
    std::cout << ">: " << (big_int3 > big_int4) << '\n';
    std::cout << "<=: " << (big_int3 <= big_int4) << '\n';
    std::cout << ">=: " << (big_int3 >= big_int4) << '\n';
    std::cout << "==: " << (big_int3 == big_int4) << '\n';
    std::cout << "!=: " << (big_int3 != big_int4) << '\n';
    if (big_int3) std::cout << "!= 0\n";
    if (!big_int3) std::cout << "== 0\n";

    return 0;
}