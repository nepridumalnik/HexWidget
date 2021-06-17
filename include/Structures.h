#pragma once

#define and &&
#define or ||

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
