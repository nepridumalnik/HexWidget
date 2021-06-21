#pragma once

#include <QtGlobal>

constexpr char TEXT_COLOR[] = "#000000";
constexpr char BACKGROUND_COLOR[] = "#F5F5F5";
constexpr char SELECTION_COLOR[] = "#4682B4";

constexpr int X_OFFS = 10;
constexpr int Y_OFFS = 30;

enum MASK {
    FIRST = 0xF0u,
    SECOND = 0x0Fu
};

enum BYTE_VALUE {
    ZERO = 0x00u,
    ONE = 0x11u,
    TWO = 0x22u,
    THREE = 0x33u,
    FOUR = 0x44u,
    FIVE = 0x55u,
    SIX = 0x66u,
    SEVEN = 0x77u,
    EIGHT = 0x88u,
    NINE = 0x99u,
    A = 0xAAu,
    B = 0xBBu,
    C = 0xCCu,
    D = 0xDDu,
    E = 0xEEu,
    F = 0xFFu
};

struct ByteRectStruct {
    quint8 byte = 0;
    QRect rect{};
};

struct SelectedCellStruct {
    qint32 index = -1;
    QRect selection{};
    MASK mask = MASK::FIRST;
};
