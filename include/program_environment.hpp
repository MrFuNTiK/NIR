#ifndef PROGRAMM_ENVORINMENT_HPP
# define PROGRAMM_ENVORINMENT_HPP

# include <memory>

# include <TDE_class.hpp>

class program_environment
{
private:
    static std::shared_ptr<program_environment> _pe;
    uint16_t _window_size;
    uint16_t _sample_rate;
    uint16_t _window_avrg_num;
    tde_meth _meth;
    weighting_func _weight_fn;
    program_environment();

public:
    ~program_environment();
    program_environment(program_environment* pe) = delete;
    void operator = (const program_environment*) = delete;

    static std::shared_ptr<program_environment> GetInstance();
    void SetWindowSize(uint16_t window_size);
    void SetSampleRate(uint16_t sample_rate);
    void SetWinAvrgNum(uint16_t num);
    void SetMethodTDE(tde_meth meth);
    void SetWeightingFunction(weighting_func feighting_fn);

    TDE* CreateCalculator();
    uint16_t GetWindowSize();
    uint16_t GetSampleRate();
    uint16_t GetWinAvrgNum();
};

#endif // PROGRAMM_ENVORINMENT_HPP