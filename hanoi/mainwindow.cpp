#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the size of the mainwindow
    setFixedSize(WIDTH, HEIGHT);

    // Create layout and buttons and labels to shoe them in mainwindow
    layout = new QVBoxLayout;
    widget = new QWidget;

    // Create horizontal boxes and add widget to them
    QHBoxLayout *h_box = new QHBoxLayout;
    layout->addLayout(h_box);
    layout->addWidget(ui->info_label, 0, Qt::AlignCenter);
    h_box->addWidget(ui->new_game_button);
    h_box->addWidget(ui->quit_button);

    QHBoxLayout *move_button_box = new QHBoxLayout;
    // Add buttons to button vector
    buttons.push_back(ui->a_button);
    buttons.push_back(ui->b_button);
    buttons.push_back(ui->c_button);

    move_button_box->addWidget(ui->a_button);
    move_button_box->addWidget(ui->b_button);
    move_button_box->addWidget(ui->c_button);

    QHBoxLayout *show_time_box = new QHBoxLayout;
    show_time_box->addWidget(ui->lcd_min);
    show_time_box->addWidget(ui->min_label);
    show_time_box->addWidget(ui->lcd_sec);
    show_time_box->addWidget(ui->sec_label);

    // Add horizontal boxes to layout and add layout to widget
    layout->addLayout(move_button_box);
    layout->addLayout(show_time_box);
    widget->setLayout(layout);

    //disable buttons
    disable_buttons();

    // create new scene where to draw poles and blocks
    make_new_scene();

    // connect buttons to certain slots
    connect(ui->quit_button, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->new_game_button, &QPushButton::clicked, this,
                                 &MainWindow::start_game_pop_up);
    connect(ui->a_button, &QPushButton::clicked, this, &MainWindow::a_pole_check);
    connect(ui->b_button, &QPushButton::clicked, this, &MainWindow::b_pole_check);
    connect(ui->c_button, &QPushButton::clicked, this, &MainWindow::c_pole_check);
    connect(&timer_, &QTimer::timeout, this,&MainWindow::update_time);

    // Set lcd colours to grey
    ui->lcd_min->setPalette(brush_color.at(GREY));
    ui->lcd_sec->setPalette(brush_color.at(GREY));
}

MainWindow::~MainWindow()
{
    // Destroy made object pointers and set to nullpointers
    delete ui;
    delete layout;
    delete widget;
    for(auto pole : poles_){
        for(unsigned int i = 0; i < pole.second.size(); i++){
            delete pole.second.at(i);
            pole.second.at(i) = nullptr;
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
// If certain keys are pressed move blocks in game window
{
    if(buttons.at(A)->isEnabled() && event->key() == Qt::Key_A){
        a_pole_check();
    }
    if(buttons.at(B)->isEnabled() && event->key() == Qt::Key_S){
        b_pole_check();
    }
    if(buttons.at(C)->isEnabled() && event->key() == Qt::Key_D){
        c_pole_check();
    }
    if(event->key() == Qt::Key_R){
        start_game_pop_up();
    }
}

void MainWindow::start_game_pop_up()
// Pop ups a start_game window where user can enter number of layers that player 
// wants to play
{
    // Stopping time and set seconds to 0
    timer_.stop();
    seconds = 0;
    ui->lcd_min->display(seconds / 60);
    ui->lcd_sec->display(seconds % 60);



    //Create and show new popup to ask user how many blocks player wants to play
    bool is_ok = false;
    number_of_layers = QInputDialog::getInt(this,
                                            tr("Layers"),
                                            tr("Enter how many layers you want "
                                               "to play with (2-8)"),
                                            VALUE, MIN, MAX, STEP, &is_ok);
    if(is_ok){
        // clear vectors from variables to add new ones to them
        layers_.clear();
        poles_.clear();

        // delete scene to draw new objects to new scene_ which will be created later
        scene_->deleteLater();

        // If user cliks ok button make new scene and start timer
        make_new_scene();
        draw_poles();
        draw_layers();
        is_timer_running = true;
        timer_.start(SECOND_IN_TIMER);
        enable_buttons();
    }
}

void MainWindow::a_pole_check()
// Move blocks in the game window
{  
    if(!poles_.at(A).second.empty() && selected == nothing){
        // If nothing is selected set A selected
        ui->info_label->setText("A selected");
        selected = A;
        check_possible_next_turn(A, B);
        check_possible_next_turn(A, C);
    }
    else{
        if(!poles_.at(B).second.empty() && selected == B){
            // Move from B pole to A pole

            layers_.at(poles_.at(B).second.back()->get_number())->moveBy(
                        -POLE_INTERVAL, delta_y(B, A));

            // Move disk (layer) to new position and add poles vector at A moved
            // disks information and remove information from poles vector at B
            poles_.at(A).second.push_back(layers_.at(poles_.at(B).second.back()
                                                     ->get_number()));
            poles_.at(B).second.pop_back();
        }
        if(!poles_.at(C).second.empty() && selected == C){
            // Move from C pole to A pole
            layers_.at(poles_.at(C).second.back()->get_number())->moveBy(
                        -POLE_INTERVAL * 2, delta_y(C, A));
            poles_.at(A).second.push_back(layers_.at(poles_.at(C).second.back()
                                                     ->get_number()));
            poles_.at(C).second.pop_back();
        }
        ui->info_label->setText("Nothing selected");
        selected = nothing;
        enable_buttons();
        // Check if game has been won and end the game then
        is_game_won();
    }
}

void MainWindow::b_pole_check()
// Works almost the same way than a_pole_check. Moving blocks works differently
{
    if(!poles_.at(B).second.empty() && selected == nothing){
        ui->info_label->setText("B selected");
        selected = B;
        check_possible_next_turn(B, A);
        check_possible_next_turn(B, C);
    }
    else{
        if(!poles_.at(A).second.empty() && selected == A){
            layers_.at(poles_.at(A).second.back()->get_number())->moveBy(
                        POLE_INTERVAL , delta_y(A, B));
            poles_.at(B).second.push_back(layers_.at(poles_.at(A).second.back()
                                                     ->get_number()));
            poles_.at(A).second.pop_back();
        }
        if(!poles_.at(C).second.empty() && selected == C){
            layers_.at(poles_.at(C).second.back()->get_number())->moveBy(
                        -POLE_INTERVAL, delta_y(C, B));
            poles_.at(B).second.push_back(layers_.at(poles_.at(C).second.back()
                                                     ->get_number()));
            poles_.at(C).second.pop_back();
        }
        selected = nothing;
        ui->info_label->setText("Nothing selected");
        enable_buttons();
        is_game_won();
    }
}

void MainWindow::c_pole_check()
// Works like a_pole_check but moving disk is different
{
    if(!poles_.at(C).second.empty() && selected == nothing){
        ui->info_label->setText("C selected");
        selected = C;
        check_possible_next_turn(C, A);
        check_possible_next_turn(C, B);
    }
    else{
        if(!poles_.at(A).second.empty() && selected == A){

            layers_.at(poles_.at(A).second.back()->get_number())->moveBy(
                        POLE_INTERVAL * 2, delta_y(A, C));
            poles_.at(C).second.push_back(layers_.at(poles_.at(A).second.back()
                                                     ->get_number()));
            poles_.at(A).second.pop_back();
        }
        if(!poles_.at(B).second.empty() && selected == B){

            layers_.at(poles_.at(B).second.back()->get_number())->moveBy(
                        POLE_INTERVAL, delta_y(B, C));
            poles_.at(C).second.push_back(layers_.at(poles_.at(B).second.back()
                                                     ->get_number()));
            poles_.at(B).second.pop_back();
        }

        ui->info_label->setText("Nothing selected");
        selected = nothing;
        enable_buttons();
        is_game_won();
    }
}

void MainWindow::update_time()
// Update timer if game is running and show time in minutes and seconds.
{
    if(is_timer_running){
        seconds++;
        ui->lcd_min->display(seconds / 60);
        ui->lcd_sec->display(seconds % 60);
    }
}

void MainWindow::is_game_won()
{
    // Check if second pole has the amount of layers when beginning of the game
    if(poles_.at(B).second.size() == number_of_layers){
        // Stop timer and create messagebox that asks if player
        // wants to start a new game
        timer_.stop();
        seconds = 0;
        int game_over_message_box = 0;
        disable_buttons();
        game_over_message_box = QMessageBox::question(
                                        0, "Game won!",
                                        "Do you want to start a new game?",
                                        QMessageBox::Yes, QMessageBox::No);

        if (game_over_message_box == QMessageBox::Yes) {
            // If player wishes to play again launch star_game popup
            start_game_pop_up();
        }
    }
}

void MainWindow::make_new_scene()
// Got code from example code
{
    scene_ = new QGraphicsScene(this);
    int left_margin = 10; // x coordinate
    int top_margin = 270; // y coordinate
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                    BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);
    layout->addWidget(ui->graphicsView);
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);
    setCentralWidget(widget);
}

void MainWindow::draw_poles()
// Draw poles to the scene
{
    // First poles x_coord
    int x_line = POLE_INTERVAL - MODIFY_X_POSITION;

    for(int i = 0; i < HOW_MANY_POLES; i++){
        // Make and draw all pole objects
        Pole pole(x_line, scene_->height());
        poles_.push_back({pole, {}});
        x_line = x_line + POLE_INTERVAL;
        scene_->addRect(poles_.at(i).first.draw(), (brush_color.at(BLACK)),
                        brush_color.at(i));
    }
}

void MainWindow::draw_layers()
// Draw disks (layers) to the first pole
{
    // adjust layers coordinates and sizes
    int y = scene_->height() - HEIGHT_LAYER;
    int width = number_of_layers * WIDTH_LAYER;
    // Get x coordinate with using poles coordinates in game window and adjust
    // it with layers mid point
    int x = poles_.at(A).first.get_x_coordinate() - width / 2;
    for(unsigned int i = 0; i < number_of_layers; i++){
        // Create new layer object and add them to containers
        Layer* layer_ = new Layer(x, y, width, HEIGHT_LAYER, i);
        layers_.push_back(layer_);
        poles_.at(A).second.push_back(layer_);
        // Adjust next coordinates to next layer
        x = x + WIDTH_LAYER / 2;
        y = y - HEIGHT_LAYER;
        width = width - WIDTH_LAYER;
        scene_->addItem(layer_);
    }
}

int MainWindow::delta_y(int pole1, int pole2)
{
    // Get the wanted y coordinate change when moving objects
    return (poles_.at(pole1).second.size() * HEIGHT_LAYER -
            poles_.at(pole2).second.size() * HEIGHT_LAYER) - HEIGHT_LAYER;
}

void MainWindow::check_possible_next_turn(int pole, int pole_to_check)
// Check what moves can be done next, if cannot be done disable that poles button
// so illegal moves can't happen
{
    if(!poles_.at(pole_to_check).second.empty()){
        // Check if pole which is going to checked is not empty
        if(poles_.at(pole).second.back()->get_width() >
                poles_.at(pole_to_check).second.back()->get_width()){
            // Set button disabled if layers width is smaller than the one that
            // is going to be moved
            buttons.at(pole_to_check)->setDisabled(true);
        }
    }
}

void MainWindow::enable_buttons()
{
    for(auto button : buttons){
        button->setEnabled(true);
    }
}

void MainWindow::disable_buttons()
{
    for(auto button : buttons){
        button->setDisabled(true);
    }
}
