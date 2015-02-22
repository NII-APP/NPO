#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class Project;
class Identity;

class Application : public QApplication
{
    Identity* self;
    Project* pro;

public:
    Application(int argc, char** argv);
    ~Application();

    static const Identity* identity();
    static Project *project();
};

#endif // APPLICATION_H
