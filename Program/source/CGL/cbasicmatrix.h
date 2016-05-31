#ifndef CGENERICMATRIX_H
#define CGENERICMATRIX_H
#include <vector>
#include <cstddef>
#include <fstream>
#include <cassert>

#ifndef NOT_QT_AVAILABLE
#include <QDebug>
#include <QDataStream>
#endif

//В связи с тем, что рассматриваемая задача связана с обработкой табличных структур, в проект внедрён исходный код шаблонной двумерной матрицы. Её реализация выходит за рамки лабораторной работы.
template <typename T>
class CBasicMatrix
{
public:
    typedef T value_type;

protected:
    typedef std::vector<T> Data;
    typedef std::vector<T*> Pointers;
    Data memory;
    Pointers m;
    int wid;

    void repoint()
    {
        int i(0);
        for (T*& it : m) {
            it = memory.data() + i;
            i +=  wid;
        }
    }

    static void writeRow(QDebug out, const T *m, const int width)
    {
        switch (width) {
        case 1:
            out << '\t' << m[0] << '\n';
            return;
        case 2:
            out << '\t' << m[0] << ',' << m[1] << '\n';
            return;
        case 3:
            out << '\t' << m[0] << ',' << m[1] << ',' << m[2] << '\n';
            return;
        case 4:
            out << '\t' << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << '\n';
            return;
        case 5:
            out << '\t' << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << ',' << m[4] << '\n';
            return;
        case 6:
            out << '\t' << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << ',' << m[4] << ',' << m[5] << '\n';
            return;
        default:
            out << '\t' << m[0] << ',' << m[1] << ',' << m[2] << "..." << m[width - 3] << ',' << m[width - 2] << ',' << m[width - 1] << '\n';
            return;
        }
    }

public:
    CBasicMatrix() : wid(0) { }
    CBasicMatrix(const CBasicMatrix<T>& m) { this->operator =(m); }
    CBasicMatrix(int w, int h, const T& val = T()) { resize(w, h); fill(val); }


    void resize(int w, int h, T v = T())
    {
        wid = w;
        m.resize(h);
        memory.resize(w * h, v);
        repoint();
    }

    CBasicMatrix<T>& operator =(const CBasicMatrix<T> & m)
    {
        memory = m.memory;
        wid = m.wid;
        this->m.resize(m.m.size());
        repoint();
        return *this;
    }

    T** pointers() { return m.data(); }
    T* operator [](int r) { return m[r]; }
    const T* operator [](int r) const { return m[r]; }


    void removeRow(int r)
    {
        if (height() <= r)
            return;
        for (T* it(m[r]), *dest(it + wid), *end(memory.data() + memory.size()); dest < end; ++it, ++dest)
            *it = *dest;
        m.resize(m.size() - 1);
        memory.resize(memory.size() - wid);
        if (memory.size())
            repoint();
        else
            *this = CBasicMatrix<T>();
    }

    void removeColumn(int c)
    {
        if (width() <= c)
            return;
        if (width() == 1) {
            *this = CBasicMatrix<T>();
            return;
        }
        int i(0);
        T* it(m[0] + c), *dest(it), *end(memory.data() + memory.size());
        while (dest < end) {
            if (m[i] + c == dest) {
                ++i;
                ++dest;
            }
            *it = *dest;
            ++it;
            ++dest;
        }
        --wid;
        memory.resize(memory.size() - m.size());
        repoint();
    }

    void fill(const T& val)
    {
#ifndef QT_NO_DEBUG
        assert(memory.data() == m.front());
#endif
        std::fill(memory.begin(), memory.end(), val);
    }

    int width() const { return wid; }
    int height() const { return static_cast<int>(m.size()); }
    int size() const { return static_cast<int>(memory.size()); }
    bool empty() const { return memory.empty(); }
    T* data() { return memory.data(); }
    const T* data() const { return memory.data(); }



#ifndef NOT_QT_AVAILABLE
    template <typename Type>
    friend QDebug operator<< (QDebug out, const CBasicMatrix<Type> &obj);

    template <typename Type>
    friend QDataStream& operator<< (QDataStream&, const CBasicMatrix<Type>&);
    template <typename Type>
    friend QDataStream& operator>> (QDataStream&, CBasicMatrix<Type>&);
#endif
};

#ifndef NOT_QT_AVAILABLE

template <typename Type>
QDebug operator<< (QDebug out, const CBasicMatrix<Type>&obj) {
    if (obj.height() == 0 || obj.width() == 0) {
        out << "CMatrix(" << obj.height() << ',' << obj.width() << ") { }\n";
        return out;
    }
    if (obj.height() <= 10) {
        out << "CMatrix(" << obj.height() << ',' << obj.width() << ") {\n";
        for (int i(0); i != obj.height(); ++i) {
            CBasicMatrix<Type>::writeRow(out, obj[i], obj.width());
        }
        out << "}\n";
    } else {
        out << "CMatrix(" << obj.height() << ',' << obj.width() << ") {\n";
        for (int i(0); i != 5; ++i) {
            CBasicMatrix<Type>::writeRow(out, obj[i], obj.width());
        }
        out << "\t\t...\n";
        for (int i(obj.height() - 5); i != obj.height(); ++i) {
            CBasicMatrix<Type>::writeRow(out, obj[i], obj.width());
        }
        out << "}\n";
    }
    return out;
}

template <typename Type>
QDataStream& operator<< (QDataStream& out, const CBasicMatrix<Type>& m) {
    return out << m.memory << m.wid;
}

template <typename Type>
QDataStream& operator>> (QDataStream& in, CBasicMatrix<Type>& m) {
    in >> m.memory >> m.wid;
    if (m.wid) {
        m.m.resize(m.memory.size() / m.wid);
        m.repoint();
    }
    return in;
}
#endif //NOT_QT_AVAILABLE

#endif // CGENERICMATRIX_H

