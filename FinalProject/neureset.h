#ifndef NEURESET_H
#define NEURESET_H

#include <QMainWindow>
#include <string>
#include <QTimer>

class Neureset
{
public:
    Neureset();
    ~Neureset();

    void connect_electrodes(){ electrodes_connected = true;};
    void session_start();
    void analysis();
    void feedback();
    void save_session();

    bool electrodes_connected = false;
    bool session_complete = true;
    bool paused = false;
    int total_analysis = 4;
    int current_round = 0;
    int total_feedbacks = 16;
    int current_feedback = 0;


private:
    void setup();

    int battery = 100;


    float logs[5][7];
    int num_logs, log_size;
    int sessions_completed = 0;




    QTimer* analysis_timer;
    QTimer* feedback_timer;
};

#endif // NEURESET_H
