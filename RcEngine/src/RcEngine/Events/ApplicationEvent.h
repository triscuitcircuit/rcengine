//
// Created by Tristan Zippert on 5/22/21.
//

#pragma once
#include "Event.h"
#include <sstream>

namespace RcEngine{
    class RC_API WindowResizeEvent: public Event{
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height){}

        inline unsigned int GetWidth() const {return m_Width;}
        inline unsigned int GetHeight() const {return m_Height;}

        std::string ToString() const override{
            std::stringstream stream;
            stream << "WindowResizeEvent(Width, Height): (" << m_Width << ", " << m_Height <<")";
            return stream.str();
        };

        EVENT_CLASS_TYPE(WindowResize)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        unsigned int m_Width, m_Height;
    };
    class RC_API WindowCloseEvent : public Event{
    public:
        WindowCloseEvent(){}

        EVENT_CLASS_CATEGORY(EventCategoryApplication)

        EVENT_CLASS_TYPE(WindowClose)
    };
    class RC_API AppTickEvent: public Event{
    public:
        AppTickEvent(){}

        EVENT_CLASS_TYPE(ApplicationTick)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
    class RC_API AppUpdateEvent: public Event{
    public:
        AppUpdateEvent(){}

        EVENT_CLASS_TYPE(AppUpdate)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
    class RC_API AppRenderEvent: public Event{
    public:
        AppRenderEvent(){}

        EVENT_CLASS_TYPE(AppRender)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}