#ifndef CPARALLELEPIPED_H
#define CPARALLELEPIPED_H
#include <QtGlobal>
#include <cmath>
#include <QVector3D>
#include <QRectF>
#include <QDebug>
class QRectF;
class QVector3D;

class CParallelepiped
{
    qreal xP;
    qreal yP;
    qreal zP;
    qreal xL;
    qreal yL;
    qreal zL;
    void testBumpy(qreal& min, qreal& max) {
        qreal d(max - min);
        if (d == 0) {
            if (min) {
                max += 0.1 * d;
                min -= 0.1 * d;
            } else {
                min = -1.0;
                max =  1.0;
            }
        }
    }
public:
    CParallelepiped();
    CParallelepiped(qreal x1, qreal x2, qreal y1, qreal y2, qreal z1, qreal z2);
    CParallelepiped(qreal l, qreal r, qreal b, qreal t, qreal n);
    CParallelepiped(qreal l, qreal r, qreal b, qreal t);
    CParallelepiped(qreal l, qreal r, qreal b);
    explicit CParallelepiped(const QVector3D&);

    inline qreal xMin() const { return xL; }
    inline qreal xMax() const { return xP; }
    inline qreal yMin() const { return yL; }
    inline qreal yMax() const { return yP; }
    inline qreal zMin() const { return zL; }
    inline qreal zMax() const { return zP; }
    inline qreal  width() const { return xP - xL; }
    inline qreal height() const { return yP - xL; }
    inline qreal length() const { return zP - zL; }
    inline qreal size() const { qreal w(width()), h(height()), l(length()); return sqrt(w * w + h * h + l * l); }

    void move(const QVector3D& v);

    inline void xTo(qreal v) { xL = xP = v; }
    inline void yTo(qreal v) { yL = yP = v; }
    inline void zTo(qreal v) { zL = zP = v; }

    inline void multX(qreal k) { xL *= k; xP *= k; }
    inline void multY(qreal k) { yL *= k; yP *= k; }
    inline void multZ(qreal k) { zL *= k; zP *= k; }

    inline QVector3D center() const { return QVector3D((xP + xL) / 2., (yP + yL) / 2., (zL + zP) / 2.); }

    inline void includeX(qreal v) { if (!(v <= xP)) { xP = v; } if (!(v >= xL)) xL = v; }
    inline void includeY(qreal v) { if (!(v <= yP)) { yP = v; } if (!(v >= yL)) yL = v; }
    inline void includeZ(qreal v) { if (!(v <= zP)) { zP = v; } if (!(v >= zL)) zL = v; }
    inline void include(const QVector3D& v) { includeX(v.x()); includeY(v.y()); includeZ(v.z()); }
    inline void include(const double* v) { includeX(*v); includeY(v[1]); includeZ(v[2]); }
    inline void include(const float*  v) { includeX(*v); includeY(v[1]); includeZ(v[2]); }

    inline void setFlatY(qreal v) { yP = yL = v; }
    inline void setFlatZ(qreal v) { zP = yL = v; }
    inline void setFlatY() { yP = yL; }
    inline void setFlatZ() { zP = yL; }
    inline void testBumpy() { testBumpyX(); testBumpyY(); testBumpyZ(); }
    inline void testBumpyX() { testBumpy(xL, xP); }
    inline void testBumpyY() { testBumpy(yL, yP); }
    inline void testBumpyZ() { testBumpy(zL, zP); }

    operator QRectF() const { return QRectF(xMin(),yMax(),width(), height()); }

    CParallelepiped& operator +=(const CParallelepiped& e);
    CParallelepiped& operator |=(const CParallelepiped& e) { return *this += e; }
    bool operator ==(const QVector3D& v) const;
    bool operator !=(const QVector3D& v) const;
    friend QDataStream& operator >> (QDataStream& in, CParallelepiped& p);
};
inline bool operator ==(const QVector3D& a, const CParallelepiped& b) { return b == a; }
inline bool operator !=(const QVector3D& a, const CParallelepiped& b) { return b != a; }
QDebug operator << (QDebug out, const CParallelepiped& p);
QTextStream& operator << (QTextStream& out, const CParallelepiped& p);
QDataStream& operator << (QDataStream& out, const CParallelepiped& p);
QDataStream& operator >> (QDataStream& in, CParallelepiped& p);
#endif // CPARALLELEPIPED_H
