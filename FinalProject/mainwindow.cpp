#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup(){
    state = POWEREDOFF;
    ui->progressBar->setValue(0);

    update_ui();
}

void MainWindow::update_ui(){
    ui->selection_arrow1->setVisible(false);
    ui->selection_arrow2->setVisible(false);
    ui->selection_arrow3->setVisible(false);

    ui->light_red->setVisible(false);
    ui->light_green->setVisible(false);
    ui->light_blue->setVisible(false);

    ui->progressBar->setVisible(false);
    ui->screen_text->setVisible(false);
    ui->timer_label->setVisible(false);

    ui->electrodes_button->setVisible(true);
    ui->battery_percentage_label->setVisible(true);

    ui->date_label->setVisible(false);
    ui->time_label->setVisible(false);

    switch (state){
        case POWEREDOFF:
            ui->battery_percentage_label->setVisible(false);
            return;
        case MENU:
            ui->screen_text->setVisible(true);
            ui->battery_percentage_label->setVisible(true);

            if(current_selection == 0){
                ui->selection_arrow1->setVisible(true);
            }else if(current_selection == 1){
                ui->selection_arrow2->setVisible(true);
            }else if(current_selection == 2){
                ui->selection_arrow3->setVisible(true);
            }

            if(EEG.electrodes_connected == false){
                ui->light_blue->setVisible(true);
            }

            return;
        case SESSION:
            ui->progressBar->setVisible(true);
            ui->timer_label->setVisible(true);
            ui->electrodes_button->setVisible(true);

            if(EEG.electrodes_connected == false){
                ui->light_blue->setVisible(true);
                ui->light_red->setVisible(true);
            }

            return;
        case LOGS:
            if(EEG.electrodes_connected == false){
                ui->light_blue->setVisible(true);
            }

            return;
        case DATETIME:
            if(EEG.electrodes_connected == false){
                ui->light_blue->setVisible(true);
            }

            ui->date_label->setVisible(true);
            ui->time_label->setVisible(true);
            QDateTime local(QDateTime::currentDateTime());
            QDateTime UTC(local.toUTC());
            ui->date_label->setText(local.date().toString());
            ui->time_label->setText(local.time().toString());


            return;
    }
}

void MainWindow::on_menu_button_up_pressed()
{
    current_selection = std::max(0, current_selection - 1);

    update_ui();
}

void MainWindow::on_menu_button_down_pressed()
{
    current_selection = std::min(2, current_selection + 1);

    update_ui();
}

void MainWindow::on_power_button_pressed()
{
    if(state == POWEREDOFF){
        state = MENU;
        update_ui();
    }else{
        state = POWEREDOFF;
        update_ui();
    }
}


void MainWindow::on_start_button_pressed()
{
    if(state == MENU){
        if(current_selection == 0){
            state = SESSION;
            update_ui();
        }else if(current_selection == 1){
            state = LOGS;
            update_ui();
        }else {
            state = DATETIME;
            update_ui();
        }
    }
}


void MainWindow::on_pause_button_pressed()
{

}


void MainWindow::on_stop_button_pressed()
{

}


void MainWindow::on_menu_button_pressed()
{
    current_selection = 0;
    state = MENU;
    update_ui();
}


void MainWindow::on_electrodes_button_pressed()
{
    EEG.electrodes_connected = (false == EEG.electrodes_connected); // proud of this haha
    update_ui();
}

