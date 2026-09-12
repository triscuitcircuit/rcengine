#include "SpectrogramPanel.h"
#include <../imgui/imgui.h>
#include <algorithm>
#include <cmath>

namespace RcEngine {

    void SpectrogramPanel::OnImGuiRender() {
        ImGui::Begin("Audio Spectrogram");

        // File selection UI
        ImGui::Text("Drop audio file here or click to browse");
        ImGui::Separator();
        
        // Drag-drop target
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const wchar_t* path = (const wchar_t*)payload->Data;
                std::filesystem::path audioPath = std::filesystem::path(path);
                std::string pathStr = audioPath.string();
                
                // Check if it's an audio file
                std::string ext = audioPath.extension().string();
                if (ext == ".wav" || ext == ".ogg" || ext == ".mp3" || ext == ".flac") {
                    LoadAudioFile(pathStr);
                }
            }
            ImGui::EndDragDropTarget();
        }
        
        // Manual file picker button
        if (ImGui::Button("Browse Audio File...")) {
            // TODO: Integrate with file dialog when available
            ImGui::OpenPopup("FilePathInput");
        }
        
        if (ImGui::BeginPopup("FilePathInput")) {
            static char pathBuffer[256] = "";
            ImGui::Text("Enter audio file path:");
            if (ImGui::InputText("##path", pathBuffer, sizeof(pathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                LoadAudioFile(std::string(pathBuffer));
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (!m_SoundBuffer) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "No audio loaded");
            ImGui::Text("Supported formats: .wav, .ogg, .mp3, .flac");
            ImGui::End();
            return;
        }

        // Update data every frame if playing
        if (m_SoundBuffer->isPlaying()) {
            UpdateLiveData();
        }

        ImGui::Separator();
        ImGui::Text("Audio: %s", m_SoundBuffer->getPath().c_str());
        
        // Playback controls (only in independent mode)
        if (m_IndependentMode) {
            if (m_SoundBuffer->isPlaying()) {
                if (ImGui::Button("Pause")) {
                    m_SoundBuffer->Pause();
                }
            } else {
                if (ImGui::Button("Play")) {
                    m_SoundBuffer->Play();
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Stop")) {
                m_SoundBuffer->Stop();
            }
            
            // Volume control
            static float volume = 1.0f;
            if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f)) {
                m_SoundBuffer->SetGain(volume);
            }
        }
        
        // Show playback position
        if (m_SoundBuffer->isPlaying()) {
            float pos = m_SoundBuffer->GetPlaybackPosition();
            const auto& info = m_SoundBuffer->GetSoundInfo();
            float duration = (float)info.frames / (float)info.samplerate;
            ImGui::Text("Playing: %.2fs / %.2fs", pos, duration);
            ImGui::ProgressBar(pos / duration);
        }
        
        ImGui::Separator();

        ImGui::Checkbox("Show Waveform", &m_ShowWaveform);
        ImGui::SameLine();
        ImGui::Checkbox("Show Spectrum", &m_ShowSpectrum);

        if (m_ShowWaveform && !m_Waveform.empty()) {
            ImGui::Text("Waveform");
            DrawWaveform(m_Waveform);
        }

        if (m_ShowSpectrum && !m_Spectrum.empty()) {
            ImGui::Text("Frequency Spectrum");
            DrawSpectrum(m_Spectrum);
        }

        ImGui::End();
    }

    void SpectrogramPanel::SetSoundBuffer(Ref<SoundBuffer> buffer) {
        m_SoundBuffer = buffer;
        m_IndependentMode = false;  // Set from component
        
        if (!buffer) {
            Clear();
            return;
        }

        m_Waveform.resize(WAVEFORM_SAMPLES);
        m_Spectrum.resize(FFT_SIZE / 2);
        
        LoadAudioData();
    }
    
    void SpectrogramPanel::LoadAudioFile(const std::string& path) {
        // Clear existing buffer
        if (m_SoundBuffer) {
            m_SoundBuffer.reset();
        }
        
        // Create new independent sound buffer
        m_SoundBuffer = CreateRef<SoundBuffer>(path.c_str());
        m_IndependentMode = true;
        m_LoadedFilePath = path;
        
        // Initialize data buffers
        m_Waveform.resize(WAVEFORM_SAMPLES);
        m_Spectrum.resize(FFT_SIZE / 2);
        
        // Load visualization data
        LoadAudioData();
    }

    void SpectrogramPanel::Clear() {
        m_Waveform.clear();
        m_Spectrum.clear();
        m_SoundBuffer = nullptr;
    }

    void SpectrogramPanel::LoadAudioData() {
        if (!m_SoundBuffer) return;
        
        // Load initial data from beginning of file
        UpdateDataAtPosition(0);
    }
    
    void SpectrogramPanel::UpdateLiveData() {
        if (!m_SoundBuffer) return;
        
        // Get current playback position in samples
        float posSeconds = m_SoundBuffer->GetPlaybackPosition();
        const auto& info = m_SoundBuffer->GetSoundInfo();
        int posSamples = (int)(posSeconds * info.samplerate);
        
        // Update visualization with current data
        UpdateDataAtPosition(posSamples);
    }
    
    void SpectrogramPanel::UpdateDataAtPosition(int startSample) {
        if (!m_SoundBuffer) return;
        
        // Get waveform data
        std::vector<float> waveformData;
        if (m_SoundBuffer->GetPCMData(waveformData, startSample, WAVEFORM_SAMPLES)) {
            // Copy to display buffer
            for (int i = 0; i < WAVEFORM_SAMPLES && i < waveformData.size(); ++i) {
                m_Waveform[i] = waveformData[i];
            }
        }
        
        // Get spectrum data (FFT)
        std::vector<float> fftInput;
        if (m_SoundBuffer->GetPCMData(fftInput, startSample, FFT_SIZE)) {
            ComputeFFT(fftInput, m_Spectrum);
        }
    }

    void SpectrogramPanel::ComputeFFT(const std::vector<float>& input, std::vector<float>& output) {
        int N = FFT_SIZE;
        if (input.size() < N) return;

        std::vector<std::complex<float>> complexInput(N);
        for (int i = 0; i < N; ++i) {
            float window = 0.5f * (1.0f - std::cos(2.0f * 3.14159265f * i / (N - 1)));
            complexInput[i] = std::complex<float>(input[i] * window, 0.0f);
        }

        for (int i = 0; i < N / 2; ++i) {
            std::complex<float> sum(0.0f, 0.0f);
            for (int j = 0; j < N; ++j) {
                float angle = -2.0f * 3.14159265f * i * j / N;
                std::complex<float> w(std::cos(angle), std::sin(angle));
                sum += complexInput[j] * w;
            }
            float magnitude = std::sqrt(sum.real() * sum.real() + sum.imag() * sum.imag());
            output[i] = 20.0f * std::log10(magnitude + 1e-6f);
        }
    }

    void SpectrogramPanel::DrawWaveform(const std::vector<float>& waveform) {
        ImVec2 canvas_size = ImVec2(ImGui::GetContentRegionAvail().x, 150);
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_pos, 
            ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
            IM_COL32(20, 20, 20, 255));
        
        draw_list->AddRect(canvas_pos, 
            ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
            IM_COL32(60, 60, 60, 255));

        float centerY = canvas_pos.y + canvas_size.y * 0.5f;
        draw_list->AddLine(
            ImVec2(canvas_pos.x, centerY),
            ImVec2(canvas_pos.x + canvas_size.x, centerY),
            IM_COL32(80, 80, 80, 255)
        );

        if (waveform.size() > 1) {
            float scale = canvas_size.y * 0.45f;
            for (size_t i = 1; i < waveform.size(); ++i) {
                float x0 = canvas_pos.x + (float)(i - 1) / waveform.size() * canvas_size.x;
                float x1 = canvas_pos.x + (float)i / waveform.size() * canvas_size.x;
                float y0 = centerY - waveform[i - 1] * scale;
                float y1 = centerY - waveform[i] * scale;
                
                draw_list->AddLine(ImVec2(x0, y0), ImVec2(x1, y1), IM_COL32(0, 255, 128, 255), 1.5f);
            }
        }

        ImGui::Dummy(canvas_size);
    }

    void SpectrogramPanel::DrawSpectrum(const std::vector<float>& spectrum) {
        ImVec2 canvas_size = ImVec2(ImGui::GetContentRegionAvail().x, 200);
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_pos, 
            ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
            IM_COL32(20, 20, 20, 255));
        
        draw_list->AddRect(canvas_pos, 
            ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
            IM_COL32(60, 60, 60, 255));

        if (spectrum.size() > 1) {
            float minDb = -60.0f;
            float maxDb = 20.0f;
            float dbRange = maxDb - minDb;
            
            float barWidth = canvas_size.x / spectrum.size();
            
            for (size_t i = 0; i < spectrum.size(); ++i) {
                float db = spectrum[i];
                db = std::max(minDb, std::min(maxDb, db));
                
                float normalized = (db - minDb) / dbRange;
                float barHeight = normalized * canvas_size.y;
                
                float x = canvas_pos.x + i * barWidth;
                float y = canvas_pos.y + canvas_size.y - barHeight;
                
                float hue = 0.6f - normalized * 0.5f;
                ImU32 color = ImGui::ColorConvertFloat4ToU32(
                    ImVec4(hue, 1.0f - hue * 0.5f, 1.0f - hue, 1.0f)
                );
                
                draw_list->AddRectFilled(
                    ImVec2(x, y),
                    ImVec2(x + barWidth - 1, canvas_pos.y + canvas_size.y),
                    color
                );
            }
        }

        ImGui::Dummy(canvas_size);
    }

}
