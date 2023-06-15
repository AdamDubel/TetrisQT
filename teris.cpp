#include "tetris.h"
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
int squareHeight=1;
int squareWidth=1;
const int Tetris::shapes[7][4][2] = {
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},  // kwadrat
    {{0, -1}, {0, 0}, {0, 1}, {0, 2}}, // linia
    {{0, -1}, {0, 0}, {0, 1}, {1, 1}}, // L
    {{0, -1}, {0, 0}, {0, 1}, {-1, 1}}, // odwrócony L
    {{0, 0}, {1, 0}, {0, 1}, {-1, 1}}, // S
    {{0, 0}, {-1, 0}, {0, 1}, {1, 1}}, // Z
    {{0, -1}, {-1, 0}, {0, 0}, {1, 0}} // T
};

Tetris::Tetris(QWidget *parent) : QWidget(parent)
{
    setFixedSize(300, 600);

    initGame();
}

void Tetris::initGame()
{
    timerId = startTimer(100);
    currentX = 0;
    currentY = 0;
    currentShape = 0;
    nextShape = rand() % 7;

    for (int i = 0; i < BoardWidth * BoardHeight; ++i)
        board[i] = 0;
}
void Tetris::rotateShape()
{
    int newShape = (currentShape + 1) % 4; // Oblicz nowy indeks kształtu (obracanie zgodnie z ruchem wskazówek zegara)

    if (tryMove(newShape, currentX, currentY)) {
        currentShape = newShape;
        update();
    }
}

void Tetris::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QRect rect = contentsRect();
    int boardTop = rect.bottom() - BoardHeight * squareHeight();

    // Rysowanie planszy
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            int shape = board[i * BoardWidth + j];
            if (shape != 0)
                drawSquare(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape);
        }
    }

    // Rysowanie aktualnego kształtu
    if (currentShape != 0) {
        for (int i = 0; i < 4; ++i) {
            int x = currentX + shapes[currentShape][i][0];
            int y = currentY + shapes[currentShape][i][1];
            drawSquare(painter, rect.left() + x * squareWidth(),
                       boardTop + y * squareHeight(), currentShape);
        }
    }
}
bool Tetris::tryMove(int newShape, int newX, int newY)
{
    for (int i = 0; i < 4; ++i) {
        int x = newX + shapes[newShape][i][0];
        int y = newY + shapes[newShape][i][1];

        // Sprawdź, czy kształt wychodzi poza granice planszy lub
        // czy kolizuje z innymi już umieszczonymi kształtami
        if (x < 0 || x >= BoardWidth || y >= BoardHeight || board[y * BoardWidth + x] != 0)
            return false;
    }

    return true;
}



void Tetris::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {
        // Przesuń kształt w dół
        if (tryMove(currentShape, currentX, currentY + 1))
            currentY++;
        else {
            // Kształt dotarł na dół, dodaj go do planszy
            for (int i = 0; i < 4; ++i) {
                int x = currentX + shapes[currentShape][i][0];
                int y = currentY + shapes[currentShape][i][1];
                board[y * BoardWidth + x] = currentShape;
            }

            // Sprawdź, czy można usunąć pełne linie
            // (implementacja tej funkcjonalności nie jest zawarta w tym kodzie)

            // Wygeneruj nowy kształt
            currentShape = nextShape;
            nextShape = rand() % 7;
            currentX = BoardWidth / 2;
            currentY = 0;

            // Sprawdź, czy nowy kształt może się poruszać w dół
            if (!tryMove(currentShape, currentX, currentY)) {
                // Gra się zakończyła
                killTimer(timerId);
                timerId = 0;
            }
        }

        // Odśwież widok
        update();

        // Inkrementuj licznik ticków
        tickCounter++;
    } else {
        QWidget::timerEvent(event);
    }
}




void Tetris::drawSquare(QPainter &painter, int x, int y, int shape)
{
    static constexpr QRgb colorTable[8] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
    };

    QColor color = colorTable[shape];
    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2, color);
    painter.setPen(color.lighter());
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);
    painter.setPen(color.darker());
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}

void Tetris::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        if (tryMove(currentShape, currentX - 1, currentY))
            currentX--;
        break;
    case Qt::Key_Right:
        if (tryMove(currentShape, currentX + 1, currentY))
            currentX++;
        break;
    case Qt::Key_Down:
        if (tryMove(currentShape, currentX, currentY + 1))
            currentY++;
        break;
    case Qt::Key_Space:
        // Obróć kształt
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}
