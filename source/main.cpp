#include "HexWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    HexWidget hexWidget;
    hexWidget.show();
    hexWidget.setBuffer(QByteArray("Some test data, another test data"));
    hexWidget.setMinimum(50);
    hexWidget.resize(750, 480);

    qDebug() << hexWidget.getBuffer();

    return QApplication::exec();
}
