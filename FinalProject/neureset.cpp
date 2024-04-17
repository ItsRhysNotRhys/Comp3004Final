#include "neureset.h"

Neureset::Neureset()
{
    setup();
}

Neureset::~Neureset()
{

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
