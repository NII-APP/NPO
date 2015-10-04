#ifndef BDFWRITER_H
#define BDFWRITER_H

class FEM;
class QString;

class BDFWriter
{
public:
    BDFWriter(const FEM*);

    void write(const QString&);

private:
    const FEM& __model;
};

#endif // BDFWRITER_H
