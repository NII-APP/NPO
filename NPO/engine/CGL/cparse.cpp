#include "cparse.h"
#include <cstring>
#include <ios>
#include <iostream>
#include <conio.h>
namespace CGL {

CParse::CParse(char *string)
    : d(string)
{
}

CParse::CParse(const char *string)
    : d(new char[strlen(string) + 1])
{
    strcpy(d, string);
}

void CParse::UNIXRowSymbol() {
    int v = 0;
    char* k(d);
    while(k[v]) {
        ++k;
        if (k[v] == 13)
            ++v;
        *k = k[v];
    }
}

CParse CParse::parseFile(const std::string& filename)
{
    CParse out;

    size_t length;

    std::ifstream is;
    is.open(filename.c_str(), std::ios::binary );
    if (is.bad()) {
        out.d = 0;
        return out;
    }

    is.seekg (0, std::ios::end);
    length = is.tellg();
    is.seekg (0, std::ios::beg);

    out.d = new char [length + 10];
    out.d[length] = '\0';

    // read data as a block:
    is.read(out.d,length);
    is.close();
    return out;
}

int CParse::integer()
{
    while (*d != '-' && *d != '+' && ( *d < '0' || *d > '9')) {
        ++d;
    }

    int num = 0;
    bool flag = *d == '-';
    *d += flag;

    while (*d >= '0' && *d <= '9')
    {
        num *= 10;
        num += *d - '0';
        ++d;
    }

    return flag ? -num : num;
}
double CParse::real()
{
    while (*d == ' ' || *d == '\t' || *d == '\n')
    {
        ++d;
    }

    bool flag = ( *d == '-' );
    if (*d == '-' || *d == '+')
    {
        ++d;
    }


    double num(0);
    double k(1.);
    while ((*d >= '0' && *d <= '9') || *d == '.')
    {
        if (*d != '.' && k == 1.)
        {
            num *= 10.;
            num += *d - '0';
        }
        else if (*d != '.')
        {
            num += (*d - '0') * k;
            k /= 10.;
        }
        else
        {
            k = .1;
        }
        ++d;
    }

    if (*d != 'e' && *d != 'E' && *d != '-' && *d != '+')
        return flag ? -num : num;

    if (*d == 'e' || *d == 'E')
        ++d;
    if (*d == '-')
    {
        k = 0.1;
        ++d;
    }
    else if (*d == '+')
    {
        k = 10.;
        ++d;
    }

    for (int e = integer(); e; --e)
        num *= k;

    return flag ? -num : num;
}
float CParse::fixFloat8()
{
    int sign(1), expV(0);
    float expK(10.0f), pointK(1.0f), num(0.0f);
    bool exp(false);
    for (int i = 0; i != 8; ++d, ++i)
    {
        switch (*d)
        {
        case '-':
            if (i == 0)
            {
                sign = -1;
            }
            else
            {
                exp = true;
                expK = 0.1f;
            }
            break;
        case '+':
            if (i != 1)
            {
                exp = true;
            }
            break;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            if (exp) {
                expV *= 10;
                expV += *d - '0';
            }
            else if (pointK == 1.0f) {
                num *= 10.f;
                num += *d - '0';
            }
            else {
                num += (*d - '0') * pointK;
                pointK /= 10.0f;
            }
            break;
        case '.':
            pointK = 0.1f;
            break;
        case '\n':
            return num * sign;
        }
    }
    if (expV) {
        while (expV) {
            num *= expK;
            --expV;
        }
    }
    return num * sign;
}
std::string CParse::string()
{
    char* start(d);

    while (*d != '\n')
        ++d;

    std::string ret(d - start, '\0');
    for (int i(0); start + i != d; ++i)
    {
        ret[i] = start[i];
    }
    ++d;

    return ret;
}
std::string CParse::word()
{
    while ( *d == ' ' || *d == '\t' || *d == '\n')
        ++d;
    char* start = d;

    while ( *d != ' ' && *d != '\t' && *d != '\n')
        ++d;

    std::string ret(d - start, '\0');
    for (int i(0); start + i != d; ++i)
    {
        ret[i] = start[i];
    }
    ++d;

    return ret;
}
void CParse::skipRow()
{
    if (!d)
    {
        return;
    }
    while (*d && *d != '\n')
    {
        ++d;
    }
    if (*d == '\n')
    {
        ++d;
    }
}

void CParse::skipTo(const char* dest)
{
    if (!*d)
    {
        return;
    }
    bool founding(false);
    int i(0);
    while (!founding) {
        i = 0;
        founding = true;
        while (d[i] && founding && dest[i]) {
            founding = d[i] == dest[i];
            ++i;
        }
        ++d;
    }
    if (!d[i]) {
        d += i;
        return;
    }
    --d;
}

bool CParse::testPrew(const char *dest)
{
    if (!*d) {
        return false;
    }
    if (!*dest) {
        return *d;
    }
    bool ret;
    int i(0);
    do
    {
        ret = d[i] == dest[i];
        ++i;
    }
    while (ret && d[i] && dest[i]);
    return ret;
}




}
