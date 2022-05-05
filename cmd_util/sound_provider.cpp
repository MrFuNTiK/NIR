#include <rt_audio/RtAudio.h>
#include <sound_provider.hpp>
#include <stdexcept>
#include <cstring>
#include <mutex>

int record_callback(void* outputBuffer,
                    void* inputBuffer,
                    unsigned int nFrames,
                    double streamTime,
                    RtAudioStreamStatus status,
                    void *userData);



SoundProvider::SoundProvider(unsigned int sampleRate,
                             unsigned int windowSize) :
    _sampleRate(sampleRate),
    _windowSize(windowSize)
{
    if(4 > windowSize)
    {
        throw std::logic_error("Window size must be greater or equal 4");
    }
    if(windowSize & (windowSize-1))
    {
        throw std::logic_error("Window size must be a power of 2");
    }

    if(0 == sampleRate)
    {
        throw std::logic_error("Sampling rate must be greater than 0");
    }
    first.resize(windowSize);
    second.resize(windowSize);

    if( rt.getDeviceCount() < 1 )
    {
        throw std::runtime_error("No audio device found");
    }
    params.deviceId = rt.getDefaultInputDevice();
    params.nChannels = 2;
    params.firstChannel = 0;

    try
    {
        rt.openStream(nullptr,
                      &params,
                      RTAUDIO_FLOAT64,
                      _sampleRate,
                      &_windowSize,
                      record_callback,
                      reinterpret_cast<void*>(&buffer) );
        rt.startStream();
    }
    catch(const RtAudioError& e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
}

SoundProvider::~SoundProvider()
{
    rt.stopStream();
}

void SoundProvider::GetData(std::vector<double>& first, std::vector<double>& second)
{
    while( buffer.status != AWAITS )
    {}
    mut.try_lock();
    std::memcpy(&first[0], &this->first[0], _windowSize*sizeof(double));
    std::memcpy(&second[0], &this->second[0], _windowSize*sizeof(double));
    status = RECIEVED;
    std::cout << "Recieved\n";
    mut.unlock();
}

int record_callback(void* ,
                    void* inputBuffer,
                    unsigned int ,
                    double ,
                    RtAudioStreamStatus ,
                    void *userData)
{
    volatile callback_buffer_t* buffer = reinterpret_cast<callback_buffer_t*>(userData);
    double* inBuffer = reinterpret_cast<double*>(inputBuffer);

    while(buffer->status != RECIEVED )
    {}

    buffer->mutex.try_lock();

    std::memcpy(    &buffer->firstChannel[0],
                    inBuffer,
                    buffer->firstChannel.size()*sizeof(double));
    std::memcpy(    &buffer->secondChannel[0],
                    inBuffer+buffer->secondChannel.size(),
                    buffer->secondChannel.size()*sizeof(double));

    std::memset(    inBuffer,
                    0,
                    buffer->firstChannel.size() * sizeof(double));
    std::memset(    inBuffer+buffer->secondChannel.size(),
                    0,
                    buffer->secondChannel.size() * sizeof(double));

    buffer->status = AWAITS;
    std::cout << "Copied\n";
    buffer->mutex.unlock();
    return 0;
}
