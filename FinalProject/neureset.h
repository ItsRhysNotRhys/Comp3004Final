#ifndef NEURESET_H
#define NEURESET_H

#include <QMainWindow>
#include <string>
#include <QTimer>

#include "waveform.h"
#include <random>

class Neureset
{
public:
    Neureset();
    ~Neureset();

    void connect_electrodes(){ electrodes_connected = true;};
    void session_start();
    void analysis();
    void feedback();
    Waveform* get_wave(int, int);
    void clear_session();
    void save_session();

    bool electrodes_connected = false;
    bool session_complete = true;
    bool paused = false;
    int total_analysis = 4;
    int current_round = 0;
    int total_feedbacks = 16;
    int current_feedback = 0;
    int battery = 100;


private:
    void setup();

    Waveform logs[5][7];
    int num_logs, log_size;
    int sessions_completed = 0;

    int alpha_low = 8;
    int alpha_range = 4;
    int beta_low = 12;
    int beta_range = 18;
    int theta_low = 4;
    int theta_range = 4;
    int gamma_low = 25;
    int gamma_range = 115;

    QTimer* analysis_timer;
    QTimer* feedback_timer;
};

#endif // NEURESET_H
