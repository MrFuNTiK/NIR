#include <stdexcept>
#include <cstring>
#include <rt_audio/RtAudio.h>
#include <logger/logger.hpp>
#include <util_helper/sound_provider.hpp>

int record_callback(void* outputBuffer,
                    void* inputBuffer,
                    unsigned int nFrames,
                    double streamTime,
                    RtAudioStreamStatus status,
                    void *userData);

SoundProvider::SoundProvider(unsigned int sampleRate,
                             unsigned int windowSize) :
    _sampleRate(sampleRate),
    _windowSize(windowSize),
    status(RECIEVED)
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
    rt.closeStream();
}

void SoundProvider::GetData(std::vector<double>& first, std::vector<double>& second)
{
    TRACE_EVENT( EVENTS::SOUND, "Sound has been requested" );
    std::unique_lock<std::mutex> lock( mut );
    buffer.cond_var.wait(lock, [=](){return status == AWAITS;});

    std::memcpy(&first[0], &this->first[0], _windowSize*sizeof(double));
    std::memcpy(&second[0], &this->second[0], _windowSize*sizeof(double));

    status = RECIEVED;
    lock.unlock();
    TRACE_EVENT( EVENTS::SOUND, "Sound has been recieved" );
}

int record_callback(void* ,
                    void* inputBuffer,
                    unsigned int ,
                    double ,
                    RtAudioStreamStatus ,
                    void *userData)
{
    TRACE_EVENT( EVENTS::SOUND, "RtAudio callback called" );
    volatile callback_buffer_t* buffer = reinterpret_cast<callback_buffer_t*>(userData);
    double* inBuffer = reinterpret_cast<double*>(inputBuffer);

    if(buffer->status != RECIEVED)
    {
        TRACE_EVENT( EVENTS::SOUND, "RtAudio callback leaved" );
        return 0;
    }

    std::unique_lock<std::mutex> lock( buffer->mutex );
    buffer->cond_var.wait(lock, [=](){return buffer->status == RECIEVED;});

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

    TRACE_EVENT( EVENTS::SOUND, "RtAudio callback wrote sound" );
    buffer->status = AWAITS;
    buffer->cond_var.notify_one();
    buffer->mutex.unlock();
    return 0;
}
