
#include "SpikeGraphPanel.h"
#include "rcpch.h"
#include "Platform/MacUtils.h"

#include <../imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <../imgui/imgui_internal.h>
namespace RcEngine {
    static std::vector<float> s_FPS(200);
    static float s_cur_time = 0.0f;
    static std::vector<float> s_time(200);
//    static std::vector<float> s_v(200);

    SpikeGraphPanel::SpikeGraphPanel() :
    m_I(m_N, 0.0),
    m_u(m_N, 0.0),
    s_v(m_N, -60.0f)
    {
    }

    void SpikeGraphPanel::OnImGuiRender() {
        static float  m_Vr = -60.0f;
        static float m_A = 0.3f;
        static float m_B = -2;
        ImGui::Begin("Spike Graph");
        ImGui::SliderFloat("resting membrane potential",&m_Vr, -100, 0, "%.3f");
        ImGui::SliderFloat("recovery time",&m_A, 0, 1, "%.3f");
        ImGui::SliderFloat("amplifying/resonant",&m_B, -2, 2, "%.3f");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewPortSize = {viewportPanelSize.x,viewportPanelSize.y};
        if(m_ViewPortSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0){
            m_ViewPortSize = {viewportPanelSize.x,viewportPanelSize.y};
        }

        for (int i = 0; i < s_v.size() - 1; ++i) {
            s_cur_time = s_v[i+1];
            s_v[i] = s_v[i + 1];
            s_v[i + 1] = s_v[i] + m_Tau * (m_K * (s_v[i] - m_Vr) * (s_v[i] - m_Vt) - s_v[i] + m_I[i]) / m_C;
            m_u[i + 1] = m_u[i] + m_Tau * m_A * (m_B * (s_v[i] - m_Vr) - m_u[i]);
            if (s_v[i + 1] >= m_vPeak) {
                s_v[i] = m_vPeak;
                s_v[i + 1] = m_Cm;
                m_u[i + 1] += m_D;
            }
        }

        char overlay[32];
        sprintf(overlay, "v %f", s_cur_time);
        ImGui::PlotLines("v", reinterpret_cast<const float *>(s_v.data()),
                         (int) s_v.size(), 0, overlay, -1.0f, 20.0f,
                         ImVec2(m_ViewPortSize.x,m_ViewPortSize.y));
        ImGui::End();

    }

    void SpikeGraphPanel::OnUpdate(RcEngine::Timestep ts) {

    }
}
