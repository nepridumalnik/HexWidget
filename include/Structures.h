#pragma once

#define and &&
#define or ||

enum MASK {
    FIRST = 0xF0,
    SECOND = 0x0F
};

typedef struct ByteRectStruct_t {
    char byte = 0;
    QRect rect;
} ByteRectStruct;

typedef struct SelectedCellStruct_t {
    qint32 index = -1;
    QRect selection;
    MASK mask = MASK::FIRST;
} SelectedCellStruct;
