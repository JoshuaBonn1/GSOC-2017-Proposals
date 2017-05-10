#ifndef TIMELINE_H
#define TIMELINE_H

#include <QMainWindow>
#include <QPaintEvent>

namespace Ui {
class Timeline;
}

class Timeline : public QMainWindow
{
    Q_OBJECT

public:
    explicit Timeline(QWidget *parent = 0);
    ~Timeline();
    void paintEvent(QPaintEvent *event);
    QRect generateRect(QRect grid, int instrument, int measure, int length, int gridWidth, int gridHeight, int meta, int text);
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::Timeline *ui;
    //Width of grid cell
    int gridWidth = 20;

    //Height of grid cell
    int gridHeight = 20;

    //Instruments
    QString instr[11] = {tr("Piano"),
                       tr("Flute"),
                       tr("Flute II"),
                       tr("Guitar"),
                       tr("Soprano"),
                       tr("Alto"),
                       tr("Tenor"),
                       tr("Bass"),
                       tr("Drums"),
                       tr("Violin"),
                       tr("Cello")};
    QRect grid;

    //Number of meta rows
    int meta = 3;

    //Cell (x, y) for selection
    QPoint selection_cell;
    QPoint selection_size = QPoint(1, 1);
};

#endif // TIMELINE_H
