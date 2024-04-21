#include "waveform.h"

Waveform::Waveform()
{

}

void Waveform::set_waveform(int f1, int f2, int f3, float a1, float a2, float a3){
    frequency_1 = f1;
    frequency_2 = f2;
    frequency_3 = f3;

    amplitude_1 = a1;
    amplitude_2 = a2;
    amplitude_3 = a3;

    //qInfo("Frequencies for wavelength:  Freq1: %d, Freq2: %d, Freq3: %d, Amp1: %f, Amp2: %f, Amp3: %f", frequency_1, frequency_2, frequency_3, amplitude_1, amplitude_2, amplitude_3);
}

void Waveform::reset(){
    frequency_1 = 0;
    frequency_2 = 0;
    frequency_3 = 0;

    amplitude_1 = 0;
    amplitude_2 = 0;
    amplitude_3 = 0;
}
