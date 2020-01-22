/*Student: Ville Tiljander, 274523, ville.tiljander@tuni.fi

Hanoi game instructions:

Game is a simple mathematical game that has 3 poles and movable disks.
When starting a new game you can enter between 2 to 8 poles to play with.
There are few simple rules.

    1. Only one disk can be moved at a time.
    2. You can only move the most upper disk in a pole.
    3. No larger disk may be placed on top of a smaller disk.
    4. Game can be won if all the disks are on the middle pole.

After starting a new game (by pressing the button on the window or pressing 'r'
button on your keyboard) you can move disks by pressing the three buttons in the
game window or pressing 'asd' buttons. Button 'a' is for the most left pole, 'b'
 is for the middle pole and 'd' is for the right pole.

Pressing once selects a pole from you want to move a disk and pressing next
button either moves disk to another pole if possible and if pressing the same
poles button again disables selection.

Additional features:

1. Every pole has different color and the color of disks changes when moving to
another pole.

2. The time of a game is shown when playing the game and it can be viewed after
game is won.

3. Another method to move the disks. First is with the buttons on the game
window and the second is keyboard buttons 'asd'. */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <pole.hh>
#include <layer.hh>
#include <QMainWindow>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <vector>
#include <QPushButton>
#include <QLayout>
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>

const int HOW_MANY_POLES = 3;
const int WIDTH = 700;
const int HEIGHT = 540;
const int POLE_INTERVAL = 210;
const int GREY = 1;
const int BLACK = 0;
const int VALUE = 0;
const int MIN = 2;
const int MAX = 8;
const int STEP = 1;
const int SECOND_IN_TIMER = 1000;
const int MODIFY_X_POSITION = 100;

class Layer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // Get key presses
    void keyPressEvent(QKeyEvent* event) override;


private slots:
    // Create pop up to start game and ask user how many layers of disks to play
    void start_game_pop_up();

    // Check and move the disks in poles
    void a_pole_check();
    void b_pole_check();
    void c_pole_check();

    // Update and show game time
    void update_time();


private:
    // Pointers to widgets and layout which are used to position them in window
    Ui::MainWindow *ui;
    QVBoxLayout *layout;
    QGraphicsScene* scene_;
    QWidget *widget;

    // Create needed timer objects and values
    QTimer timer_;
    bool is_timer_running = false;
    int seconds = 0;

    // Containers where to store poles, disks and buttons
    std::vector<std::pair<Pole, std::vector<Layer*>>> poles_;
    std::vector<QPushButton*> buttons;
    std::vector<Layer*> layers_;
    
    // How many disks to play with
    unsigned int number_of_layers = 0;

    // Enum poles to numbers to simplify code when moving buttons
    enum {nothing= 3,
         A = 0,
         B = 1,
         C = 2};
    // What pole is selected
    int selected = nothing;

    const int BORDER_DOWN = 220;
    const int BORDER_RIGHT = 680;

    // Check if all disks are in the middle pole
    void is_game_won();

    // Draw new poles and discs
    void make_new_scene();
    void draw_poles();
    void draw_layers();

    // Get the change of y coordinates and return it
    int delta_y(int pole1, int pole2);

    // Check what moves are possible and disable all buttons if moving a disc to
    // that place would make an illegal move
    void check_possible_next_turn(int pole, int pole_to_check);
    void enable_buttons();
    void disable_buttons();

};

#endif // MAINWINDOW_HH
