#ifndef CCOLORSCALE_H
#define CCOLORSCALE_H
#include <vector>
#include <QColor>
#include "crange.h"

namespace CGL {

class CColorScale
{
public:
    typedef std::vector<QRgb> ColorNodes;
private:
    ColorNodes array;
    CRealRange domain;
public:
    CColorScale()
    {
        array.push_back(0xFF0000);
        array.push_back(0x00FF00);
        array.push_back(0x0000FF);
    }

    ~CColorScale() {}

    QRgb operator()(const qreal& op) const {
        if (array.empty()) {
            return 0x000000;
        }
        if (array.size() == 1) {
            return array.front();
        }
        const size_t edges(array.size() - 1);
        const size_t node((op - domain.getMin()) / domain.range() * edges);
        if (node >= edges) {
            return array.back();
        }
        const CRealRange current(domain.range() / edges * node, domain.range() / edges * (node + 1));
        typedef CRange<short> Channel;
        const Channel red((array.at(node) / 0x10000) % 0x100, (array.at(node + 1) / 0x10000) % 0x100);
        const Channel green((array.at(node) / 0x100) % 0x100, (array.at(node + 1) / 0x100) % 0x100);
        const Channel blue(array.at(node) % 0x100, array.at(node + 1) % 0x100);
        const qreal part(current[op]);
        return (static_cast<QRgb>(red(part)) * 0x10000) + (green(part) * 0x100) + blue(part);
    }
    void setDomain(const CRealRange& v) { domain = v; }
    void setArray(const ColorNodes& v) { array = v; }
};


}

#endif // CCOLORSCALE_H
