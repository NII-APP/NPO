#ifndef SECTION_H
#define SECTION_H

#include <vector>

class QDataStream;

class Section
{
protected:
    virtual void loadV(QDataStream &) = 0;
    virtual void save(QDataStream &) const = 0;
public:
    enum Type {
        ShellType,
        SolidType
    };

    Section();
    virtual ~Section();

    virtual Type type() const = 0;
    virtual Section* clone() const = 0;
    virtual int getMatId() const = 0;
    virtual void setProperty(int, int) = 0;
    virtual void setProperty(int, double) {}

    bool operator==(const Section& op) const;

    double getSection() { }

    static Section* load(QDataStream&);

    friend QDataStream& operator<<(QDataStream&, const Section&);
};

QDataStream& operator<<(QDataStream&, const Section&);

#endif // SECTION_H
