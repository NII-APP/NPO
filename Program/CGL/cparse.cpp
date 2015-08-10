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
{
    int len = static_cast<int>(strlen(string)) + 1;
    d = new char[len];
    memcpy(d, string, len);
}

void CParse::UNIXRowSymbol() {
    char* v(d);
    char* k(d);
    do {
        if (*v == 13)
            ++v;
        *k = *v;
        ++v;
        ++k;
    } while(*v);
}

CParse CParse::parseFile(const std::string& filename)
{
    CParse out;

    size_t length;

    std::ifstream is;
    is.open(filename.c_str(), std::ios::binary );
    if (is.bad()) {
        out.d = nullptr;
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
    while (*d && *d != '-' && ( *d < '0' || *d > '9')) {
        ++d;
    }

    int num = 0;
    bool flag(*d == '-');
    *d += flag;

    while (*d && *d >= '0' && *d <= '9') {
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
    if (*d == '-' || *d == '+') {
        ++d;
    }

    double num(0);
    double k(1.);
    double k2(10.);
    double k3(1.);
    while ((*d >= '0' && *d <= '9') || *d == '.' || *d == ',') {
        if (*d != '.' && *d != ',') {
            num *= k2;
            num += (*d - '0') * k;
            k /= k3;
        } else {
            k = .1;
            k3 = 10.;
            k2 = 1.0;
        }
        ++d;
    }

    if (*d != 'e' && *d != 'E' && *d != '+' && *d != '-') {
        return flag ? -num : num;
    }

    if (*d == 'E' || *d == 'e') {
        ++d;
    }

    k = 10.0;
    if (*d == '-') {
        k = 0.1;
        ++d;
    } else if (*d == '+') {
        ++d;
    }

    num *= pow(k, integer());
    return flag ? -num : num;
}
float CParse::fixFloat(int fixLen)
{
    int sign(1);
    int expV(0);
    float expK(10.0f);
    float k1(1.0f);
    float k2(10.0f);
    float k3(1.0f);
    float num(0.0f);
    bool exp(false);
    while (fixLen && *d && *d != '\n') {
        switch (*d)
        {
        case ' ':
            break;
        case '-':
            if (num) {
                exp = true;
                expK = 0.1f;
            } else {
                sign = -1;
            }
            break;
        case '+':
            if (num) {
                exp = true;
            }
            break;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            if (exp) {
                expV *= 10;
                expV += *d - '0';
            }
            else {
                num *= k2;
                num += (*d - '0') * k1;
                k1 /= k3;
            }
            break;
        case '.':
            k1 = 0.1f;
            k2 = 1.0f;
            k3 = 10.0f;
            break;
        }
        ++d;
        --fixLen;
    }
    num *= pow(expK, expV);
    return num * sign;
}
std::string CParse::string()
{
    char* start(d);

    while (*d != '\n' && *d && *d != '\r')
        ++d;

    std::string ret(d - start, '\0');
    for (int i(0); start + i != d; ++i) {
        ret[i] = start[i];
    }
    ++d;

    return ret;
}
std::string CParse::word()
{
    while ((*d == ' ' || *d == '\t' || *d == '\n' || *d == '\r') && *d) {
        ++d;
    }
    char* start(d);
    while (*d != ' ' && *d != '\t' && *d != '\n' && *d != '\r' && *d) {
        ++d;
    }

    std::string ret(d - start, '\0');
    for (int i(0); start + i != d; ++i) {
        ret[i] = start[i];
    }

    return ret;
}
void CParse::skipRow() {
    if (!d) {
        return;
    }
    while (*d && *d != '\n') {
        ++d;
    }
    if (*d == '\n') {
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
        return !!*d;
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
