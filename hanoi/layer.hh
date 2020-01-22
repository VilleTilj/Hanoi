#ifndef LAYER_HH
#define LAYER_HH

#include <QRectF>
#include <QGraphicsObject>
#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QGraphicsItem>
#include <QColor>

const int WIDTH_LAYER = 30;
const int HEIGHT_LAYER = 20;
// Brush color vector to paint layers colors
const std::vector<QColor> brush_color = {(Qt::black), (Qt::gray), (Qt::yellow),
                                         (Qt::red), (Qt::blue), (Qt::green)};

class Layer : public QGraphicsObject
{
public:
    Layer();
    Layer(int x, int y, int width, int height, int number);
    ~Layer();

    // Return rectangular layer object
    QRectF boundingRect() const;

    // return width of a layer
    int get_width();

    // Return index number of a layer
    int get_number();

    // Paint a layers randomly
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = Q_NULLPTR);

private:
    QColor color;
    int x_;
    int y_;
    int width_;
    int height_;
    int number_;
};

#endif // LAYER_HH
