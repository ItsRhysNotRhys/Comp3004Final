#ifndef NEURESET_H
#define NEURESET_H

#include <QMainWindow>
#include <string>

class Neureset
{
public:
    Neureset();
    ~Neureset();

    void connect_electrodes(){ electrodes_connected = true;};

    bool electrodes_connected = false;

private:
    void setup();

    int battery = 100;


    float logs[5][7];
    int num_logs, log_size;
    int sessions_completed = 0;
};

#endif // NEURESET_H
