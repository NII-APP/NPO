#ifndef CSIZE_H
#define CSIZE_H

class QSize;

class CSize
{
public:
    CSize(const CSize&);
    CSize(int width, int height);
    CSize();
    CSize(const QSize&);
    ~CSize();

    void setWidth(const int width);
    void setHeight(const int height);
    int width() const;
    int height() const;
private:
    int w;
    int h;
};

#ifndef NOT_QT_AVAILABLE
#include <QDebug>
inline QDebug operator<<(QDebug out, const CSize& s) {
    out << "CSize{" << s.width() << ',' << s.height() << '}';
    return out;
}
#endif

#endif // CSIZE_H
