//
// Created by Tristan Zippert on 7/6/21.
//

#include <RcEngine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace RcEngine {
    class RcEditor : public Application {
    public:
        RcEditor()
        : Application("Rc Engine Editor"){
            PushLayer(new EditorLayer());
        };

        ~RcEditor() {};
    };

    Application *CreateApplication() {
        return new RcEditor();
    }
}