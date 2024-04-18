#include "neureset.h"

Neureset::Neureset()
{
    setup();
}

Neureset::~Neureset()
{
    delete analysis_timer;
    delete feedback_timer;
}

void Neureset::setup(){
    log_size = sizeof(logs[0]) / sizeof(float);
    num_logs = sizeof(logs) / (sizeof(float) * log_size);

    qInfo("number of logs: %d", num_logs);
    qInfo("length of logs: %d", log_size);

    for(int i = 0; i < num_logs; ++i){
        for(int j = 0; j < log_size; ++j){
            logs[i][j] = 0.0;
        }
    }


}

void Neureset::session_start(){
    current_round = 0;
    analysis_timer->start(5000);
    qInfo("Analysing... Please wait");
}

void Neureset::analysis(){
    //Perform random frequency generation
    //Save frequencies to the array
}

void Neureset::feedback(){
    qInfo("Print frequency value impulsed");
}
