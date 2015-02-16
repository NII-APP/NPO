#ifndef CEXPRESSION_H
#define CEXPRESSION_H

#include <cmath>
#include <string>
#include <cstring>
#include <cstdlib>
#include <limits>

class CExpression
{
    struct sintElem
    {
    /*
     Основная структура данных. Если лексема нулевая, есть число. Иначе даболавская константа
     лежит мёртвым грузом и неинициализирована а смысл имеет только чаровская ( в соновном,
     обозначает операцию).
    */
        char lexeme;
        double number;
    };

    bool fine;//является ли данное выражение нормальным
    bool test;//Нужно ли тестировать выражение на правильность при перестроении
    bool expForm;//Если 1 то распознаёт числа в экеспоненциальном виде. иначе
            //представляет их в виде произведения/суммы трёх чисел. По умолчанию 1

    sintElem* walkthroughArray;/*Массив в котором хранятся результаты предразбора строки.
                               Признаком конца такого массива является walkthroughArray[i].lexeme == 127*/

    //про mutable: http://alenacpp.blogspot.com/2005/10/mutable-constcast.html
    mutable double* numArray;//Буфер памяти под нужны calculate
    mutable sintElem* calculationIt;/*Даже итератор для перебора в calculation я объявляю заранее*/
//Всё. Больше в объекте ничего не хранится.


    //Функции конструктора. Подробные коментарии указаны в теле функций.
    /*Функции хранящиеся в файле ArifmometrSortSigment.
    Комплекс функций необходим для сортировки в польскую нотацию*/
    void      sortPoland    (sintElem*              );
    void      recursionMain (sintElem*,short&,short&);
    void      recursionPlus (sintElem*,short&,short&);
    void      recursionMult (sintElem*,short&,short&);
    void      recursionPow  (sintElem*,short&,short&);
    void      recursionUnary(sintElem*,short&,short&);
    /*Функции хранящиеся в файле ArifmometrPreparationSigment.
    Комплекс функция необходимых для полготовки строки к сортировке в польскую нотацию*/
    void      sintAdaptation   (char*                  );
    double    setPoint         (double, short          );
    static char* addMult       (char*                  );
    sintElem* setArray         (char*                  );
    static void addCode        (char*                  );
    void toExpForm             (char*                  );

//Функции для проверки строки как выражения и для функции оптимизации.
    static inline bool isNumber(char);//0-9/./x/y
    static inline bool isFigure(char);//0-9/.
    static inline bool isLetter(char);//строчная латинская буква-ли?
    static inline bool isUnary(char);//Арность операции. 1 если унарная, 0 если бинарная.
    static inline bool isOperation(char);//если не 0 и не 'x' и не 'y'
    sintElem doIt(char,sintElem);//Выполняет операцию char над операндом
    sintElem doIt(char,sintElem,sintElem);//ами
    unsigned operatorCount(sintElem*);//
    void delElements(sintElem*, unsigned);

public:
/*
Все функции которым нечего возвращать возвращают ссылку на объект
для которого они вызваны.
*/
    CExpression();//Назначает выражение неверным, проверку ставит в true
    inline CExpression(const CExpression& exp);//Вызывает operator =
    inline CExpression(char const* exp, bool isTest = true);//Вызывает rebild
    inline CExpression(const std::string& exp, bool isTtest = true);//Вызывает rebild
    ~CExpression(void);
    CExpression& operator = (const CExpression& exp);
    inline CExpression& rebild(const std::string& st);//Просто с конвертацией
    CExpression& rebild(const char* exp);//Полное удаление старого выражения, подготовка нового их строки exp.
    double calculate(double x = 0., double y = 0.) const;
    //Сразу вычисляет строку. ТОЛЬКО ДЛЯ ОДНОРАЗОВОЙ ЭКСПЛУАТАЦИИ!
    static double calculate(const char* st, double x = 0., double y = 0.);
    static double calculate(const std::string& st, double x = 0., double y = 0.);

    static inline bool isExp(const std::string&);
    static bool isExp(const char*);


    CExpression& setTest(bool t) { test = t; return *this; }
    CExpression& setExpForm(bool t) { expForm = t; return *this; }
    bool isExpFor() const { return expForm; }
    bool isTest() const { return test; }
    bool isFine() const { return fine; }
    operator bool () const { return fine; }

    CExpression& optimization ();
    bool isConst();
    bool isConstFast() const;
};


#endif // CEXPRESSION_H
