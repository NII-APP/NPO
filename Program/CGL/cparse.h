#ifndef CPARSE_H
#define CPARSE_H

#include <fstream>
#include <string>

class CParse
{
protected:
    char* d;
public:
    CParse(char *string = 0);
    static CParse parseFile(const std::string& filename);
    inline char *data() { return d; }

    void UNIXRowSymbol();

    int integer();
    double real();
    std::string string();
    std::string row() { return string(); }
    std::string word();
    float fixFloat8() { return fixFloat(8); }
    float fixFloat16() { return fixFloat(16); }
    float fixFloat(const int fixLen);

    void skipRow();
    void skipRows(int);
    void skipTo(const char* dest);
    inline void skipTo(const std::string& dest) { skipTo(dest.c_str()); }
    bool testPrew(const char* dest);
    inline bool testPrew(const std::string& dest) { return testPrew(dest.c_str()); }

    CParse& operator+= (int v) { d += v; return *this; }
    CParse& operator-= (int v) { d -= v; return *this; }
    CParse& operator++ () { ++d; return *this; }
    CParse& operator++ (int) { ++d; return *this; }
    int operator- (const CParse& p) const { return static_cast<int>(d - p.d); }
    char operator* () const { return *d; }
    char operator[] (int i) const { return d[i]; }
};


#endif // CPARSE_H
