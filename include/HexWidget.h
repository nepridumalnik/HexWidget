#pragma once

#include <stdexcept>

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

    void setMaximum(quint16 newMinimumSize);

    void setMinimum(quint16 newMaximumSize);

    quint8 getByte(quint16 index);

    void setByte(quint16 index, quint8 newByte);

    void resizeText(QSize newSize);

    QByteArray getBuffer();

signals:
    void onTextUpdate();

private:
    SelectedCellStruct selectedCellStruct{};
    QVector<ByteRectStruct> byteArray{};
    int columnNumber = 16;
    int rowWidth = 0;
    int columnOffset = 0;
    quint16 minimumSize = 0;
    quint16 maximumSize = UINT16_MAX;
    QFont appFont;
    QFontMetrics *fm = nullptr;
    QColor selectionColor;
    QColor charColor;
    QColor backgroundColor;

    const QPoint startHexPoint = QPoint(X_OFFS, Y_OFFS);
    QPoint cursor;

    void newColumn();

    void newRow(QPoint point);

    void drawSelection(QPainter *painter);

    void paintEvent(QPaintEvent *event) override;

    void drawRows(QPainter *painter = nullptr);

    void mousePressEvent(QMouseEvent *event) override;

    void selection();

    void setSelectionCell(int i, MASK mask);

    void keyPressEvent(QKeyEvent *event) override;

    void goRight();

    void goLeft();

    void goUp();

    void goDown();

    void setWidgetFont(const QFont &font);

    void setSelectionColor(const char *color);

    void setCharColor(const char *color);

    void setBackgroundColor(const char *color);

    QRect getCellRect();

    void resetByteValue(int key);

    void insertNewByte();

    void removePrevious();

    void removeCurrent();

    void resizeBuffer();
};
