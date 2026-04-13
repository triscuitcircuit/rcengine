//
// Created by Tristan Zippert on 3/8/25.
//

#ifndef LUACAMERACONTROLLER_H
#define LUACAMERACONTROLLER_H

#include "RcEngine/Scene/ScriptableEntity.h"
#include "RcEngine/Core/Input.h"
#include "RcEngine/Core/keycodes.h"

namespace RcEngine {
    class LuaCameraController : public ScriptableEntity {
    public:
        void OnCreate(){
            std::cout << "Entity Created (Lua)" << std::endl;
        }
        void OnDestroy(){

        }
        void OnUpdate(Timestep ts){
            auto& translation = GetComponent<TransformComponent>().Translation;
            float speed = 5.0f;

            if(Input::IsKeyPressed(Key::A)){
                translation.x -= speed * ts;
            }
            if(Input::IsKeyPressed(Key::D)){
                translation.x += speed * ts;
            }
            if(Input::IsKeyPressed(Key::W)){
                translation.y += speed * ts;
            }
            if(Input::IsKeyPressed(Key::S)){
                translation.y -= speed * ts;
            }
        }
    };
}
#endif //LUACAMERACONTROLLER_H
