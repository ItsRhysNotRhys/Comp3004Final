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

    delete session_updater;
    delete analysis_timer;
    delete feedback_timer;
    delete series;
    delete chart;
    delete chart_view;
}

void MainWindow::setup(){
    state = POWEREDOFF;
    ui->progressBar->setRange(0, 100);
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

    series = new QLineSeries();
    series->clear();

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    chart->setTitle("Wave form");

    chart_view = new QChartView(chart, ui->graphicsView);
    chart_view->resize(500, 350);
    chart_view->setRenderHint(QPainter::Antialiasing);



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
    ui->battery_percentage_label->setText(QString::number(EEG.battery));

    ui->date_label->setVisible(false);
    ui->time_label->setVisible(false);

    ui->graphicsView->setVisible(false);

    ui->wave_debug->setVisible(false);

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
            ui->wave_debug->setVisible(true);
            ui->round_number->setText(QString::number(round_selection + 1));
            ui->electrode_number->setText(QString::number(electrode_selection + 1));
            ui->graphicsView->setVisible(true);
            load_graph();
            ui->progressBar->setVisible(true);
            ui->progressBar->setValue(EEG.current_round * 100 / (EEG.total_analysis + 1));
            ui->timer_label->setVisible(true);
            if(session_time_remaining < 10){
                QString time_left = "0:0" + QString::number(session_time_remaining);
                ui->timer_label->setText(time_left);
            }else {
                QString time_left = "0:" + QString::number(session_time_remaining);
                ui->timer_label->setText(time_left);
            }
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
    EEG.session_complete = false;
    EEG.clear_session();
    session_time_remaining = ((EEG.total_analysis - EEG.current_round) * (5 + 1) + 5);

    round_selection = 0;
    electrode_selection = 0;

    if(EEG.electrodes_connected == false){
        qInfo("electrodes are not connected... Waiting");
        return;
    }else{
        EEG.paused = false;
    }

    session_updater->start(1000);
    analysis_timer->start(5000);
    qInfo("Analysing... Please wait");
}

void MainWindow::session_update(){
    if(EEG.session_complete == true){
        update_ui();
        session_updater->stop();
        state = MENU;
    }else{
        if(EEG.paused == false){
            session_time_remaining--;
            EEG.battery --;

            if(EEG.battery % 4 == 0){
                EEG.battery --;
            }
        }

        update_ui();
    }
}

void MainWindow::analysis(){
    if(EEG.current_round < EEG.total_analysis){
        qInfo("Done Analysis, delivering feedback impulses");
        EEG.analysis();
        analysis_timer->stop();
        feedback_timer->start(1000 / 16);
        EEG.current_feedback = 0;
    }else{
        qInfo("Session completed! Saving session log... DONE");
        EEG.analysis();
        analysis_timer->stop();
        EEG.session_complete = true;
        state = MENU;
        update_ui();
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
        EEG.current_round++;
        qInfo("Analysing... Please wait");
    }
}

void MainWindow::load_graph(){
    Waveform* wave = EEG.get_wave(round_selection, electrode_selection);

    float a1 = wave->amplitude_1;
    float a2 = wave->amplitude_2;
    float a3 = wave->amplitude_3;
    int f1 = wave->frequency_1;
    int f2 = wave->frequency_2;
    int f3 = wave->frequency_3;
    //qInfo("Frequencies for electrode:  Freq1: %d, Freq2: %d, Freq3: %d, Amp1: %f, Amp1: %f, Amp1: %f", f1, f2, f3, a1, a2, a3);
    chart->removeSeries(series);
    series->clear();

    for(float x = 0.0; x < 0.75; x += 0.001){

        float y = a1 * sin(2 * 3.14 * f1 * x) + a2 * sin(2 * 3.14 * f2 * x) + a3 * sin(2 * 3.14 * f3 * x);
        qInfo("%f", y);

        series->append(x, y);
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    ui->graphicsView->repaint();

}

void MainWindow::clear_current_session(){
    session_updater->stop();
    analysis_timer->stop();
    feedback_timer->stop();
    EEG.current_round = 0;
    EEG.current_feedback = 0;
    EEG.session_complete = true;
}

void MainWindow::resume_round(){
    analysis_timer->start(5000);
    session_updater->start(1000);
    qInfo("Analysing... Please wait");

    //Set time left to amount at the start of the round
    session_time_remaining = ((EEG.total_analysis - EEG.current_round) * (5 + 1) + 5);
    update_ui();
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
        clear_current_session();
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
        resume_round();
    }
}


void MainWindow::on_pause_button_pressed()
{
    if(state == SESSION){
        qInfo("Session Paused. Waiting...");
        EEG.paused = true;
        analysis_timer->stop();
        feedback_timer->stop();
    }
}


void MainWindow::on_stop_button_pressed()
{
    if(state == SESSION){
        clear_current_session();
        state = MENU;
        update_ui();
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

    if(state == SESSION && EEG.session_complete == false){
        if(EEG.electrodes_connected){
            EEG.paused = false;
            resume_round();
        }else{
            EEG.paused = true;
            analysis_timer->stop();
            feedback_timer->stop();
            qInfo("Electrodes disconnected... Waiting for connection...");
        }

        }
}


void MainWindow::on_wave_round_up_pressed()
{
    round_selection = std::min(EEG.total_analysis, round_selection + 1);
    update_ui();
}


void MainWindow::on_wave_round_down_pressed()
{
    round_selection = std::max(0, round_selection - 1);
    update_ui();
}


void MainWindow::on_electrode_up_pressed()
{
    electrode_selection = std::min(6, electrode_selection + 1);
    update_ui();
}


void MainWindow::on_electrode_down_pressed()
{
    electrode_selection = std::max(0, electrode_selection - 1);
    update_ui();
}

