#include "HexWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    HexWidget hexWidget;
    hexWidget.show();
    hexWidget.setBuffer(QByteArray("Some test data, another test data"));
    hexWidget.setMinimum(50);
    hexWidget.resize(750, 480);

    qDebug() << hexWidget.getBuffer();

    try {
        hexWidget.getByte(100);
    }
    catch (std::exception &e) {
        qDebug() << e.what();
    }

    try {
        hexWidget.setByte(100, 0);
    }
    catch (std::exception &e) {
        qDebug() << e.what();
    }

    return QApplication::exec();
}
