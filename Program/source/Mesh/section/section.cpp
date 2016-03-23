#include "section.h"

#include <QDataStream>
#include <QDebug>

#include "shell.h"
#include "solid.h"

Section::Section()
{

}

Section::~Section()
{
}

namespace {

Section* resolveType(quint32 t)
{
    switch (t) {
    case Section::ShellType:
        return new Shell;
    case Section::SolidType:
        return new Solid;
    default:
        return nullptr;
    }
}
}

bool Section::operator==(const Section& op) const
{
    if (type() != op.type()) {
        return false;
    }
    return true;
}

Section* Section::load(QDataStream& in)
{
    quint32 t;
    in >> t;
    Section* const rez(resolveType(t));
    rez->loadV(in);
    return rez;
}

QDataStream& operator<<(QDataStream& s, const Section& v)
{
    s << static_cast<quint32>(v.type());
    v.save(s);
    return s;
}
