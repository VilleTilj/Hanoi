#include "layer.hh"

Layer::Layer(int x, int y, int width, int height, int number):
    QGraphicsObject(nullptr),
    color(nullptr),
    x_(x),
    y_(y),
    width_(width),
    height_(height),
    number_(number)
{

}

Layer::~Layer()
{

}

QRectF Layer::boundingRect() const
{
    return QRectF(x_, y_, width_, height_);
}

int Layer::get_width()
{
    return width_;
}

int Layer::get_number()
{
    return number_;
}

void Layer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // Make a random number generator and randomly select color for layer
    int random_number = rand() % 5 + 1;
    painter->setBrush(brush_color.at(random_number));
    color = brush_color.at(random_number);
    painter->drawRoundedRect(boundingRect(), 1, 1);
}

