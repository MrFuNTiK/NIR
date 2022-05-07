#ifndef SOUND_PROVIDER_HPP
# define SOUND_PROVIDER_HPP

# include <vector>
# include <mutex>
# include "rt_audio/RtAudio.h"


typedef enum
{
    AWAITS,
    RECIEVED,
} audio_data_status;

typedef struct
{
    std::vector<double>& firstChannel;
    std::vector<double>& secondChannel;
    volatile audio_data_status& status;
    std::mutex& mutex;
} callback_buffer_t;

class SoundProvider
{
private:
    unsigned int _sampleRate;
    unsigned int _windowSize;
    std::mutex mut;
    audio_data_status status = RECIEVED;

    RtAudio rt;
    RtAudio::StreamParameters params;
    std::vector<double> first;
    std::vector<double> second;
    callback_buffer_t buffer{first, second, status, mut};

public:
    SoundProvider(unsigned int sampleRate, unsigned int windowSize);
    ~SoundProvider();
    void GetData(std::vector<double>& first, std::vector<double>& second);

};

#endif // SOUND_PROVIDER_HPP
