//
// Created by Tristan Zippert on 5/22/21.
//

#pragma once

#include "Event.h"
#include <sstream>

namespace RcEngine{

    class RC_API MouseMovedEvent: public Event{
    public:
        MouseMovedEvent(float x, float y)
        : m_Mouse(x), m_Mouse(y){}
        inline float GetX() const {return m_MouseX; }
        inline float GetY() const {return m_MouseY; }

        std::string ToString() const override{
            std::stringstream stream;
            stream << "MouseMovedEvent(X,y): (" << m_MouseX << ", " << m_MouseY << ")";
            return stream.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_MouseX, m_MouseY;
    };
    class RC_API MouseScrolledEvent: public Event{
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset){}
        inline float GetXOffset() const {return m_XOffset;}
        inline float GetYOffset() const{return m_YOffset;}

        std::string ToString() const override{
            std::stringstream stream;
            stream << "MouseScrolledEvent(X,Y): "<< GetXOffset() << ", " << GetYOffset();
            return stream.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)

        EVENT_CLASS_CATEGORY(EventCategoryMouse| EventCategoryInput)
    private:
        float m_XOffset, m_YOffset;
    };
    class RC_API MouseButtonEvent: public Event{
    public:
        inline int GetMouseButton() const {return m_Button}
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    protected:
        MouseButtonEvent(int button)
        : m_Button(button){}
        int m_Button;
    };
    class RC_API MouseButtonPressedEvent: public MouseButtonEvent{
    public:
        MouseButtonPressedEvent(int button)
        : MouseButtonEvent(button){}
        std::string ToString() const override{
            std::stringstream stream;
            stream << "MouseButtonPressedEvent: " << m_Button;
            return stream.str()
        }

        EVENT_CLASS_TYPE(MouseButtonPress)
    };
    class RC_API MouseButtonReleasedEvent: public MouseButtonEvent{
    public:
        MouseButtonReleasedEvent(int button)
        : MouseButtonEvent(button){}
        std::string ToString() const override{
            std::stringstream stream;
            stream << "MouseButtonReleasedEvent: "<< m_Button;
            return stream.str();
        }

        EVENT_CLASS_TYPE(MouseButtonRelease)
    };
}