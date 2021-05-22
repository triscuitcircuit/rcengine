//
// Created by Tristan Zippert on 5/22/21.
//
#pragma once

#include "Event.h"
#include <sstream>

namespace RcEngine{
    class RC_API KeyEvent: public Event{
    public:
        inline int GetKeyCode() const{return m_KeyCode; }
        EVENT_CLASS_CATEGORY( EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(int keycode): m_KeyCode(keycode){}
        int m_KeyCode;
    };
    class RC_API KeyPressedEvent
            : public KeyEvent{
        KeyPressedEvent(int keycode, int repeat): KeyEvent(keycode), m_RepeatCount(repeat){}

        inline int GetRepeatCount() const {return m_RepeatCount;}

        std::string ToString() const override{
            std::stringstream stream;
            stream << "Received KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " many repeats)";
            return stream.str();
        }

        EVENT_CLASS_TYPE(OnKeyPressed);
    private:
        int m_RepeatCount;
    };
    class RC_API KeyReleasedEvent: public KeyEvent{

    public:
        KeyReleasedEvent(int keycode)
        : KeyEvent(keycode){}

        std::string ToString() const override{
            std::stringstream stream;
            stream << "KeyReleasedEvent: " << m_KeyCode;
            return stream.str();
        }
        EVENT_CLASS_TYPE(OnKeyRelease)
    };
}