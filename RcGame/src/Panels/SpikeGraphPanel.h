#include "RcEngine/Core/Timestep.h"
#include "glm/vec2.hpp"
#include <vector>

namespace RcEngine{
    class SpikeGraphPanel{
    public:
        SpikeGraphPanel();
        void OnImGuiRender();
        void OnUpdate(RcEngine::Timestep ts);
    private:
        glm::vec2 m_ViewPortSize = {1264,666};
        int m_N = 200;
        double m_C = 100;
        double m_Vt = -40.0;
        double m_K = 0.7;
        double m_Cm = -50.0;
        double m_D = 100;
        int m_vPeak = 35;

        double m_Tau = 1.0;
        std::vector<double> m_I;
        std::vector<double> m_u;
        std::vector<double> s_v;
    };
}