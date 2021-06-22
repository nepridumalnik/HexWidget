#pragma once

#include <QtCore>

constexpr char TEXT_COLOR[] = "#000000";
constexpr char BACKGROUND_COLOR[] = "#F5F5F5";
constexpr char SELECTION_COLOR[] = "#4682B4";

constexpr int X_OFFS = 15;
constexpr int Y_OFFS = 30;

enum MASK
{
    FIRST = 0xF0u,
    SECOND = 0x0Fu
};

struct ByteRectStruct
{
    quint8 byte;
    QRect rect;
};

struct SelectedCellStruct
{
    qint32 index;
    QRect selection;
    MASK mask;
};
