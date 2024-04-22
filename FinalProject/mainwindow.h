#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//'sudo apt install libqt5charts5-dev' to download qtcharts

#include <QMainWindow>
#include <QDateTime>
#include <QtCharts>
#include <QDebug>
#include "neureset.h"
#include <algorithm>
#include <string>

enum State { POWEREDOFF, MENU, SESSION, LOGS, DATETIME };

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void session_update();

private slots:
    void on_menu_button_up_pressed();

    void on_menu_button_down_pressed();

    void on_power_button_pressed();

    void on_start_button_pressed();

    void on_pause_button_pressed();

    void on_stop_button_pressed();

    void on_menu_button_pressed();

    void on_electrodes_button_pressed();

    void on_wave_round_up_pressed();

    void on_wave_round_down_pressed();

    void on_electrode_up_pressed();

    void on_electrode_down_pressed();

private:
    Ui::MainWindow *ui;

    void setup();
    void update_ui();
    void analysis();
    void feedback();
    void start_session();
    void clear_current_session();
    void resume_round();
    void load_graph();

    int current_selection = 0;
    int session_time_remaining = 0;
    int electrode_selection = 0;
    int round_selection = 0;
    int wait_timer = 0;

    State state = State::MENU;

    QTimer* analysis_timer;
    QTimer* feedback_timer;
    QTimer* session_updater;
    QChartView* chart_view;
    QChart* chart;
    QLineSeries* series;
    Neureset EEG;
};
#endif // MAINWINDOW_H
