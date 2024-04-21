#include "neureset.h"

Neureset::Neureset()
{
    setup();
    srand(time(nullptr));
}

Neureset::~Neureset()
{
    delete analysis_timer;
    delete feedback_timer;
}

void Neureset::setup(){
    log_size = sizeof(logs[0]) / sizeof(Waveform);
    num_logs = sizeof(logs) / (sizeof(Waveform) * log_size);

    clear_session();

    qInfo("number of logs: %d", num_logs);
    qInfo("length of logs: %d", log_size);

}

void Neureset::session_start(){
    current_round = 0;
    analysis_timer->start(5000);
    qInfo("Analysing... Please wait");
}

void Neureset::clear_session(){
    for(int i = 0; i < num_logs; ++i){
        for(int j = 0; j < log_size; ++j){
            logs[i][j].reset();
        }
    }
}

void Neureset::analysis(){
    //Perform random frequency generation
    //Save frequencies to the array
    int band_selection = rand() % 2;

    //band = 0 then use a b t
    //band = 1 then use b t g
    int freq1, freq2, freq3;

    for(int i = 0; i < log_size; ++i){
        if(band_selection == 0){
            freq1 = (rand() % alpha_range) + alpha_low;
            freq2 = (rand() % beta_range) + beta_low;
            freq3 = (rand() % theta_range) + theta_low;
        }else{
            freq1 = (rand() % beta_range) + beta_low;
            freq2 = (rand() % theta_range) + theta_low;
            freq3 = (rand() % gamma_range) + gamma_low;
        }

        float amp1 = ((rand() % 9) / 10.0) + 0.2;
        float amp2 = ((rand() % 9) / 10.0) + 0.2;
        float amp3 = ((rand() % 9) / 10.0) + 0.2;

        //qInfo("Frequencies for electrod %d :  Freq1: %d, Freq2: %d, Freq3: %d, Amp1: %f, Amp2: %f, Amp3: %f", i, freq1, freq2, freq3, amp1, amp2, amp3);

        logs[current_round][i].set_waveform(freq1, freq2, freq3, amp1, amp2, amp3);
    }


}

Waveform* Neureset::get_wave(int round, int wave){
    return &(logs[round][wave]);
}

void Neureset::feedback(){
    qInfo("Print frequency value impulsed");
}
