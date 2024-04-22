#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QMainWindow>

class Waveform
{
public:
    Waveform();
    void set_waveform(int, int, int, float, float, float);
    void set_dominant_frequency();
    void reset();

    int frequency_1 = 0;
    int frequency_2 = 0;
    int frequency_3 = 0;
    float amplitude_1 = 0;
    float amplitude_2 = 0;
    float amplitude_3 = 0;

    int dominant_frequency = 0;

private:

};

#endif // WAVEFORM_H
