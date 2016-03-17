#include "csize.h"
#ifndef NOT_QT_AVAILABLE
#include <QSize>
#endif

CSize::CSize() {}
CSize::CSize(const CSize& s) : w(s.w), h(s.h) {}
CSize::CSize(int width, int height) : w(width), h(height) {}
#ifndef NOT_QT_AVAILABLE
CSize::CSize(const QSize& s) : w(s.width()), h(s.height()) {}
#endif

CSize::~CSize() { }

void CSize::setWidth(const int width)
{
    w = width;
}

void CSize::setHeight(const int height)
{
    h = height;
}

int CSize::width() const
{
    return w;
}

int CSize::height() const
{
    return h;
}
