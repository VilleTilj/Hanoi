#ifndef POLE_HH
#define POLE_HH
#include <QRectF>

const int WIDTH_POLE = 20;
const int HEIGHT_POLE = -150;

class Pole
{
public:
    Pole(int x_cord, int y_cord);
    ~Pole();

    // Return rectangular pole object to paint it to scene
    QRectF draw();

    // return poles center x coordinate
    int get_x_coordinate();

private:
    QRectF graphic_; // Make rect object to draw pole
    int x;
};

#endif // POLE_HH
