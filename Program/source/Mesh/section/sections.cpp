#include "sections.h"
#include <QDataStream>
#include <QDebug>

Sections::Sections(const Sections& op)
    : std::vector<Section*>(op.size())
{
    Sections::const_iterator it(op.begin());
    Sections::iterator sect(begin());
    const Sections::iterator sectEnd(end());
    while (sect != sectEnd) {
        *sect = *it ? (*it)->clone() : nullptr;
        ++sect;
        ++it;
    }
}

Sections::~Sections()
{
    for (Section*& i : *this) {
        delete i;
    }
}

void Sections::reachOut(int s)
{
    if (size() <= s) {
        ++s;
        this->resize(s, nullptr);
    }
}

QDataStream& operator<< (QDataStream& out, const Sections& s)
{
    out << static_cast<quint32>(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] != nullptr) {
            out << i;
            out << *s[i];
        } else {
            out << size_t(-1);
        }
    }
    return out;
}

QDataStream& operator>> (QDataStream& in, Sections& s)
{
    quint32 sectionsSize;
    in >> sectionsSize;
    s.resize(sectionsSize, nullptr);
    for (size_t i = 0; i < sectionsSize; ++i) {
        size_t id;
        in >> id;
        if (id != size_t(-1)) {
            s[i] = Section::load(in);
        }
    }
    return in;
}

bool Sections::operator ==(const Sections& op) const
{
    if (op.size() != size()){
#ifndef QT_NO_DEBUG
        qDebug() << "\tshells size " << op.size() << " " << size();
#endif
        return false;
    }
    for (size_t i = 0; i < op.size(); ++i) {
        if (!(*op[i] == *this->operator [](i))) {
#ifndef QT_NO_DEBUG
            qDebug() << "\tshells i " << i;
#endif
            return false;
        }
    }
    return true;
}
