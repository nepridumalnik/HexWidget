#include "HexWidget.h"

HexWidget::HexWidget(QWidget *parent) : QWidget(parent) {
    selectionColor.setNamedColor(STEELBLUE);
    charColor.setNamedColor(BLACK);
    backgroundColor.setNamedColor(WHITESMOKE);

    QPalette pal(palette());
    pal.setColor(QPalette::Window, backgroundColor);
    setAutoFillBackground(true);
    setPalette(pal);

    appFont = property("font").value<QFont>();
    appFont.setPixelSize(14);
    setFont(appFont);
}

HexWidget::~HexWidget() {
    delete fm;
}

void HexWidget::prependBuffer(const QByteArray &prependByteArray) {
    int counter = 0;
    for (auto b : prependByteArray) {
        if (counter >= maximumSize) {
            break;
        }
        byteArray.prepend(ByteRectStruct{(quint8) b, getCellRect()});
        counter++;
    }

    update();
}

void HexWidget::appendBuffer(const QByteArray &appendByteArray) {
    int counter = 0;
    for (auto b : appendByteArray) {
        if (counter >= maximumSize) {
            break;
        }
        byteArray.append(ByteRectStruct{(quint8) b, getCellRect()});
        counter++;
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
    resizeBuffer();
}

void HexWidget::newColumn() {
    cursor.setX(cursor.x() + columnOffset + appFont.pixelSize());
}

void HexWidget::newRow(QPoint point) {
    cursor.setY(cursor.y() + rowWidth);
    cursor.setX(point.x());
}

void HexWidget::drawSelection(QPainter *painter) {
    if (-1 != selectedCellStruct.index) {
        painter->fillRect(selectedCellStruct.selection, selectionColor);
    }
}

void HexWidget::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    painter.setPen(charColor);
    painter.setFont(QFont("monospace", appFont.pixelSize()));

    drawSelection(&painter);
    drawRows(&painter);
}

void HexWidget::drawRows(QPainter *painter) {
    cursor = startHexPoint;
    int counter = 0;

    for (auto &i : byteArray) {
        i.rect.moveBottomLeft(cursor);

        QString text = QString::number((int) i.byte, 16).toUpper();
        if (1 == text.size()) {
            text = QChar('0') + text;
        }

        if (nullptr != painter) { painter->drawText(cursor, text.right(2)); }
        counter++;

        newColumn();

        if (counter == columnNumber) {
            counter = 0;
            newRow(startHexPoint);
        }
    }
}

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
                setSelectionCell(i, MASK::FIRST);
            } else {
                setSelectionCell(i, MASK::SECOND);
            }
            break;
        }
    }
}

void HexWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();

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
        case Qt::Key_Space:
            insertNewByte();
            break;
        case Qt::Key_Backspace:
            removePrevious();
            break;
        case Qt::Key_Delete:
            removeCurrent();
            break;
        default:
            resetByteValue(key);
            break;
    }

    update();
}

void HexWidget::goRight() {
    if (-1 == selectedCellStruct.index and !byteArray.empty()) {
        selectedCellStruct.index = byteArray.size() - 1;
    }

    if (selectedCellStruct.index >= (byteArray.size() - 1)) {
        selectedCellStruct.mask = MASK::SECOND;
        selectedCellStruct.index = (byteArray.size() - 1);
    } else {
        if (selectedCellStruct.mask == MASK::FIRST) {
            selectedCellStruct.mask = MASK::SECOND;
        } else {
            selectedCellStruct.mask = MASK::FIRST;
            selectedCellStruct.index++;
        }
    }

    setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
}

void HexWidget::goLeft() {
    if (-1 == selectedCellStruct.index and !byteArray.empty()) {
        selectedCellStruct.index = 0;
    }

    if (selectedCellStruct.index <= 0) {
        selectedCellStruct.mask = MASK::FIRST;
        selectedCellStruct.index = 0;
    } else {
        if (selectedCellStruct.mask == MASK::FIRST) {
            selectedCellStruct.mask = MASK::SECOND;
            selectedCellStruct.index--;
        } else {
            selectedCellStruct.mask = MASK::FIRST;
        }
    }

    setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
}

void HexWidget::goUp() {
    if (-1 == selectedCellStruct.index and !byteArray.empty()) {
        selectedCellStruct.index = 0;
    }

    if (selectedCellStruct.index - columnNumber >= 0) {
        selectedCellStruct.index -= (qint32) columnNumber;
        setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
    } else {
        selectedCellStruct.index = 0;
        setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
    }
}

void HexWidget::goDown() {
    if (-1 == selectedCellStruct.index and !byteArray.empty()) {
        selectedCellStruct.index = byteArray.size() - 1;
    }

    if (byteArray.size() - selectedCellStruct.index - columnNumber >= 0) {
        selectedCellStruct.index += (qint32) columnNumber;
        setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
    } else {
        selectedCellStruct.index = byteArray.size() - 1;
        setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
    }
}

void HexWidget::setSelectionCell(int i, MASK mask) {
    if (i < 0 or i >= byteArray.size()) {
        return;
    }

    selectedCellStruct.index = i;
    selectedCellStruct.mask = mask;
    selectedCellStruct.selection = byteArray[i].rect;

    if (mask == MASK::FIRST) {
        selectedCellStruct.selection.setRight(byteArray[i].rect.center().x());
    } else {
        selectedCellStruct.selection.setLeft(byteArray[i].rect.center().x());
    }
}

void HexWidget::setFont(const QFont &font) {
    QWidget::setFont(font);
    appFont = font;

    delete fm;

    fm = new QFontMetrics(appFont);

    columnOffset = fm->horizontalAdvance(SYMBOL) * 3;
    rowWidth = fm->height() * 2;

    for (auto &i : byteArray) {
        i.rect = getCellRect();
    }

    update();
}

QRect HexWidget::getCellRect() {
    QRect rect;
    rect.setSize(QSize(rowWidth * 0.9f, fm->height() * 1.2));
    return rect;
}

void HexWidget::setSelectionColor(const char *color) {
    selectionColor.setNamedColor(color);
}

void HexWidget::setCharColor(const char *color) {
    charColor.setNamedColor(color);
}

void HexWidget::setBackgroundColor(const char *color) {
    backgroundColor.setNamedColor(color);
}

void HexWidget::resetByteValue(int key) {
    if (selectedCellStruct.index < 0 or selectedCellStruct.index >= byteArray.size()) {
        return;
    }

    quint8 newByte = 0;

    switch (key) {
        case Qt::Key_0:
            newByte = BYTE_VALUE::ZERO;
            break;
        case Qt::Key_1:
            newByte = BYTE_VALUE::ONE;
            break;
        case Qt::Key_2:
            newByte = BYTE_VALUE::TWO;
            break;
        case Qt::Key_3:
            newByte = BYTE_VALUE::THREE;
            break;
        case Qt::Key_4:
            newByte = BYTE_VALUE::FOUR;
            break;
        case Qt::Key_5:
            newByte = BYTE_VALUE::FIVE;
            break;
        case Qt::Key_6:
            newByte = BYTE_VALUE::SIX;
            break;
        case Qt::Key_7:
            newByte = BYTE_VALUE::SEVEN;
            break;
        case Qt::Key_8:
            newByte = BYTE_VALUE::EIGHT;
            break;
        case Qt::Key_9:
            newByte = BYTE_VALUE::NINE;
            break;
        case Qt::Key_A:
            newByte = BYTE_VALUE::A;
            break;
        case Qt::Key_B:
            newByte = BYTE_VALUE::B;
            break;
        case Qt::Key_C:
            newByte = BYTE_VALUE::C;
            break;
        case Qt::Key_D:
            newByte = BYTE_VALUE::D;
            break;
        case Qt::Key_E:
            newByte = BYTE_VALUE::E;
            break;
        case Qt::Key_F:
            newByte = BYTE_VALUE::F;
            break;
        default:
            return;
    }

    byteArray[selectedCellStruct.index].byte &= ~selectedCellStruct.mask;
    byteArray[selectedCellStruct.index].byte |= selectedCellStruct.mask & newByte;

    goRight();
}

void HexWidget::insertNewByte() {
    if (maximumSize <= byteArray.size()) {
        return;
    }

    if (selectedCellStruct.index >= 0) {
        byteArray.insert(selectedCellStruct.index + 1, ByteRectStruct{0, byteArray[selectedCellStruct.index].rect});
        selectedCellStruct.mask = MASK::FIRST;
        selectedCellStruct.index++;

        drawRows();
        selectedCellStruct.selection = byteArray[selectedCellStruct.index].rect;
        selectedCellStruct.selection.setRight(selectedCellStruct.selection.center().x());
    } else {
        if (byteArray.empty()) {
            byteArray += ByteRectStruct{0, getCellRect()};
        }
    }
}

void HexWidget::removeCurrent() {
    const int size = byteArray.size();

    if (size <= minimumSize) {
        return;
    }

    if (size < 1) {
        return;
    } else if (size == 1) {
        byteArray.removeAt(0);
        selectedCellStruct.index = -1;
        return;
    } else if (selectedCellStruct.index < size - 1) {
        byteArray.removeAt(selectedCellStruct.index);
    } else {
        byteArray.removeAt(selectedCellStruct.index);
        selectedCellStruct.index--;
        selectedCellStruct.selection = byteArray[selectedCellStruct.index].rect;
        selectedCellStruct.selection.setRight(selectedCellStruct.selection.center().x());
    }
}

void HexWidget::removePrevious() {
    if (byteArray.size() <= minimumSize) {
        return;
    }

    if (selectedCellStruct.index > 0) {
        selectedCellStruct.index--;
        byteArray.removeAt(selectedCellStruct.index);
        selectedCellStruct.selection = byteArray[selectedCellStruct.index - 1].rect;
        selectedCellStruct.selection.setRight(selectedCellStruct.selection.center().x());
    }
}

void HexWidget::setMinimum(int newMinimumSize) {
    minimumSize = newMinimumSize;
    resizeBuffer();
}

void HexWidget::setMaximum(int newMaximumSize) {
    maximumSize = newMaximumSize;
    resizeBuffer();
}

void HexWidget::resizeBuffer() {
    int tmp = minimumSize - byteArray.size();
    if (tmp > 0) {
        for (int i = 0; i < tmp; ++i) {
            byteArray.append(ByteRectStruct{0, getCellRect()});
        }
    }

    if (maximumSize < byteArray.size()) {
        byteArray.resize(maximumSize);
    }
}

QByteArray HexWidget::getBuffer() {
    QByteArray array;
    for (auto b : byteArray) {
        array.append((char) b.byte);
    }
    return array;
}
