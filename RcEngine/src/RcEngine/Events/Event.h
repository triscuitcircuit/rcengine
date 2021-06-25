//
// Created by Tristan Zippert on 5/22/21.
//
#pragma once

#include "RcEngine/Core/Core.h"
#include <src/rcpch.h>

namespace RcEngine{

    // Events arent buffered in any way and use a blocking system, in the future it should be
    // put into a Queue system

    enum class EventType{
        None=0, WindowClose, WindowResize, WindowOnFocus,WindowOnLostFocus,WindowMoved,
        ApplicationTick, AppUpdate, AppRender, OnKeyPressed, OnKeyRelease, OnKeyTyped,
        MouseButtonPress, MouseButtonRelease, MouseMoved, MouseScrolled,
    };

    // Bit-Shifted constant field values for bit masking
    enum EventCategory{
        None = 0x00,
        EventCategoryApplication = 0x01,
        EventCategoryInput = 0x02,
        EventCategoryKeyboard = 0x04,
        EventCategoryMouse = 0x08,
        EventCategoryMouseButton= 0x10,
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::type;} \
                  virtual EventType  GetEventType() const override {return GetStaticType(); } \
                  virtual const char*  GetName() const override {return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

    class RC_API Event{
        friend class EventDispatcher;
    public:
        virtual ~Event() = default;

        //  Virtual for functions that have to be implemented
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const {return GetName();}

        // Bitwise & used to check bitposition of EventCategory
        inline bool IsInCategory(EventCategory category){
            return GetCategoryFlags() & category;
        }

        // check if event is already handled
        bool m_Handeled = false;
    };

    class EventDispatcher{
        template<typename A>
        using EventFn = std::function<bool(A&)>;
    public:
        EventDispatcher(Event& event)
            :m_Event(event){}

        template<typename T,typename A>
        bool Dispatch(const A& func){
            if(m_Event.GetEventType() == T::GetStaticType()){
                m_Event.m_Handeled = func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };
    inline std::ostream& operator<<(std::ostream& os, const Event& e){
        return os << e.ToString();
    }

}