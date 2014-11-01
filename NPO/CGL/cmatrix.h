#ifndef CMatrix_H
#define CMatrix_H
#include <vector>
#include <cstddef>

class QDebug;
class QDataStream;

namespace CGL {

class CMatrix
{
public:
    typedef double T;
private:
    typedef std::vector<T> Data;
    typedef std::vector<T*> Pointers;

    Data data;
    Pointers m;
    size_t wid;
    void repoint();
public:
    CMatrix();
    CMatrix(size_t w, size_t h);
    CMatrix(const CMatrix&);

    void resize(size_t w, size_t h);

    CMatrix& operator =(const CMatrix &);

    T* operator [](size_t r) { return m[r]; }
    const T* operator [](size_t r) const { return m[r]; }

    T minInRow(size_t r);
    T minInColumn(size_t c);
    T minInRowExclude(size_t r, size_t exclude);
    T minInColumnExclude(size_t c, size_t exclude);
    void plusInRow(size_t r, const T& val);
    void plusInColumn(size_t c, const T& val);
    void excludeRow(size_t r);
    void excludeColumn(size_t c);

    void naNtoInf();

    size_t width() const { return wid; }
    size_t height() const { return m.size(); }

    friend QDebug operator<< (QDebug out, const CMatrix &obj);

    friend QDataStream& operator<< (QDataStream&, const CMatrix&);
    friend QDataStream& operator>> (QDataStream&, CMatrix&);
};

QDataStream& operator<< (QDataStream&, const CMatrix&);
QDataStream& operator>> (QDataStream&, CMatrix&);

QDebug operator<< (QDebug out, const CMatrix &obj);








}

#endif // CMatrix_H
