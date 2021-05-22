#include "RcEngine.h"
//#include "RcEngine/EntryPoint.h"
//main function in entrypoint.h
class RcGame: public RcEngine::Application{
public:
    RcGame() { };
    ~RcGame() { };
};
RcEngine::Application* RcEngine::CreateApplication(){
    return new RcGame();
}