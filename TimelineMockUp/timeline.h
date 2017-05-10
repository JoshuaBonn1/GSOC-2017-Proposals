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

private:
    Ui::Timeline *ui;
    //Width of grid cell
    int gridWidth = 20;

    //Height of grid cell
    int gridHeight = 20;
};

#endif // TIMELINE_H
