#include "HexWidget.h"

HexWidget::HexWidget(QWidget *parent) : QWidget(parent) {
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    font = property("font").value<QFont>();
    font.setPixelSize(14);
    setFont(font);
}

void HexWidget::prependBuffer(const QByteArray &prependByteArray) {
    QRect rect;
    int pixelSize = font.pixelSize() * 2;
    rect.setSize(QSize(pixelSize, pixelSize));
    for (auto b : prependByteArray) {
        byteArray.prepend(ByteRectStruct_t{b, rect});
    }

    update();
}

void HexWidget::appendBuffer(const QByteArray &appendByteArray) {
    QRect rect;
    int pixelSize = font.pixelSize() * 2;
    rect.setSize(QSize(pixelSize, pixelSize));
    for (auto b : appendByteArray) {
        byteArray.append(ByteRectStruct_t{b, rect});
    }
    update();
}

void HexWidget::setBuffer(const QByteArray &setByteArray) {
    byteArray.clear();
    appendBuffer(setByteArray);
}

auto HexWidget::getBufferSize() const {
    return byteArray.size();
}

auto HexWidget::getBuffer() const {
    return byteArray.data();
}

void HexWidget::clearBuffer() {
    byteArray.clear();
}

void HexWidget::newColumn() {
    cursor.setX(cursor.x() + columnOffset + font.pixelSize());
}

void HexWidget::newRow(QPoint point) {
    cursor.setY(cursor.y() + rowOffset + font.pixelSize());
    cursor.setX(point.x());
}

void HexWidget::drawSelection(QPainter &painter) {
    if (selectedCellStruct.index == -1) {
        return;
    }

    QRect blackRect = selectedCellStruct.selection;
    blackRect.setRight(blackRect.x() + blackRect.width() - 1);
    painter.drawRect(blackRect);
    painter.fillRect(selectedCellStruct.selection, Qt::blue);
}

void HexWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.
            setPen(Qt::black);
    painter.
            setFont(QFont("monospace", font.pixelSize())
    );

    drawSelection(painter);
    drawHeader(painter);
    drawRows(painter);
    drawAscii(painter);
}

void HexWidget::drawHeader(QPainter &painter) {

}

void HexWidget::drawRows(QPainter &painter) {
    cursor = startHexPoint;
    quint64 counter = 0;

    for (auto &i : byteArray) {
        i.rect.moveBottomLeft(cursor);
        painter.drawText(cursor, QString::number(i.byte, 16).toUpper());
        counter++;

        newColumn();

        if (counter == columnNumber) {
            counter = 0;
            newRow(startHexPoint);
        }
    }
}

void HexWidget::drawAscii(QPainter &painter) {}

void HexWidget::mousePressEvent(QMouseEvent *event) {
    int key = event->button();

    if (key == Qt::LeftButton) {
        selection();

    }

    update();

}

void HexWidget::selection() {
    QPoint cursorPos = QCursor::pos();
    cursorPos = this->mapFromGlobal(cursorPos);
    selectedCellStruct.index = -1;

    for (int i = 0; i < byteArray.size(); ++i) {
        if (byteArray[i].rect.contains(cursorPos)) {
            selectedCellStruct.index = i;
            selectedCellStruct.selection = byteArray[i].rect;

            int centerX = byteArray[i].rect.center().x();

            if (cursorPos.x() < centerX) {
                selectedCellStruct.selection.setRight(centerX);
                selectedCellStruct.mask = MASK::FIRST;
            } else {
                selectedCellStruct.selection.setLeft(centerX);
                selectedCellStruct.mask = MASK::SECOND;
            }

            break;
        }
    }
}

void HexWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();

    if (selectedCellStruct.index != -1) {
        switch (key) {
            case Qt::Key_Left:
                goLeft();
                break;
            case Qt::Key_Right:
                goRight();
                break;
            case Qt::Key_Up:
                goUp();
                break;
            case Qt::Key_Down:
                goDown();
                break;
            default:
                break;
        }
    }
}

void HexWidget::goRight() {

}

void HexWidget::goLeft() {

}

void HexWidget::goUp() {

}

void HexWidget::goDown() {

}
