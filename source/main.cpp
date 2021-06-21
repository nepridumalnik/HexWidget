#include "HexWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    HexWidget hexWidget;
    hexWidget.setBuffer(QByteArray("Some test data, another test data"));
    hexWidget.setMinimum(50);
    hexWidget.show();

    qDebug() << hexWidget.getBuffer();

    return QApplication::exec();
}
