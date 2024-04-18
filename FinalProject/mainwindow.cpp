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

    session_updater = new QTimer(this);
    session_updater->setSingleShot(false);
    QObject::connect(session_updater, &QTimer::timeout, this, &MainWindow::session_update);

    analysis_timer = new QTimer();
    analysis_timer->setSingleShot(false);
    connect(analysis_timer, &QTimer::timeout, this, &MainWindow::analysis);

    feedback_timer = new QTimer();;
    feedback_timer->setSingleShot(false);
    connect(feedback_timer, &QTimer::timeout, this, &MainWindow::feedback);

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
            ui->electrodes_button->setVisible(false);
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
                ui->electrodes_button->setText("Connect Electrodes");
            }else{
                ui->electrodes_button->setText("Disconnect Electrodes");
            }

            return;
        case SESSION:
            ui->progressBar->setVisible(true);
            ui->timer_label->setVisible(true);
            ui->electrodes_button->setVisible(true);

            if(EEG.electrodes_connected == false){
                ui->light_blue->setVisible(true);
                ui->light_red->setVisible(true);
                ui->electrodes_button->setText("Connect Electrodes");
            }else{
                ui->electrodes_button->setText("Disconnect Electrodes");
            }

            return;
        case LOGS:
            if(EEG.electrodes_connected == false){
                ui->light_blue->setVisible(true);
                ui->electrodes_button->setText("Connect Electrodes");
            }else{
                ui->electrodes_button->setText("Disconnect Electrodes");
            }

            return;
        case DATETIME:
            if(EEG.electrodes_connected == false){
                ui->light_blue->setVisible(true);
                ui->electrodes_button->setText("Connect Electrodes");
            }

            ui->date_label->setVisible(true);
            ui->time_label->setVisible(true);
            QDateTime local(QDateTime::currentDateTime());
            ui->date_label->setText(local.date().toString());
            ui->time_label->setText(local.time().toString());


            return;
    }
}

void MainWindow::start_session(){
    EEG.current_round = 0;
    analysis_timer->start(5000);
    qInfo("Analysing... Please wait");
}

void MainWindow::session_update(){
    if(EEG.session_complete == true){
        update_ui();
        session_updater->stop();
        state = MENU;
    }else{
        update_ui();
    }
}

void MainWindow::analysis(){
    if(EEG.current_round < EEG.total_analysis){
        EEG.current_round++;
        qInfo("Done Analysis, delivering feedback impulses");
        EEG.analysis();
        analysis_timer->stop();
        feedback_timer->start(1000 / 16);
        EEG.current_feedback = 0;
    }else{
        qInfo("Session completed! Saving session log... DONE");
        analysis_timer->stop();
        EEG.session_complete = true;
    }
}

void MainWindow::feedback(){
    if(EEG.current_feedback < EEG.total_feedbacks){
        EEG.current_feedback++;
        EEG.feedback();
        qInfo("Administered feedback impulse %d of %d", EEG.current_feedback, EEG.total_feedbacks);
    }else{
        feedback_timer->stop();
        analysis_timer->start(5000);
        qInfo("Analysing... Please wait");
    }
}

void MainWindow::on_menu_button_up_pressed()
{
    if(state == POWEREDOFF){
        return;
    }
    current_selection = std::max(0, current_selection - 1);

    update_ui();
}

void MainWindow::on_menu_button_down_pressed()
{
    if(state == POWEREDOFF){
        return;
    }

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
            start_session();
            update_ui();
        }else if(current_selection == 1){
            state = LOGS;
            update_ui();
        }else if(current_selection == 2){
            state = DATETIME;
            update_ui();
        }
    }else if(state == SESSION && EEG.electrodes_connected){
        EEG.paused = false;
    }
}


void MainWindow::on_pause_button_pressed()
{
    if(state == SESSION){
        EEG.paused = true;
    }
}


void MainWindow::on_stop_button_pressed()
{
    if(state == SESSION){
        session_updater->stop();
        state = MENU;
        //Call function in EEG to clear session data
    }
}


void MainWindow::on_menu_button_pressed()
{
    if(state == POWEREDOFF){
        return;
    }

    current_selection = 0;
    state = MENU;
    update_ui();
}


void MainWindow::on_electrodes_button_pressed()
{
    EEG.electrodes_connected = (false == EEG.electrodes_connected); // proud of this haha
    update_ui();
}

