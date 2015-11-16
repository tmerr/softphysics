#include "mainwindow.hpp"

int main() {
    MainWindow win(640, 480, "physics simulation");
    win.mainLoop();
    return 0;
}
