#pragma once

#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

#include "Structures.h"

class HexWidget : public QWidget {
public:
    explicit HexWidget(QWidget *parent = nullptr);

    void prependBuffer(const QByteArray &prependByteArray);

    void appendBuffer(const QByteArray &appendByteArray);

    void setBuffer(const QByteArray &setByteArray);

    auto getBufferSize() const;

    auto getBuffer() const;

    void clearBuffer();

private:
    SelectedCellStruct_t selectedCellStruct{};
    QVector<ByteRectStruct_t> byteArray{};
    quint64 columnNumber = 16;
    quint8 rowOffset = 22;
    quint8 columnOffset = 22;
    QFont font;

    const QPoint startOffsetPoint = QPoint(20, 50);
    const QPoint startHexPoint = QPoint(75, 50);
    QPoint cursor;

    void newColumn();

    void newRow(QPoint point);

    void drawSelection(QPainter &painter);

    void paintEvent(QPaintEvent *event) override;

    void drawHeader(QPainter &painter);

    void drawRows(QPainter &painter);

    void drawAscii(QPainter &painter);

    void mousePressEvent(QMouseEvent *event) override;

    void selection();

    void keyPressEvent(QKeyEvent *event) override;

    void goRight();

    void goLeft();

    void goUp();

    void goDown();
};