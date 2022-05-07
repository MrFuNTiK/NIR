#ifndef SOUND_PROVIDER_HPP
# define SOUND_PROVIDER_HPP

# include <vector>
# include <mutex>
# include "rt_audio/RtAudio.h"

///@defgroup utility_helpers
///@addtogroup utility_helpers
///@{

///@defgroup sound_provider
///@addtogroup sound_provider
///@{

/**
 * @brief Status of data provider
 */
typedef enum
{
    AWAITS,     ///< Set by RtAudio callback when new data was copied to provider internal storage
    RECIEVED,   ///< Set by @ref SoundProvider::GetData() "GetData()" when data from provider storage was copiet to provided pointers
} audio_data_status;

/**
 * @brief Buffer data passed to RtAudio callback
 */
typedef struct
{
    std::vector<double>& firstChannel;
    std::vector<double>& secondChannel;
    volatile audio_data_status& status;
    std::mutex& mutex;
} callback_buffer_t;

/**
 * @class SoundProvider
 * @brief Class uses RtAudio to get data from ADC device.
 *
 */
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
    /**
     * @brief Construct a new Sound Provider object
     *
     * @param sampleRate    required sampling frequency of signals
     * @param windowSize    requared number of samples per channel
     */
    SoundProvider(unsigned int sampleRate, unsigned int windowSize);
    ~SoundProvider();

    /**
     * @brief Get new data from ADC.
     *
     * Method awaits until new data will be avaliable and writes it to provided ponters.
     *
     * @param first     samples of the first channel
     * @param second    samples of the second channel
     */
    void GetData(std::vector<double>& first, std::vector<double>& second);
};

///@} sound_provider
///@} utility_helpers

#endif // SOUND_PROVIDER_HPP
