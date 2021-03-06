#include "HexWidget.h"

HexWidget::HexWidget(QWidget *parent) : QWidget(parent)
{
    selectionColor.setNamedColor(SELECTION_COLOR);
    charColor.setNamedColor(TEXT_COLOR);
    backgroundColor.setNamedColor(BACKGROUND_COLOR);

    QPalette pal(palette());
    pal.setColor(QPalette::Window, backgroundColor);
    setAutoFillBackground(true);
    setPalette(pal);

    appFont = property("font").value<QFont>();
    appFont.setPixelSize(14);
    setWidgetFont(appFont);

    setFocusPolicy(Qt::StrongFocus);
}

HexWidget::~HexWidget()
{
    delete fm;
}

void HexWidget::prependBuffer(const QByteArray &prependByteArray)
{
    int counter = 0;
    for (auto b : prependByteArray)
    {
        if (counter >= maximumSize)
        {
            break;
        }
        byteArray.prepend(ByteRectStruct{(quint8)b, getCellRect()});
        counter++;
    }

    emit onTextUpdate();
    update();
}

void HexWidget::appendBuffer(const QByteArray &appendByteArray)
{
    int counter = 0;
    for (auto b : appendByteArray)
    {
        if (counter >= maximumSize)
        {
            break;
        }
        byteArray.append(ByteRectStruct{(quint8)b, getCellRect()});
        counter++;
    }

    emit onTextUpdate();
    update();
}

void HexWidget::setBuffer(const QByteArray &setByteArray)
{
    byteArray.clear();
    appendBuffer(setByteArray);
}

int HexWidget::getBufferSize() const
{
    return byteArray.size();
}

void HexWidget::clearBuffer()
{
    byteArray.clear();
    resizeBuffer();
}

void HexWidget::newColumn()
{
    cursor.setX(cursor.x() + columnOffset + appFont.pixelSize());
}

void HexWidget::newRow(QPoint point)
{
    if (singleLine)
    {
        return;
    }

    cursor.setY(cursor.y() + rowWidth);
    cursor.setX(point.x());
}

void HexWidget::drawSelection(QPainter *painter)
{
    if (-1 != selectedCellStruct.index)
    {
        painter->fillRect(selectedCellStruct.selection, selectionColor);
    }
}

void HexWidget::resizeText(QSize newSize)
{
    setMinimumSize(newSize);
}

void HexWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(charColor);
    painter.setFont(QFont("monospace", appFont.pixelSize()));

    drawSelection(&painter);
    drawRows(&painter);
}

void HexWidget::drawRows(QPainter *painter)
{
    cursor = startHexPoint;
    int counter = 0;

    QSize hexSize;
    hexSize.setHeight(0);
    hexSize.setWidth((getCellRect().width() + X_OFFS) * columnNumber);

    for (auto &i : byteArray)
    {
        i.rect.moveBottomLeft(cursor);

        QString text = QString::number((int)i.byte, 16).toUpper();
        if (1 == text.size())
        {
            text = QChar('0') + text;
        }

        if (nullptr != painter)
        {
            painter->drawText(cursor, text.right(2));
        }
        counter++;

        newColumn();

        if (counter == columnNumber)
        {
            counter = 0;
            newRow(startHexPoint);
        }

        hexSize.setHeight(i.rect.bottom() + Y_OFFS);
    }

    if (autoResize)
    {
        if (0 == hexSize.height())
        {
            hexSize.setHeight(rowWidth);
        }
        resizeText(hexSize);
    }
}

void HexWidget::mousePressEvent(QMouseEvent *event)
{
    int key = event->button();
    if (key == Qt::LeftButton)
    {
        selection();
    }

    emit onTextUpdate();
    update();
}

void HexWidget::selection()
{
    QPoint cursorPos = QCursor::pos();
    cursorPos = this->mapFromGlobal(cursorPos);
    selectedCellStruct.index = -1;

    for (int i = 0; i < byteArray.size(); ++i)
    {
        if (byteArray[i].rect.contains(cursorPos))
        {
            selectedCellStruct.index = i;
            selectedCellStruct.selection = byteArray[i].rect;

            int centerX = byteArray[i].rect.center().x();

            if (cursorPos.x() < centerX)
            {
                setSelectionCell(i, MASK::FIRST);
            }
            else
            {
                setSelectionCell(i, MASK::SECOND);
            }
            break;
        }
    }
}

void HexWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    switch (key)
    {
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

    emit onTextUpdate();
    update();
}

void HexWidget::goRight()
{
    if (-1 == selectedCellStruct.index && !byteArray.empty())
    {
        selectedCellStruct.index = byteArray.size() - 1;
    }

    if (selectedCellStruct.index >= (byteArray.size() - 1))
    {
        selectedCellStruct.mask = MASK::SECOND;
        selectedCellStruct.index = (byteArray.size() - 1);
    }
    else
    {
        if (selectedCellStruct.mask == MASK::FIRST)
        {
            selectedCellStruct.mask = MASK::SECOND;
        }
        else
        {
            selectedCellStruct.mask = MASK::FIRST;
            selectedCellStruct.index++;
        }
    }

    setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
}

void HexWidget::goLeft()
{
    if (-1 == selectedCellStruct.index && !byteArray.empty())
    {
        selectedCellStruct.index = 0;
    }

    if (selectedCellStruct.index <= 0)
    {
        selectedCellStruct.mask = MASK::FIRST;
        selectedCellStruct.index = 0;
    }
    else
    {
        if (selectedCellStruct.mask == MASK::FIRST)
        {
            selectedCellStruct.mask = MASK::SECOND;
            selectedCellStruct.index--;
        }
        else
        {
            selectedCellStruct.mask = MASK::FIRST;
        }
    }

    setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
}

void HexWidget::goUp()
{
    if (-1 == selectedCellStruct.index && !byteArray.empty())
    {
        selectedCellStruct.index = 0;
    }

    if (selectedCellStruct.index - columnNumber >= 0)
    {
        selectedCellStruct.index -= (qint32)columnNumber;
        setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
    }
    else
    {
        selectedCellStruct.index = 0;
        setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
    }
}

void HexWidget::goDown()
{
    if (-1 == selectedCellStruct.index && !byteArray.empty())
    {
        selectedCellStruct.index = byteArray.size() - 1;
    }

    if (byteArray.size() - selectedCellStruct.index - columnNumber >= 0)
    {
        selectedCellStruct.index += (qint32)columnNumber;
        setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
    }
    else
    {
        selectedCellStruct.index = byteArray.size() - 1;
        setSelectionCell(selectedCellStruct.index, selectedCellStruct.mask);
    }
}

void HexWidget::setSelectionCell(int i, MASK mask)
{
    if (i < 0 || i >= byteArray.size())
    {
        return;
    }

    selectedCellStruct.index = i;
    selectedCellStruct.mask = mask;
    selectedCellStruct.selection = byteArray[i].rect;

    if (mask == MASK::FIRST)
    {
        selectedCellStruct.selection.setRight(byteArray[i].rect.center().x());
    }
    else
    {
        selectedCellStruct.selection.setLeft(byteArray[i].rect.center().x());
    }
}

void HexWidget::setWidgetFont(const QFont &font)
{
    QWidget::setFont(font);
    appFont = font;

    delete fm;

    fm = new QFontMetrics(appFont);

    columnOffset = fm->boundingRect(QChar(SYMBOL)).width() * 3;
    rowWidth = fm->height() * 2;

    for (auto &i : byteArray)
    {
        i.rect = getCellRect();
    }

    emit onTextUpdate();
    update();
}

QRect HexWidget::getCellRect()
{
    QRect rect;
    rect.setSize(QSize(rowWidth * 0.7f, fm->height() * 1.1));
    return rect;
}

void HexWidget::setSelectionColor(const char *color)
{
    selectionColor.setNamedColor(color);
}

void HexWidget::setCharColor(const char *color)
{
    charColor.setNamedColor(color);
}

void HexWidget::setBackgroundColor(const char *color)
{
    backgroundColor.setNamedColor(color);
}

void HexWidget::resetByteValue(int key)
{
    if (selectedCellStruct.index < 0 || selectedCellStruct.index >= byteArray.size())
    {
        return;
    }

    const quint8 originalByte = byteArray[selectedCellStruct.index].byte;
    quint8 newByte = 0;

    if (Qt::Key_0 <= key && key <= Qt::Key_9)
    {
        newByte = (quint8)key - Qt::Key_0;
    }
    else if (Qt::Key_A <= key && key <= Qt::Key_F)
    {
        newByte = (quint8)key - Qt::Key_A + 0x0A;
    }
    else
    {
        return;
    }

    newByte += (newByte << 4);

    byteArray[selectedCellStruct.index].byte &= ~selectedCellStruct.mask;
    byteArray[selectedCellStruct.index].byte |= selectedCellStruct.mask & newByte;

    if (selectedCellStruct.index == byteArray.size() - 1 && MASK::SECOND == selectedCellStruct.mask &&
        byteArray[selectedCellStruct.index].byte == originalByte)
    {
        insertNewByte();
    }
    else
    {
        goRight();
    }
}

void HexWidget::insertNewByte()
{
    if (maximumSize <= byteArray.size())
    {
        return;
    }

    if (selectedCellStruct.index >= 0)
    {
        byteArray.insert(selectedCellStruct.index + 1, ByteRectStruct{0, byteArray[selectedCellStruct.index].rect});
        selectedCellStruct.mask = MASK::FIRST;
        selectedCellStruct.index++;

        drawRows();
        selectedCellStruct.selection = byteArray[selectedCellStruct.index].rect;
        selectedCellStruct.selection.setRight(selectedCellStruct.selection.center().x());
    }
    else
    {
        if (byteArray.empty())
        {
            byteArray += ByteRectStruct{0, getCellRect()};
        }
    }
}

void HexWidget::removeCurrent()
{
    const int size = byteArray.size();

    if (size <= minimumSize)
    {
        return;
    }

    if (size < 1)
    {
        return;
    }
    else if (size == 1)
    {
        byteArray.removeAt(0);
        selectedCellStruct.index = -1;
        return;
    }
    else if (selectedCellStruct.index < size - 1)
    {
        byteArray.removeAt(selectedCellStruct.index);
    }
    else
    {
        byteArray.removeAt(selectedCellStruct.index);
        selectedCellStruct.index--;
        selectedCellStruct.selection = byteArray[selectedCellStruct.index].rect;
        selectedCellStruct.selection.setRight(selectedCellStruct.selection.center().x());
    }
}

void HexWidget::removePrevious()
{
    if (byteArray.size() <= minimumSize || 0 == selectedCellStruct.index)
    {
        return;
    }

    selectedCellStruct.index--;
    if (0 != selectedCellStruct.index)
    {
        byteArray.removeAt(selectedCellStruct.index);
        selectedCellStruct.selection = byteArray[selectedCellStruct.index - 1].rect;
        selectedCellStruct.selection.setRight(selectedCellStruct.selection.center().x());
        selectedCellStruct.mask = MASK::FIRST;
        goLeft();
        goLeft();
    }
}

void HexWidget::setMinimum(quint16 newMinimumSize)
{
    minimumSize = newMinimumSize;
    if (minimumSize > maximumSize)
    {
        minimumSize = maximumSize;
    }
    resizeBuffer();
}

void HexWidget::setMaximum(quint16 newMaximumSize)
{
    maximumSize = newMaximumSize;
    if (maximumSize < minimumSize)
    {
        maximumSize = minimumSize;
    }
    resizeBuffer();
}

void HexWidget::resizeBuffer()
{
    int tmp = minimumSize - byteArray.size();
    if (tmp > 0)
    {
        for (int i = 0; i < tmp; ++i)
        {
            byteArray.append(ByteRectStruct{0, getCellRect()});
        }
    }

    if (maximumSize < byteArray.size())
    {
        byteArray.resize(maximumSize);
    }
}

QByteArray HexWidget::getBuffer()
{
    QByteArray array;
    for (auto b : byteArray)
    {
        array.append((char)b.byte);
    }
    return array;
}

quint8 HexWidget::getByte(quint16 index)
{
    return byteArray[index].byte;
}

void HexWidget::setByte(quint16 index, quint8 newByte)
{
    byteArray[index].byte = newByte;
    emit onTextUpdate();
    update();
}

void HexWidget::setSingleLine(bool isSingleLine)
{
    singleLine = isSingleLine;
    update();
}

void HexWidget::setAutoResize(bool isAutoResize)
{
    autoResize = isAutoResize;
}

void HexWidget::focusOutEvent(QFocusEvent *event)
{
    selectedCellStruct.index = -1;
    emit onTextUpdate();
}

void HexWidget::onTextUpdate() {}
