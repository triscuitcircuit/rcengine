#pragma once

#include "RcEngine.h"
#include "RcEngine/Sound/SoundBuffer.h"
#include <vector>
#include <complex>
#include <cmath>
#include <sndfile.h>

namespace RcEngine {

    class SpectrogramPanel {
    public:
        SpectrogramPanel() = default;
        ~SpectrogramPanel() = default;

        void OnImGuiRender();
        void SetSoundBuffer(Ref<SoundBuffer> buffer);
        void Clear();

    private:
        void LoadAudioData();
        void ComputeFFT(const std::vector<float>& input, std::vector<float>& output);
        void DrawWaveform(const std::vector<float>& waveform);
        void DrawSpectrum(const std::vector<float>& spectrum);

        Ref<SoundBuffer> m_SoundBuffer;
        std::vector<float> m_Waveform;
        std::vector<float> m_Spectrum;
        bool m_ShowWaveform = true;
        bool m_ShowSpectrum = true;
        
        static constexpr int FFT_SIZE = 512;
        static constexpr int WAVEFORM_SAMPLES = 2048;
    };

}
