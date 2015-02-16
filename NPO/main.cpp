#include "application.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    setlocale(LC_ALL,"RUS");

    return a.exec();
}
