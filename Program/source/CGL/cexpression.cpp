#include "cexpression.h"
#include <iostream>
#define qDebug() std::cout

CExpression::CExpression()
    : fine(false) , test(true), expForm(true) , walkthroughArray(0) , numArray(0) {   }
CExpression::CExpression(const char* CExpression, bool isTest)
    : fine(true), test(isTest), expForm(true), walkthroughArray(0), numArray(0) { rebild(CExpression); }
CExpression::CExpression(const std::string& CExpression, bool isTest)
    : fine(true), test(isTest), expForm(true), walkthroughArray(0), numArray(0) { rebild(CExpression.c_str()); }
CExpression::CExpression(const CExpression& copy)
    : walkthroughArray(0) , numArray(0) { this->operator =(copy); }
CExpression& CExpression::operator = (const CExpression& copy)
{
    test = copy.test;
    fine = copy.fine;

    if (!fine)
    {
        delete[] walkthroughArray;
        delete[] numArray;
        return *this;
    }

    size_t sizeOfArr = 0;
    while(copy.walkthroughArray[sizeOfArr].lexeme != 127) {
        ++sizeOfArr;
    }//size of array was be estimated
    walkthroughArray = new sintElem [sizeOfArr + 1];//
    for(size_t i = 0; i != sizeOfArr; ++i)
    {
        walkthroughArray[i] = copy.walkthroughArray[i];
    }
    walkthroughArray[sizeOfArr].lexeme = 127;//copy them

    size_t i = 0;
    size_t numbersCount = 0;
    while(walkthroughArray[i].lexeme != 127)
    {
        if(walkthroughArray[i].lexeme == 'x' ||
           walkthroughArray[i].lexeme == 'y' ||
          !walkthroughArray[i].lexeme)
        {
            ++numbersCount;
        }
        ++i;
    }//size of bufer for calculate was estimated
    numArray = new double[numbersCount + 1];//allocate bufer
    return *this;
}

CExpression::~CExpression(void)
{
    delete[] walkthroughArray;
    delete[] numArray;
}
double CExpression::calculate(double x, double y) const
{
    if (!fine)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    calculationIt = walkthroughArray;
    while ((*calculationIt).lexeme != 127)
    {
        /*
        if ((*calculationIt).lexeme)
            qDebug() << (*calculationIt).lexeme << '(' << (int)calculationIt->lexeme << ')';
        else qDebug() << (*calculationIt).number;//*/
        switch ((*calculationIt).lexeme)
        {
            case 0:
                *++numArray = calculationIt->number;
                break;
            case '+':
                --numArray;
                *numArray += numArray[1];
                break;
            case '*':
                --numArray;
                *numArray *= numArray[1];
                break;
            case '-':
                --numArray;
                *numArray -= numArray[1];
                break;
            case '%':
                *numArray *= -1;
                break;
            case '/':
                --numArray;
                *numArray /= numArray[1];
                break;
            case '^':case -121:
                --numArray;
                *numArray = pow(*numArray,numArray[1]);
                break;
            case 'x':
                *++numArray = x;
                break;
            case 'y':
                *++numArray = y;
                break;
            case -128:
                *numArray = sin(*numArray);
                break;
            case -127:
                *numArray = cos(*numArray);
                break;
            case -126:
                *numArray = tan(*numArray);
                break;
            case -125:
                *numArray = 1 / tan(*numArray);
                break;
            case -124:
                --numArray;
                *numArray = log(*numArray)/log(numArray[1]);
                break;
            case -123:
                *numArray = log10(*numArray);
                break;
            case -122:
                *numArray = log(*numArray);
                break;
            case -120:
                *numArray = pow(*numArray,0.5);
                break;
            case -119:
                *numArray *= *numArray;
                break;
            case -118:
                *numArray = fabs(*numArray);
                break;
            case -117:
                *numArray = asin(*numArray);
                break;
            case -116:
                *numArray = acos(*numArray);
                break;
            case -115:
                *numArray = atan(*numArray);
                break;
            case -114:
                *numArray = atan(1 / *numArray);
                break;
            case -113:
                *numArray = sinh(*numArray);
                break;
            case -112:
                *numArray = cosh(*numArray);
                break;
        }
        ++calculationIt;
    }
    --numArray;
    return numArray[1];
}
double CExpression::calculate(const char* st, double x, double y) { return CExpression(st, true).calculate(x,y); }
double CExpression::calculate(const std::string& st, double x, double y) { return CExpression(st, true).calculate(x,y); }

/*inline*/ CExpression& CExpression::rebild(const std::string& st) { return rebild(st.c_str()); }
CExpression& CExpression::rebild(const char* CExpression)
{
    delete[] walkthroughArray;
    delete[] numArray;
    walkthroughArray = 0;
    numArray = 0;
    fine = true;
    if (!isExp(CExpression))
    {
        fine = false;
        return *this;
    }
    int len = static_cast<int>(strlen(CExpression));
    char* sourseString = new char[len * 3 + 1];
    for (int i = 0, j = 0; i < len; ++i)
        sourseString[j++] = CExpression[i];
    sourseString[len] = 0;

    sintAdaptation(sourseString);//Делает строку регистронезависимой, удаляет пробелы/'\t'/'\n', приводит скобочки к однообразию
    addCode(sourseString);//Заменяет ссылки на функции символами от -128 для удобства дальнейшего анализа
    std::cout << sourseString << std::endl;
    for (int i(0); sourseString[i]; ++i) {
        if (sourseString[i] == 't')
            sourseString[i] = 'y';
    }
    std::cout << sourseString << std::endl;
    if (expForm) {
        toExpForm(sourseString);//Преобразовывает записи типа -3.0e-2 так,
    }//чтобы они вычислялись как экспоненциальная форма числа.
    addMult(sourseString);//Добавляет * и 0 для реализации унарного минуса. Функции объеденены по историческим причинам.
    sortPoland(setArray(sourseString));/*При вызове преобразует строку в массив sintElem и высылает его для
                                       сортировки в польскую нотацию*/

    short i = 0,
          numbersCount = 0,
          opCount = 1;
    while (walkthroughArray[i].lexeme != 127)
    {
        if (walkthroughArray[i].lexeme == 'x' ||
            walkthroughArray[i].lexeme == 'y' ||
           !walkthroughArray[i].lexeme) {
            ++numbersCount;//Посчитать максимально возможное количество чисел в стеке(тоесть все числа вобще)
        } else {
            opCount += !isUnary(walkthroughArray[i].lexeme);
        }
        ++i;
    }
    //qDebug() << opCount << '<' << numbersCount;
    if (opCount > numbersCount)
    {
        delete[] walkthroughArray;
        delete[] numArray;
        walkthroughArray = 0;
        numArray = 0;
        fine = false;
        return *this;
    }

    numArray = new double[numbersCount + 1];//1 лишний элемент нужен для оптимизации алгаритма вычисления
    //std::cout << "объект создан.\n";
    return *this;
}

short strlen(char* st){
/*Так как функция из библиотеки выдаёт ворнинг я её переписал*/
    char* len = st;
    while (*len) ++len;
    return static_cast<short>(len - st);
}
void CExpression::sintAdaptation (char* st)
{
    int i = -1, j = 0;
    while (st[++i])//Регистронезависимость и приведение скобочек к единообразию
        if(st[i] >= 'A' && st[i] <= 'Z')
            st[i] -= 'A' - 'a';
        else if(st[i] == '{' || st[i] == '[')
            st[i] = '(';
        else if(st[i] == '}' || st[i] == ']')
            st[i] = ')';
    i = 0;
    while (st[i - 1])//Убираю пробелы, символы табуляции и переноса строки
    {
        while(st[i] && ( st[i] == ' ' || st[i] == '\n' || st[i] == '\t'))
            ++i;
        st[j] = st[i];
        ++j;
        ++i;
    }
    i = j = 0;
    while (st[i - 1])//Убираю унарные плюсы
    {
        while(st[i] && st[i] == '+' && ( !i || (st[i - 1] != ')' && !isNumber(st[i - 1])) ) )
            ++i;
        st[j] = st[i];
        ++j;
        ++i;
    }
}
void CExpression::addCode (char* st)
{
    int j(0),i=0;
    while(st[i+3])
    {
        if(st[i]=='s'&&st[i+1]=='i'&&st[i+2]=='n')
        {
            st[i]=-128;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='c'&&st[i+1]=='o'&&st[i+2]=='s')
        {
            st[i] = -127;
            j = i + 1;
            while(st[j + 1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='t'&&st[i+1]=='g')
        {
            st[i] = -126;
            j = i + 1;
            while(st[j]) {
                st[j]=st[j+1];
                ++j;
            }
        }
        else if(st[i]=='c'&&st[i+1]=='t'&&st[i+2]=='g')
        {
            st[i]=-125;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='l'&&st[i+1]=='o'&&st[i+2]=='g')
        {
            st[i]=-124;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='l'&&st[i+1]=='g')
        {
            st[i]=-123;
            j=i+1;
            while(st[j]) {
                st[j]=st[j+1];
                ++j;
            }
        }
        else if(st[i]=='l'&&st[i+1]=='n')
        {
            st[i]=-122;
            j=i+1;
            while(st[j]) {
                st[j]=st[j+1];
                ++j;
            }
        }
        else if(st[i]=='p'&&st[i+1]=='o'&&st[i+2]=='w')
        {
            st[i]=-121;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='s'&&st[i+1]=='q'&&st[i+2]=='r'&&st[i+3]=='t')
        {
            st[i]=-120;
            j=i+1;
            while(st[j+2]) {
                st[j] = st[j + 3];
                ++j;
            }
        }
        else if(st[i]=='s'&&st[i+1]=='q'&&st[i+2]=='r')
        {
            st[i]=-119;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='a'&&st[i+1]=='b'&&st[i+2]=='s')
        {
            st[i]=-118;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i+4] && st[i+5]) {
            if(st[i]=='a'&&st[i+1]=='r'&&st[i+2]=='c'&&st[i+3]=='s'&&st[i+4]=='i'&&st[i+5]=='n'){
                st[i]=-117;
                j=i+1;
                while(st[j+4]) {
                    st[j] = st[j + 5];
                    ++j;
                }
            }
            else if(st[i]=='a'&&st[i+1]=='r'&&st[i+2]=='c'&&st[i+3]=='c'&&st[i+4]=='o'&&st[i+5]=='s'){
                st[i]=-116;
                j=i+1;
                while(st[j+4]) {
                    st[j] = st[j + 5];
                    ++j;
                }
            }
            else if(st[i]=='a'&&st[i+1]=='r'&&st[i+2]=='c'&&st[i+3]=='t'&&st[i+4]=='g'){
                st[i]=-115;
                j=i+1;
                while(st[j+3]) {
                    st[j] = st[j + 4];
                    ++j;
                }
            }
            else if(st[i]=='a'&&st[i+1]=='r'&&st[i+2]=='c'&&st[i+3]=='c'&&st[i+4]=='t'&&st[i+5]=='g'){
                st[i]=-114;
                j=i+1;
                while(st[j+4]) {
                    st[j] = st[j + 5];
                    ++j;
                }
            }
            else if(st[i]=='s'&&st[i+1]=='i'&&st[i+2]=='n'&&st[i+3]=='h')
            {
                st[i]=-113;
                j=i+1;
                while(st[j+2]) {
                    st[j] = st[j + 3];
                    ++j;
                }
            }
            else if(st[i]=='c'&&st[i+1]=='o'&&st[i+2]=='s'&&st[i+3]=='h')
            {
                st[i]=-112;
                j=i+1;
                while(st[j+2]) {
                    st[j] = st[j + 3];
                    ++j;
                }
            }
        }
        i++;
    }
}
char* CExpression::addMult (char* st)
/*Добавляет в анализируемую строку *. Ввиду сложности условвие срабатывания расписано в несколько строк
    Функция не предназначена для полного решения вопроса а только помогает упростить ввод данных*/
{
    int strLen = strlen(st),//Хрнаит длину строки изменяемую функцией!
        i = 0,                    //Глобальный бегунок
        j;             //Бегунок используемый при добавлении '*'!

    while (st[i]){
        if (
            (
                ((st[i]>47 && st[i]<58) || (st[i-1]=='p' && st[i]=='i') || st[i]=='e' || st[i]=='x' || st[i]=='y' || st[i]==')')
                &&
                (st[i+1]=='(' || st[i+1]<=-100 || st[i+1]=='e' || (st[i+1]=='p'&&st[i+2]=='i') || st[i+1] =='x' || st[i+1] == 'y')
            )/*Если справа число или переменная или ) а слева (, x, pi, e или функция*/
            ||
            (
                ((st[i-1]=='p' && st[i]=='i') || st[i]=='e' || st[i]=='x' || st[i]=='y' || st[i]==')')
                &&
                ( st[i+1]>='0' && st[i+1]<='9' )
            )
          )//Если нужно добавить *. Хоть и здоровенный но выносить в функцию не стал. Решил что не круто.
        {
            ++strLen;
            j=strLen;
            st[j]=0;
            while(i+1 != j)
            {
                --j;
                st[j+1] = st[j];
            }
            st[j]='*';
        }
        ++i;
    }
    //qDebug() << "theroes added $" << st << "$";
    return st;
}
void CExpression::toExpForm(char* st)
{
    int i = 0;
    int strLen = strlen(st);
    while (st[i])
    {
        if (st[i] == 'e')
        {
            int j = 1 + (st[i + 1] == '-' || st[i + 1] == '+');
            if (isFigure(st[i - 1]) && isFigure(st[i + j]))
            {
                while (i - j && isFigure(st[i - j]))
                {
                    ++j;
                }//Нашли начало числовой составляющей
                j += i - j == 0;

                int k = 1 + ( st[i + 1] == '+' || st[i + 1] == '-' );

                while (st[i + k] && isFigure(st[i + k]))
                {
                    ++k;
                }//Нашли конец экспоненциальной части

                //добавить в конец и в начало по скобочке, заменить е на *10^
                strLen += 5;
                int l = strLen;
                while(i + k + 4 != l)
                {
                    st[l] = st[l - 5];
                    --l;
                }
                st[l-4] = ')';
                while(i + 4 != l)
                {
                    st[l] = st[l - 4];
                    --l;
                }
                st[l] = '^';
                st[--l] = '0';
                st[--l] = '1';
                st[--l] = '*';
                --l;
                ++j;
                while(l && i - j + 2 != l)
                {
                    st[l] = st[l - 1];
                    --l;
                }
                st[l] = '(';
                i += 4;
            }
        }
        ++i;
    }
}
double CExpression::setPoint (double arg, short del)
{
    while(del--)
        arg /= 10;
    return arg;
}
CExpression::sintElem* CExpression::setArray (char* st)
/*
Преобразует строку в массив элементов следующего вида:
    Каждый элемент имеет переменную lexeme типа char хранящую все лексэмы,
    если же встречается число то оно записывается в переменную number типа double.
ВНИМАНИЕ! Признаком конца такого массива является выполнение равенства lexeme == 127

Воспринимает упоминания pi и e как чисел что делает невозможной экоспоненциальную форму записи числа в классическом виде.
    если необходимо записать число в экспоненциальной форме пользуйтесь функцией pow,
    анализатор оптимизирован для исполненя этой функции и упрощает алгоритм если
    необходимо возвести в целую степень!

В анализаторе не предусмотрены методы позволяющие использовать не десятиричную форму представления чисел.
*/
{
    int i=0;//Бегунок символьного массива
    //Обозначаем унарную операцию заменив '-' на '%'
    if(*st == '-')
        *st = '%';
    while(st[i++])
        if((st[i-1] == '(' && st[i]=='-' && st[i+1]!='(') || ((st[i-1] == '*' || st[i-1] == '/' || st[i-1] == '+' || st[i-1] == '-' || st[i-1] == '^') && st[i] == '-'))
            st[i] = '%';
    int elKol=0;//Хранит количество элементов после завершения первого корневого цикла while
    //Подсчитываем количество элементов чтобы выделить память. Выделяем память.
    i=0;
    while(st[i]){
        //if(st[i] == '%' && st[i+1] == 'p' && st[i+2] == 'i')
            //--elKol;//Если это вобще унарный минус перед пи
        while(((st[i] >= '0' && st[i] <= '9') || st[i] == '.' || st[i] == '%') && ((st[i+1] >= '0' && st[i+1] <= '9') || st[i+1] == '.' || st[i+1] == 'p' || st[i+1] == 'e'))
            ++i;//Если текущий и следующий символы - части числа
        if(st[i] == 'p' && st[i+1] == 'i')
            ++i;//Чтобы пи за 2 элемента не считать и включить унарный минус.
        ++elKol;
        ++i;
    }
    sintElem* firstAnalysisArray = new sintElem [elKol + 1];//Плюс место под признак конца масива
    //Цикл основного анализа
    i=0;
    int point = 0,        //Хранит количество знаков в числе после запятой.
        j = 0,            //Бегунок нового масива
        det = 1;          //Коэффициент учёта унарного минуса
    bool isFigure = false;//Признак указывающий на то что до этого было число
    while(st[i])
    {
        if(st[i]>='0'&&st[i]<='9')
            if(point)//Если это число после запятой
                firstAnalysisArray[j].number += setPoint(st[i] - '0',point++);
            else if(isFigure){//Если не первая цифра числа
                firstAnalysisArray[j].number *= 10;
                firstAnalysisArray[j].number += st[i] - '0';
            }
            else{//Если это первая цифра в числе
                firstAnalysisArray[j].lexeme = 0;//Фикссируем что это число
                firstAnalysisArray[j].number = st[i] - '0';
                isFigure = true;
                if(i && st[i-1] == '%')//Если унарная операция
                    det=-1;
            }
        else if(st[i]=='.')
        {
            point=1;
            if(!isFigure)
            {
                isFigure = true;
                firstAnalysisArray[j].lexeme = 0;//Фикссируем что это число
                firstAnalysisArray[j].number = 0.;
                if(i && st[i-1] == '%')//Если унарная операция
                    det=-1;
            }
        }
        else if(st[i]=='e'){
            if(i && st[i - 1] == '%')
                det = -1;
            firstAnalysisArray[j].lexeme = 0;
            firstAnalysisArray[j++].number = 2.718281828459045235360287471352662497757 * det;
            det = 1;
        }
        else if(st[i]=='p'&&st[i+1]=='i'){
            if(i && st[i - 1] == '%')
                det=-1;
            firstAnalysisArray[j].lexeme = 0;
            firstAnalysisArray[j++].number = 3.141592653589793238462643383279502884197 * det;
            det=1;//Занулить данные если раньше было число(рудиментарная форма защиты от ошибок)
            ++i;//отработано сразу два символа
        }
        else if(isFigure){
            firstAnalysisArray[j++].number *= det;
            firstAnalysisArray[j++].lexeme = st[i];
            det = 1;
            point = isFigure = 0;
        }
        else if(st[i] != '%' || (st[i + 1] != 'p' && st[i + 1] != 'e' && ( st[i + 1] < '0' || st[i + 1] > '9' ) && st[i + 1] != '.'))
        {
            firstAnalysisArray[j++].lexeme = st[i];
        }
        ++i;
    }
    if(isFigure)
        firstAnalysisArray[j].number *= det;
    delete[] st;
    firstAnalysisArray[elKol].lexeme = 127;//Записываем признак конца масcива
    //*
    qDebug() << "firstAnalysis: ";
    for(i = 0; firstAnalysisArray[i].lexeme != 127; ++i)
        if(firstAnalysisArray[i].lexeme)
            qDebug() << firstAnalysisArray[i].lexeme ;
        else
        {

            if(firstAnalysisArray[i].number == 2.718281828459045235360287471352662497757)
                qDebug() <<'e';
            else if(firstAnalysisArray[i].number == -2.718281828459045235360287471352662497757)
                qDebug() << "-e";
            else if(firstAnalysisArray[i].number == -3.141592653589793238462643383279502884197)
                qDebug() << "-pi";
            else if(firstAnalysisArray[i].number == 3.141592653589793238462643383279502884197)
                qDebug() << "pi";
            else
                qDebug() << 'n';
        }
    qDebug() << '\n';//*/
    return firstAnalysisArray;
}





/*



сортировка



*/



void CExpression::recursionPlus (CExpression::sintElem* sourseArray, short &i, short &j)
{
    /*В случае +- мы ищем следующий +- или конец масива.
    Рекурсия нужна чтобы всё остальное было полностью независимо*/
    while(sourseArray[i].lexeme != '+' && sourseArray[i].lexeme != '-' && sourseArray[i].lexeme != 127){
        if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
            walkthroughArray[j++] = sourseArray[i++];
        else if(sourseArray[i].lexeme == '+' || sourseArray[i].lexeme == '-' || sourseArray[i].lexeme == '*' || sourseArray[i].lexeme == '/' || sourseArray[i].lexeme == '^' || sourseArray[i].lexeme < -100 || sourseArray[i].lexeme == '%'){
            sintElem cnt = sourseArray[i];
            switch(cnt.lexeme)
            {
            case'*':case'/':
                recursionMult (sourseArray,++i,j);
                break;
            case'^':
                recursionPow  (sourseArray,++i,j);
                break;
            case'%':
                recursionUnary(sourseArray,++i,j);
                break;
            default:
                recursionMain (sourseArray,++(++i),j);
                while(sourseArray[i].lexeme == ',')
                    recursionMain(sourseArray,++i,j);
                ++i;
            }
            walkthroughArray[j++] = cnt;
        }
        else if(sourseArray[i].lexeme == '('){
            recursionMain(sourseArray,++i,j);
            ++i;
        }
        else if(sourseArray[i].lexeme == ')' || sourseArray[i].lexeme == ',')
            return;
    }
}
void CExpression::recursionMult (CExpression::sintElem* sourseArray, short &i, short &j)
{
    //В случае /* мы ищем следующий /*-+ или конец масива
    while(!sourseArray[i].lexeme || sourseArray[i].lexeme == '^' || sourseArray[i].lexeme == '%' || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y' || sourseArray[i].lexeme == '(' || sourseArray[i].lexeme < -100){
        if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
            walkthroughArray[j++] = sourseArray[i++];
        else if(sourseArray[i].lexeme == '^' || sourseArray[i].lexeme < -100 || sourseArray[i].lexeme == '%' ){
            sintElem cnt = sourseArray[i];
            switch(cnt.lexeme)
            {
            case '^'://свич уже вырожден в 2 исхода потому заменяется на if
                recursionPow  (sourseArray,++i,j);
                break;
            case '%':
                recursionUnary(sourseArray,++i,j);
                break;
            default:
                recursionMain (sourseArray,++(++i),j);
                while(sourseArray[i].lexeme == ',')
                    recursionMain(sourseArray,++i,j);
                ++i;
            }
            walkthroughArray[j++] = cnt;
        }
        else if(sourseArray[i].lexeme == '('){
            recursionMain(sourseArray,++i,j);
            ++i;
        }
        else if(sourseArray[i].lexeme == ')' || sourseArray[i].lexeme == ',')
            return;
    }
}
void CExpression::recursionPow  (CExpression::sintElem* sourseArray, short &i, short &j)
{
    //В случае ^ нам только и нужно что выписать содержимое идущих далее скобок или одно число
    if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
        walkthroughArray[j++] = sourseArray[i++];
    else if(sourseArray[i].lexeme < -100 || sourseArray[i].lexeme == '%'){//Это может быть только функция
        sintElem cnt = sourseArray[i];
        if(sourseArray[i].lexeme == '%'){
            recursionUnary(sourseArray,++i,j);
        }
        else
        {
            recursionMain (sourseArray,++(++i),j);
            while(sourseArray[i].lexeme == ',')
                recursionMain(sourseArray,++i,j);
            ++i;//В конце стоит скобочка которую мы пропустили ++(++i) вызывая функцию
        }
        walkthroughArray[j++] = cnt;
    }
    else if(sourseArray[i].lexeme == '('){
        recursionMain(sourseArray,++i,j);
        ++i;
    }
}
void CExpression::recursionUnary(CExpression::sintElem* sourseArray, short &i, short &j)
{
    //В случае % нам только и нужно что выписать содержимое идущих далее скобок или одно число
    if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
        walkthroughArray[j++] = sourseArray[i++];
    else if(sourseArray[i].lexeme < -100){//Это может быть только функция
        sintElem cnt = sourseArray[i];
        recursionMain(sourseArray,++(++i),j);
        while(sourseArray[i].lexeme == ',')
            recursionMain(sourseArray,++i,j);
        ++i;//В конце стоит скобочка которую мы пропустили ++(++i) вызывая функцию
        walkthroughArray[j++] = cnt;
    }
    else if(sourseArray[i].lexeme == '('){
        recursionMain(sourseArray,++i,j);
        ++i;
    }
}
void CExpression::recursionMain (CExpression::sintElem* sourseArray,short&i,short&j)
{
    while(sourseArray[i].lexeme != 127){
        if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
            walkthroughArray[j++] = sourseArray[i++];
        else if(sourseArray[i].lexeme == '+' || sourseArray[i].lexeme == '-' || sourseArray[i].lexeme == '*' || sourseArray[i].lexeme == '/' || sourseArray[i].lexeme == '^' || sourseArray[i].lexeme < -100 || sourseArray[i].lexeme == '%'){
            sintElem cnt = sourseArray[i];
            switch(cnt.lexeme)
            {//Для +-/*^ приходится писать рекурсивную функцию другого вида с другими условиями выхода
            case'+':case'-':
                recursionPlus(sourseArray,++i,j);
                break;
            case'*':case'/':
                recursionMult(sourseArray,++i,j);
                break;
            case'^':
                recursionPow (sourseArray,++i,j);
                break;
            case'%':
                recursionUnary (sourseArray,++i,j);
                break;
            default://Если это не операции +-/*^ значит это функция и за её кодом идут скобки -> ++(++i)
                recursionMain(sourseArray,++(++i),j);
                while(sourseArray[i].lexeme == ',')
                    recursionMain(sourseArray,++i,j);
                ++i;//В конце стоит скобочка которую мы пропустили ++(++i) вызывая функцию
            }
            walkthroughArray[j++] = cnt;
        }
        else if(sourseArray[i].lexeme == '('){
            recursionMain(sourseArray,++i,j);
            ++i;
        }
        else if(sourseArray[i].lexeme == ')' || sourseArray[i].lexeme == ',')
            return;
    }
}
void CExpression::sortPoland (sintElem* sourseArray)
{
/*Сортирует масив sourseArray в польскую нотацию переписывая его в окончательный
  массив предразбора строки walkthroughArray, удаляет скобочки.
  Непосредственно в теле функции происходит только выделение памяти под
  окончательный массив. Сама же функция сортировки введу её рекурсивной природы
  разнесена по рекурсивным частям*/
    short int elemQuantity = 0,//Колличество элементов в новом масиве
              i = 0,           //Бегунок старого масива
              j = 0;           //Бегунок нового масива
    while(sourseArray[i].lexeme != 127)
        if(sourseArray[i++].lexeme != '(' && sourseArray[i-1].lexeme != ')' && sourseArray[i-1].lexeme != ',')
            ++elemQuantity;
    walkthroughArray = new sintElem [elemQuantity + 1];
    i = 0;
    recursionMain(sourseArray, i, j);//i и j педедаются по ссылке(!)
    walkthroughArray[j].lexeme = 127;
    /*
    std::cout << "Notation: ";
    for(i = 0; i < j; ++i)
        if(walkthroughArray[i].lexeme)
            std::cout << walkthroughArray[i].lexeme;
        else
            std::cout << walkthroughArray[i].number;
    std::cout << '\n';//*/
}



/*




Проверка на выражение



*/
/*inline*/ bool CExpression::isFigure(char s) { return (s >= '0' && s <= '9') || s == '.'; }
/*inline*/ bool CExpression::isNumber(char s) { return isFigure(s) || s == 'x' || s == 'y' || s == 't'; }
/*inline*/ bool CExpression::isLetter(char s) { return s >= 'a' && s <= 'z'; }
/*inline*/ bool CExpression::isUnary(char s) { return s == '%' || (s < -100 && s != -124 && s != -121); }
/*inline*/ bool CExpression::isOperation(char s) { return s && s != 'x' && s != 'y'; }
/*inline*/ bool CExpression::isExp(const std::string& str) { return isExp(str.c_str()); }
bool CExpression::isExp(const char* St)
{
/*
 Проверяет, является ли строка выражением. работает ощутимо долго.
(но не слишком. Но сопоставимо с полным разбором строки и переводом её в выражение)
*/
    if (!strlen(St))
    {
        return false;
    }
    int i = -1, j = 0;
    char* st = new char [strlen(St) * 2];
    while(St[++i])//Проверка на наличие посторонних символов
        if(St[i] != ' ' && St[i] != '\n') {
            if(St[i] >= 'A' && St[i] <= 'Z')
                st[j++] = St[i] - ('A' - 'a');
            else if(isLetter(St[i]) || isNumber(St[i]) ||
                                    St[i] == '*' || St[i] == '/' || St[i] == '+' || St[i] == '-' ||
                                    St[i] == '^' || St[i] == '(' || St[i] == ')' || St[i] == ',' || St[i] == '.')
                st[j++] = St[i];
            else if(St[i] == '}' || St[i] == ']')
                st[j++] = ')';
            else if(St[i] == '{' || St[i] == '[')
                st[j++] = '(';
            else
            {
                delete[] st;
                return !true && !false;
            }
        }
    i = -1;
    while(st[++i])
        if(st[i] == 'p' && st[i + 1] == 'i')//Если это pi, заменить его на 77, чтобы в дальнейшем анализировать как число
        {
            st[i] = '7';
            st[++i] = '7';
        }
        else if(st[i] == 'e' || st[i] == 'x' || st[i] =='y' || st[i] == 't' )//если это e, y или x
            st[i] = '2';
    st[j] = i = 0;
    //std::cout << "Symbols is right.\nFree string is \"" << st << "\"\n";
    if(*st != 0 && ( *st != '+' && *st != '-' && *st != '(' && !isNumber(*st) && !isLetter(*st) ) )//Проверка на порядок следования
    {
        delete[] st;
        return !true && !false;
    }
    //std::cout << "First symbol is right.\n";

    while(st[++i])
        if( (( st[i] == '+' || st[i] == '-' || st[i] == '/' || st[i] == '*' || st[i] == '^' || st[i] == '(' || st[i] == ',' ) &&
            !isNumber(st[i + 1]) && !isLetter(st[i + 1]) && st[i + 1] != '(' && st[i + 1] != '-' && st[i + 1] != '+')
        || (st[i] == ')' &&
            !isNumber(st[i + 1]) && !isLetter(st[i + 1]) && st[i + 1] != '(' && st[i + 1] != '-' && st[i + 1] != '+' && st[i + 1] != '^'
                                     && st[i + 1] != ')' && st[i + 1] != '*' && st[i + 1] != '/' && st[i + 1] != ','  && st[i + 1])
        || (isLetter(st[i]) &&
            ( !st[i + 1] || (!isLetter(st[i + 1]) && st[i + 1] != '(' ))))
        {
            //std::cout << i << " is " << st[i] << '\n';
            delete[] st;
            return !true && !false;
        }
    //Тест последнего символа
    --i;
    if(st[i] != ')' && ( st[i] < '0' || st[i] > '9' ) )
    {
        delete[] st;
        return !true && !false;
    }
    //std::cout << "Sequence is right.\n";
    //qDebug() << "()...";
    i = -1;
    j = 0;
    while(st[++i])//Проверка на скобочки
    {
        if(st[i] == '(')
            ++j;
        else if(st[i] == ')')
            --j;
        if( j < 0 )
        {
            delete[] st;
            return !true && !false;
        }
    }
    if(j)
    {
        delete[] st;
        return !true && !false;
    }
    //std::cout << "Brackets is right.\n";
    //qDebug() << "funk...";
    addCode(st);
    i = -1;
    while(st[++i])
    {
        if(isLetter(st[i]) || (i && st[i - 1] < -100 && st[i] < -100))
        {//Если есть неопознанные функции - символы не заменённые на экваваленты или две функции подрят
            //std::cout << st << std::endl;
            delete[] st;
            return !true && !false;
        }
    }

    //qDebug() << "true";
    delete[] st;
    return true;
}


/*

 оптимизация выражения

*/
CExpression::sintElem CExpression::doIt(char op, sintElem a)
{
/*
Выполнение унарных операций
*/
    //std::cout << op << a.number << std::endl;
    sintElem ret;
    ret.lexeme = 0;
    switch (op)
    {
        case '%':
            ret.number = 0 - a.number;
            break;
        case -128:
            ret.number = sin(a.number);
            break;
        case -127:
            ret.number = cos(a.number);
            break;
        case -126:
            ret.number = tan(a.number);
            break;
        case -125:
            ret.number = 1 / tan(a.number);
            break;
        case -123:
            ret.number = log10(a.number);
            break;
        case -122:
            ret.number = log(a.number);
            break;
        case -120:
            ret.number = pow(a.number,0.5);
            break;
        case -119:
            ret.number = a.number * a.number;
            break;
        case -118:
            ret.number = *numArray>0 ? a.number : 0 - a.number;
            break;
        case -117:
            ret.number = asin(a.number);
            break;
        case -116:
            ret.number = acos(a.number);
            break;
        case -115:
            ret.number = atan(a.number);
            break;
        case -114:
            ret.number = atan(1 / a.number);
            break;
        case -113:
            ret.number = sinh(a.number);
            break;
        case -112:
            ret.number = cosh(a.number);
            break;
    }
    return ret;
}
CExpression::sintElem CExpression::doIt(char op, sintElem a, sintElem b)
{
/*
Выполнение бинарных операций
*/
    //std::cout << a.number << op << b.number << std::endl;
    sintElem ret;
    ret.lexeme = 0;
    switch (op)
    {
        case '+':
            ret.number = a.number + b.number;
            break;
        case '*':
            ret.number = a.number * b.number;
            break;
        case '-':
            ret.number = a.number - b.number;
            break;
        case '/':
            ret.number = a.number / b.number;
            break;
        case '^':case -121:
            ret.number = pow(a.number, b.number);
            break;
        case -124:
            ret.number = log(a.number)/log(b.number);
            break;
    }
    return ret;
}
unsigned CExpression::operatorCount(sintElem* op)
{

    unsigned k = 1;
    signed j = 0;
    while(k)//проверку на достижение начала массива ставить не нужно, если мы предпологаем нотацию разрешаемой
    {//Ну пошутил. . . Если это оператор, добавить 1 если он бинарный, если не оператор (число) вычесть 1.
        k += isOperation(op[-++j].lexeme) ?  !isUnary(op[-j].lexeme) : -1;//по моему это приведёт нас ко второму операнду
    }
    return j;
}
/*std::ostream& operator << (std::ostream &cout, CExpression::sintElem el)
{
    if(el.lexeme)
        cout << el.lexeme;
    else
        cout << el.number;
    return cout;
}*/
void CExpression::delElements(sintElem* from, unsigned how)
{
/*
Удаляет how элементов из массива from, без проверок
*/
    for(unsigned i = 0; from[i + how - 1].lexeme != 127; ++i)
    {
        from[i] = from[i + how];
    }
}
CExpression& CExpression::optimization()
{
/*
Проводит беглый предрасчёт выражения. Не учитывает комутативность операци.
*/
    if (!fine)
    {
        return *this;
    }
    unsigned counter = 0;
    while(walkthroughArray[counter].lexeme != 127)
        ++counter;
    sintElem* temp = new sintElem[counter + 1];

    unsigned i = 0, j = 0;
    while(walkthroughArray[i].lexeme != 127)
    {
        if(!walkthroughArray[i].lexeme || walkthroughArray[i].lexeme == 'x' || walkthroughArray[i].lexeme == 'y')
            temp[j++] = walkthroughArray[i];
        else
            if(isUnary(walkthroughArray[i].lexeme))
                if( !temp[j - 1].lexeme )
                    temp[j - 1] = doIt(walkthroughArray[i].lexeme, temp[j - 1]);
                else
                    temp[j++] = walkthroughArray[i];
            else
                if( !temp[j - 1].lexeme && !temp[j - 2].lexeme ) {
                    --j;
                    temp[j - 1] = doIt(walkthroughArray[i].lexeme, temp[j - 1], temp[j]);
                } else
                    temp[j++] = walkthroughArray[i];
        ++i;
    }
    temp[j].lexeme = 127;
    /*Безсмысленные операции:
* 1, 0
+ 0
/ 1
- 0
^ 1

я вижу *, как я ищу операнды? все возможные операции находящиеся перед ним уже выполнены, т. е. может быть 1 x 3 ^ * это же 1*х^3, что должно быть x^3,
что значит что я должен собирать какой-то стек из переменных пока не найду в обоих сторонах x или не соберу все, тогда с одной из сторон должен остатся
только безсмысленный операнд, проверить обе стороны на его наличие, удалить требуемое.
*/

    i = j = 0;
    if(temp[1].lexeme != 127)
        ++(++i);//речь о бинарных операциях, нет смысла изучать что там раньше третьего знака.
    while(temp[i].lexeme != 127)
    {
        switch(temp[i].lexeme)
        {
        case '*':
            if(!temp[i - 1].lexeme && temp[i - 1].number == 1)//правый операнд мы можем проверить сразу
            {
                delElements(temp + i - 1, 2);//убираем 1 и *
                --(--i);
            }
            else if(!temp[i - 1].lexeme && temp[i - 1].number == 0)
            {
                temp[i].lexeme = 0;
                temp[i].number = 0;//Чтобы не вызывать два удаления записываем 0 вместо *
                j = operatorCount(temp + i - 1);//собираем второй операнд, удаляем его и *.
                delElements(temp + i - j - 1, j + 1);
                i -= j + 1;
            }
            else//иначе просто собираем всё что попадает в правый операнд
            {
                j = operatorCount(temp + i);
                if(!temp[i - j - 1].lexeme && temp[i - j - 1].number == 1)
                {//удаляем 1 и само умножение.
                    delElements(temp + i, 1);
                    delElements(temp + i - j - 1, 1);
                    --(--i);
                }
                else if(!temp[i - j - 1].lexeme && temp[i - j - 1].number == 0)
                {
                    delElements(temp + i - j, j + 1);//удаляем всё кроме нуля
                    i -= j + 1;
                }
            }
            break;
        case '+': case '-':
            if(!temp[i - 1].lexeme && temp[i - 1].number == 0)
            {
                delElements(temp + i - 1, 2);
                --(--i);
            }
            else
            {
                j = operatorCount(temp + i);
                if(!temp[i - j - 1].lexeme && temp[i - j - 1].number == 0)
                {
                    delElements(temp + i, 1);
                    delElements(temp + i - j - 1, 1);
                    --(--i);
                }
            }
            break;
        case '/':
            if(!temp[i - 1].lexeme && temp[i - 1].number == 1)
            {
                delElements(temp + i - 1, 2);
                --(--i);
            }
            if(!temp[i - 1].lexeme && temp[i - 1].number == 0)
            {
                temp[i].lexeme = 0;
                temp[i].number = 1;
                j = operatorCount(temp + i - 1);
                delElements(temp + i - j - 1, j + 1);
                i -= j + 1;
            }
            else
            {
                j = operatorCount(temp + i);
                if(!temp[i - j - 1].lexeme && temp[i - j - 1].number == 0)
                {
                    delElements(temp + i - j, j + 1);
                    i -= j + 1;
                }
            }
            break;
        case '^': case -121:
            if(!temp[i - 1].lexeme && temp[i - 1].number == 1)
            {
                delElements(temp + i - 1, 2);
                --(--i);
            }
            else
            {
                j = operatorCount(temp + i);
                if(!temp[i - j - 1].lexeme && ( temp[i - j - 1].number == 0 || temp[i - j - 1].number == 1 ) )
                {
                    delElements(temp + i - j, j + 1);
                    i -= j + 1;
                }
            }
            break;
        }
        ++i;
    }

    delete[] walkthroughArray;
    walkthroughArray = temp;

    /*
    std::cout << "Оптимизированная нотация:\n";
    for(i = 0; walkthroughArray[i].lexeme != 127; ++i)
        if(walkthroughArray[i].lexeme)
            std::cout << walkthroughArray[i].lexeme << std::endl;
        else
            std::cout << walkthroughArray[i].number << std::endl;
    //*/

    return *this;
}



/*

Является ли константой данное выражение?

*/
bool CExpression::isConst()
{/*
 Предпологается, что функция применяется для полей ввода и прочего в том же роде.
 В таком контексте нет смысла делать её быстрой (например просто проверить
 присутствие х и у в выражении). По этому она и не быстрая.
  Если это нормальное выражение и после его оптимизации в нём осталось только одно число то true
*/
    optimization();
    return fine && walkthroughArray[0].lexeme == 0 && walkthroughArray[1].lexeme == 127;
}

bool CExpression::isConstFast() const
{// Таки просто проверка. . .
    for (int i = 0; walkthroughArray[i].lexeme != 127; ++i)
    {
        if (walkthroughArray[i].lexeme == 'x' || walkthroughArray[i].lexeme == 'y')
        {
            return false;
        }
    }
    return true;
}
