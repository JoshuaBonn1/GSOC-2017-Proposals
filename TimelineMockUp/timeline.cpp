#include "timeline.h"
#include "ui_timeline.h"
#include <QRect>
#include <QPainter>
#include <QPoint>
#include <QSize>
#include <QLine>
#include <QVector>
#include <QColor>
#include <QDebug>

Timeline::Timeline(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Timeline)
{
    ui->setupUi(this);
}

Timeline::~Timeline()
{
    delete ui;
}

void Timeline::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
        if(event->angleDelta().y() > 0){
            gridWidth++;
            gridHeight++;
        }
        else{
            gridWidth--;
            gridHeight--;
        }

        if(gridHeight < 2) gridHeight = 2;
        if(gridWidth < 2) gridWidth = 2;
        if(gridHeight > 30) gridHeight = 30;
        if(gridWidth > 30) gridWidth = 30;
        update();
    }
}

QRect Timeline::generateRect(QRect grid, int instrument, int measure, int length, int gridWidth, int gridHeight, int meta, int text){
    //if(play1_meas < 1 || play1_meas > grid.right() / gridWidth)

    int x = (measure - 1) * gridWidth + grid.left() + (text + 1) * gridWidth;
    int y = (instrument - 1) * gridHeight + grid.top();
    int width = length * gridWidth;

    if(width + x > grid.right()) width = grid.right() - x + 1;

    y += (meta + 1) * gridHeight;
    QRect box(x, y, width, gridHeight);

    return box;
}

void Timeline::paintEvent(QPaintEvent *event){
    //Number of meta rows
    int meta = 3;

    //Location of grid on window
    QPoint* topLeftGridPoint = new QPoint(50, 100);

    //Size of grid
    QSize* gridSize = new QSize(800, 300);
    QRect grid = QRect(*topLeftGridPoint, *gridSize);

    QPoint top = grid.topLeft();
    QPoint bottom = grid.bottomLeft();
    QPoint left = grid.topLeft();
    QPoint right = grid.topRight();



    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);

    //Draw grid box
    painter.drawRect(grid);

    //Draw some instruments
    QVector <QRect> boxes;
    for(int i = 0; i < int(grid.height()/gridHeight)-meta-1; i++){
        boxes.push_back(generateRect(grid, i%(int(grid.height()/gridHeight)-meta-1) + 1, rand()%(int(grid.width()/gridWidth) - 3) + 1, rand()%10 + 1, gridWidth, gridHeight, meta, 3));
    }
    QPainterPath instruments;
    for(int i = 0; i < int(grid.height()/gridHeight)-meta-1; i++){
        instruments.addRoundedRect(boxes[0], (gridHeight/3), (gridHeight/3));
        boxes.pop_front();
    }
    QPen pen(QColor(100, 100, 100), 1);
    painter.setPen(pen);
    painter.fillPath(instruments, QColor(100, 100, 100));



    top.setX(grid.left() + 3 * gridWidth);
    bottom.setX(grid.left() + 3 * gridWidth);
    bottom.setY(grid.top() + meta * gridHeight);

    //Draw meta rows vertical lines
    for(int i = top.x(); i + gridWidth < grid.right(); i += gridWidth){
        top.setX(top.x() + gridWidth);
        bottom.setX(bottom.x() + gridWidth);
        painter.setPen(QColor(5, 5, 5));
        painter.drawLine(top, bottom);
    }

    top = grid.topLeft();
    bottom = grid.bottomLeft();
    top.setX(grid.left() + 3 * gridWidth);
    top.setY(grid.top() + (meta + 1) * gridHeight);
    bottom = grid.bottomLeft();
    bottom.setX(grid.left() + 3 * gridWidth);

    //Fill text
    QFont font = painter.font();
    font.setPointSize(gridHeight / 2);
    painter.setFont(font);

    //Draw tempo change
    QPainterPath path;
    path.setFillRule( Qt::WindingFill );
    QPoint tempo_tl = grid.topLeft();
    tempo_tl.setX(tempo_tl.x() + gridWidth * 4);
    QPoint tempo_br = tempo_tl;
    tempo_br.setY(tempo_br.y() + gridHeight - 1);
    tempo_br.setX(tempo_br.x() + gridWidth * 3);
    QRect tempo = QRect(tempo_tl, tempo_br);
    path.addRoundedRect( tempo, (gridHeight/2.5), (gridHeight/2.5) );
    path.addRect( QRect(tempo_tl.x(), tempo_tl.y(), gridHeight/2, gridHeight/2) ); // Top right corner not rounded
    path.addRect( QRect(tempo_tl.x(), tempo_tl.y() + gridHeight/2, gridHeight/2, gridHeight/2) ); // Top right corner not rounded
    painter.fillPath( path.simplified(), QColor(170, 170, 170)); // Only Top left & bottom right corner rounded
    painter.drawPath( path.simplified() );
    tempo.setLeft(tempo.left() + gridWidth / 4);
    painter.drawText(tempo, tr("Allegro"));

    //Draw all vertical lines
    for(int i = top.x(); i + gridWidth < grid.right(); i += gridWidth){
        top.setX(top.x() + gridWidth);
        bottom.setX(bottom.x() + gridWidth);
        painter.setPen(QColor(5, 5, 5));
        painter.drawLine(top, bottom);
    }

    //Draw all horizontal lines
    for(int i = grid.top(); i + gridHeight < grid.bottom(); i += gridHeight){
        left.setY(left.y() + gridHeight);
        right.setY(right.y() + gridHeight);
        painter.setPen(QColor(150, 150, 150));
        painter.drawLine(left, right);
    }


    //Tempo
    painter.setPen(Qt::black);
    QPoint text = grid.topLeft();
    text.setY(text.y() + gridHeight * .75);
    text.setX(text.x() + gridWidth / 2);
    painter.drawText(text, tr("Tempo"));
    //Key
    text.setY(text.y() + gridHeight);
    painter.drawText(text, tr("Key Sig"));
    //Time
    text.setY(text.y() + gridHeight);
    painter.drawText(text, tr("Time Sig"));
    //Measure
    text.setY(text.y() + gridHeight);
    painter.drawText(text, tr("Measures"));
    //Draw measure numbers
    int original_x = text.x();
    //Increment for initial number
    text.setX(text.x() + gridWidth*4 - 5);
    int start = 1;
    for(int i = start-1; text.x() < grid.right(); i += 5){
        if(i == start-1){
            painter.drawText(text, QString::number(start));
            text.setX(text.x() + gridWidth * 4);
        }
        else{
            painter.drawText(text, QString::number(i));
            text.setX(text.x() + gridWidth * 5);
        }
    }
    //text.setX(text.x() + gridWidth * 5);
    text.setX(original_x);

    //Instruments
    QString instr[] = {tr("Piano"),
                       tr("Flute"),
                       tr("Flute II"),
                       tr("Guitar"),
                       tr("Soprano"),
                       tr("Alto"),
                       tr("Tenor"),
                       tr("Bass"),
                       tr("Drums"),
                       tr("Violin"),
                       tr("Cello"),};
    for(unsigned int i = 0; i < (sizeof(instr)/sizeof(*instr)); i++){
        text.setY(text.y() + gridHeight);
        if(text.y() > grid.bottom()) break;
        painter.drawText(text, instr[i]);
    }


}
