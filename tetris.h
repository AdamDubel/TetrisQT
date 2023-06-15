#ifndef TETRIS_H
#define TETRIS_H

#include <QWidget>

class Tetris : public QWidget
{
    Q_OBJECT

public:
    Tetris(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    void initGame();
    void drawSquare(QPainter &painter, int x, int y, int shape);
    bool tryMove(int newShape, int newX, int newY);
    void rotateShape();
    enum { BoardWidth = 10, BoardHeight = 30 };

    int timerId;
    int board[BoardWidth * BoardHeight];
    int currentX;
    int currentY;
    int currentShape;
    int nextShape;
    int squareWidth() { return contentsRect().width() / BoardWidth; }
    int squareHeight() { return contentsRect().height() / BoardHeight; }
int tickCounter;
    static const int shapes[7][4][2];
};

#endif // TETRIS_H
