#include <QApplication>
#include "keithleyapp.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KeithleyApp window;
    window.setWindowTitle("Keithley 2000 Logger");
    window.resize(300, 150);
    window.show();
    return app.exec();
}
