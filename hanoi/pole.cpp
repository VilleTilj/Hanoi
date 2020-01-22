#include "pole.hh"

Pole::Pole(int x_cord, int y_cord):
  x(x_cord)
{
    QRectF graph(x_cord, y_cord, WIDTH_POLE, HEIGHT_POLE);
    graphic_ = graph;

}

Pole::~Pole()
{

}

QRectF Pole::draw()
{
    return graphic_;
}

int Pole::get_x_coordinate()
{
    return x + WIDTH_POLE / 2;
}
