#ifndef PROGRAMM_ENVORINMENT_HPP
# define PROGRAMM_ENVORINMENT_HPP

# include <memory>
# include "../TDE/TDE_class.hpp"

class programm_environment
{
private:
    static std::shared_ptr<programm_environment> _pe;
    uint16_t _window_size;
    uint16_t _sample_rate;
    tde_meth _meth;
    weighting_func _weight_fn;
    programm_environment();

public:
    ~programm_environment();
    programm_environment(programm_environment* pe) = delete;
    void operator = (const programm_environment*) = delete;

    static std::shared_ptr<programm_environment> GetInstance();
    void SetWindowSize(uint16_t window_size);
    void SetSampleRate(uint16_t sample_rate);
    void SetMethodTDE(tde_meth meth);
    void SetWeightingFunction(weighting_func feighting_fn);
    TDE* GetCalculator();
};

#endif // PROGRAMM_ENVORINMENT_HPP
