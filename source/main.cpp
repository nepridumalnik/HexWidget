#include "HexWidget.h"
#include <QScrollArea>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    HexWidget *hexWidget = new HexWidget;
    hexWidget->setBuffer(QByteArray("Some test data, another test data"));
    hexWidget->setMinimum(100);

    qDebug() << hexWidget->getBuffer();

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setAutoFillBackground(false);
    scrollArea->setWidgetResizable(true);
    scrollArea->resize(640, 480);
    scrollArea->setWidget(hexWidget);
    scrollArea->show();

    return QApplication::exec();
}
