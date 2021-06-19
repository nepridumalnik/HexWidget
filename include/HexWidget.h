#pragma once

#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetrics>
#include <QApplication>

#include "Structures.h"

#define SYMBOL "C"

class HexWidget : public QWidget {
public:
    explicit HexWidget(QWidget *parent = nullptr);

    ~HexWidget() override;

    void prependBuffer(const QByteArray &prependByteArray);

    void appendBuffer(const QByteArray &appendByteArray);

    void setBuffer(const QByteArray &setByteArray);

    auto getBufferSize() const;

    auto getBuffer() const;

    void clearBuffer();

    void setMaximum(uint64_t newMinimumSize);

    void setMinimum(uint64_t newMaximumSize);

private:
    SelectedCellStruct selectedCellStruct{};
    QVector<ByteRectStruct> byteArray{};
    int columnNumber = 16;
    int rowWidth = 0;
    int columnOffset = 0;
    uint64_t minimumSize = 0;
    uint64_t maximumSize = UINT64_MAX;
    QFont appFont;
    QFontMetrics *fm = nullptr;
    QColor selectionColor;
    QColor charColor;
    QColor backgroundColor;
    QPainter *painter = nullptr;

    const QPoint startOffsetPoint = QPoint(20, 50);
    const QPoint startHexPoint = QPoint(75, 50);
    QPoint cursor;

    void newColumn();

    void newRow(QPoint point);

    void drawSelection();

    void paintEvent(QPaintEvent *event) override;

    void drawHeader();

    void drawRows();

    void drawAscii();

    void mousePressEvent(QMouseEvent *event) override;

    void selection();

    void setSelectionCell(int i, MASK mask);

    void keyPressEvent(QKeyEvent *event) override;

    void goRight();

    void goLeft();

    void goUp();

    void goDown();

    void setFont(const QFont &font);

    void setSelectionColor(const char *color);

    void setCharColor(const char *color);

    void setBackgroundColor(const char *color);

    QRect getCellRect();

    void resetByteValue(int key);

    void insertNewByte();

    void removePrevious();

    void removeCurrent();
};
