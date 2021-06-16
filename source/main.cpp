#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QDebug>

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

class HexWidget : public QWidget {
public:
    explicit HexWidget(QWidget *parent = nullptr) : QWidget(parent) {
        QPalette pal(palette());
        pal.setColor(QPalette::Background, Qt::white);
        setAutoFillBackground(true);
        setPalette(pal);

        font = property("font").value<QFont>();
        font.setPixelSize(14);
        setFont(font);
    }

    void prependBuffer(const QByteArray &prependByteArray) {
        QRect rect;
        int pixelSize = font.pixelSize() * 2;
        rect.setSize(QSize(pixelSize, pixelSize));
        for (auto b : prependByteArray) {
            byteArray.prepend(ByteRectStruct{b, rect});
        }

        update();
    }

    void appendBuffer(const QByteArray &appendByteArray) {
        QRect rect;
        int pixelSize = font.pixelSize() * 2;
        rect.setSize(QSize(pixelSize, pixelSize));
        for (auto b : appendByteArray) {
            byteArray.append(ByteRectStruct{b, rect});
        }
        update();
    }

    void setBuffer(const QByteArray &setByteArray) {
        byteArray.clear();
        appendBuffer(setByteArray);
    }

    auto getBufferSize() const {
        return byteArray.size();
    }

    auto getBuffer() const {
        return byteArray.data();
    }

    void clearBuffer() {
        byteArray.clear();
    }

private:
    SelectedCellStruct selectedCellStruct;
    QVector<ByteRectStruct> byteArray;
    quint64 columnNumber = 16;
    quint8 rowOffset = 22;
    quint8 columnOffset = 22;
    QFont font;

    const QPoint startOffsetPoint = QPoint(20, 50);
    const QPoint startHexPoint = QPoint(75, 50);
    QPoint cursor;

    void newColumn() {
        cursor.setX(cursor.x() + columnOffset + font.pixelSize());
    }

    void newRow(QPoint point) {
        cursor.setY(cursor.y() + rowOffset + font.pixelSize());
        cursor.setX(point.x());
    }

    void drawSelection(QPainter &painter) {
        if (selectedCellStruct.index == -1) {
            return;
        }

        QRect blackRect = selectedCellStruct.selection;
        blackRect.setRight(blackRect.x() + blackRect.width() - 1);
        painter.drawRect(blackRect);
        painter.fillRect(selectedCellStruct.selection, Qt::blue);
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.setFont(QFont("monospace", font.pixelSize()));

        drawSelection(painter);
        drawHeader(painter);
        drawRows(painter);
        drawAscii(painter);
    }

    void drawHeader(QPainter &painter) {

    }

    void drawRows(QPainter &painter) {
        cursor = startHexPoint;
        quint64 counter = 0;

        for (auto &i : byteArray) {
            i.rect.moveBottomLeft(cursor);
            painter.drawText(cursor, QString::number(i.byte, 16).toUpper());
            counter++;

            newColumn();

            if (counter == columnNumber) {
                counter = 0;
                newRow(startHexPoint);
            }
        }
    }

    void drawAscii(QPainter &painter) {}

    void mousePressEvent(QMouseEvent *event) override {
        int key = event->button();

        if (key == Qt::MouseButton::LeftButton) {
            selection();
        }

        update();
    }

    void selection() {
        QPoint cursorPos = QCursor::pos();
        cursorPos = this->mapFromGlobal(cursorPos);
        selectedCellStruct.index = -1;

        for (int i = 0; i < byteArray.size(); ++i) {
            if (byteArray[i].rect.contains(cursorPos)) {
                selectedCellStruct.index = i;
                selectedCellStruct.selection = byteArray[i].rect;

                int centerX = byteArray[i].rect.center().x();

                if (cursorPos.x() < centerX) {
                    selectedCellStruct.selection.setRight(centerX);
                    selectedCellStruct.mask = MASK::FIRST;
                } else {
                    selectedCellStruct.selection.setLeft(centerX);
                    selectedCellStruct.mask = MASK::SECOND;
                }

                break;
            }
        }
    }

    void keyPressEvent(QKeyEvent *event) override {
        qDebug() << __FUNCTION__;
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    HexWidget hexWidget;
    hexWidget.show();
    hexWidget.setBuffer(QByteArray("Some test data, another test data"));
    hexWidget.resize(750, 480);

    return QApplication::exec();
}
