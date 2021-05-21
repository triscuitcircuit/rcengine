#include "RcEngine.h"

class RcGame: public RcEngine::Application{
public:
    RcGame() { };
    ~RcGame() { };
};
RcEngine::Application* RcEngine::CreateApplication(){
    return new RcGame();
}