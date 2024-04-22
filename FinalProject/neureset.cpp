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
    for(int i = 0; i < log_size; ++i){
        int dom_freq = logs[current_round][i].dominant_frequency;
        int offset = dom_freq + (current_round * 5) + 5;
        qInfo("Site %d: Dominant frequency = %d, Offset frequency to be delivered = %d", i + 1, dom_freq, offset);
    }
}

void Neureset::save_session(QString file_name){
    const char* dir_name = "Sessions";
    std::string dir = "Sessions";

    if(mkdir(dir_name, 0755) == 0){
        qInfo("Created Session folder");
    }


    std::ofstream myfile(dir + "/" + file_name.toStdString() + ".txt");

    if(myfile.is_open()){
        for(int i = 0; i < num_logs; ++i){
            myfile << "Round " << i + 1 << "\n-------------------\n" << "-------------------\n";
            for(int j = 0; j < log_size; ++j){
                int frequency_1 = logs[i][j].frequency_1;
                int frequency_2 = logs[i][j].frequency_2;
                int frequency_3 = logs[i][j].frequency_3;

                float amplitude_1 = logs[i][j].amplitude_1;
                float amplitude_2 = logs[i][j].amplitude_2;
                float amplitude_3 = logs[i][j].amplitude_3;

                int dominant = logs[i][j].dominant_frequency;

                myfile << "Site " << j + 1 << "\n------------\n";
                myfile << "Frequency 1: " << frequency_1 << ",  Amplitude 1: " << amplitude_1 << "\n";
                myfile << "Frequency 2: " << frequency_2 << ",  Amplitude 2: " << amplitude_2 << "\n";
                myfile << "Frequency 3: " << frequency_3 << ",  Amplitude 3: " << amplitude_3 << "\n";
                myfile << "Dominant frequency: " << dominant << "\n\n";
            }
        }

        myfile.close();
        qInfo("Saved session");
        sessions[sessions_completed] = file_name;
    }else {
        return;
    }
}
