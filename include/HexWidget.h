#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetrics>
#include <QApplication>

#include "Structures.h"

#define SYMBOL 'C'

class HexWidget : public QWidget
{
public:
    explicit HexWidget(QWidget *parent = nullptr);

    ~HexWidget() override;

    void clearBuffer();
    QByteArray getBuffer();
    int getBufferSize() const;
    quint8 getByte(quint16 index);
    void resizeText(QSize newSize);
    void setCharColor(const char *color);
    void setWidgetFont(const QFont &font);
    void setSingleLine(bool isSingleLine);
    void setAutoResize(bool isAutoResize);
    void setMaximum(quint16 newMaximumSize);
    void setMinimum(quint16 newMinimumSize);
    void setSelectionColor(const char *color);
    void setBackgroundColor(const char *color);
    void setByte(quint16 index, quint8 newByte);
    void setBuffer(const QByteArray &setByteArray);
    void appendBuffer(const QByteArray &appendByteArray);
    void prependBuffer(const QByteArray &prependByteArray);

signals:
    void onTextUpdate();

private:
    QFont appFont;
    QColor charColor;
    int rowWidth = 0;
    int columnOffset = 0;
    QColor selectionColor;
    int columnNumber = 16;
    QColor backgroundColor;
    bool singleLine = false;
    bool autoResize = true;
    quint16 minimumSize = 0;
    QFontMetrics *fm = nullptr;
    quint16 maximumSize = UINT16_MAX;
    QVector<ByteRectStruct> byteArray{};
    SelectedCellStruct selectedCellStruct{};

    const QPoint startHexPoint = QPoint(X_OFFS, Y_OFFS);
    QPoint cursor;

    void goUp();
    void goLeft();
    void goDown();
    void goRight();
    void selection();
    void newColumn();
    void resizeBuffer();
    QRect getCellRect();
    void removeCurrent();
    void insertNewByte();
    void removePrevious();
    void newRow(QPoint point);
    void resetByteValue(int key);
    void drawSelection(QPainter *painter);
    void focusOutEvent(QFocusEvent *event);
    void setSelectionCell(int i, MASK mask);
    void drawRows(QPainter *painter = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};
