//
// Created by Tristan Zippert on 6/16/21.
//
#pragma once
#ifndef RCENGINE_CLION_TIMESTEP_H
#define RCENGINE_CLION_TIMESTEP_H

namespace RcEngine{
    class Timestep{
    public:
        Timestep(float time=0.0f)
            :m_Time(time){

        }
        float GetSeconds() const {return m_Time;}
        float GetMiliseconds() const {return m_Time * 1000.0f;}
        operator float() const {return m_Time;}
    private:
        float m_Time;
    };
}


#endif //RCENGINE_CLION_TIMESTEP_H
