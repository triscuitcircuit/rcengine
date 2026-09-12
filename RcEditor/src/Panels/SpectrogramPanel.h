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
        
        // Independent file loading
        void LoadAudioFile(const std::string& path);

    private:
        void LoadAudioData();
        void UpdateLiveData();
        void UpdateDataAtPosition(int startSample);
        void ComputeFFT(const std::vector<float>& input, std::vector<float>& output);
        void DrawWaveform(const std::vector<float>& waveform);
        void DrawSpectrum(const std::vector<float>& spectrum);

        Ref<SoundBuffer> m_SoundBuffer;
        std::vector<float> m_Waveform;
        std::vector<float> m_Spectrum;
        bool m_ShowWaveform = true;
        bool m_ShowSpectrum = true;
        
        // For independent playback
        std::string m_LoadedFilePath;
        bool m_IndependentMode = false;  // True if loaded independently (not from component)
        
        static constexpr int FFT_SIZE = 512;
        static constexpr int WAVEFORM_SAMPLES = 2048;
    };

}
