//
// Created by Tristan Zippert on 5/13/22.
//

#ifndef RCGAMEENGINE_SOUNDBUFFER_H
#define RCGAMEENGINE_SOUNDBUFFER_H
#include "AL/al.h"
#include "sndfile.h"

namespace RcEngine{
    class SoundBuffer{
    public:
        SoundBuffer(const char* filename);
        ~SoundBuffer();

        ALint Source(){return a_Source;}

        bool isPlaying(){return Playing;}

        void Play();
        void Pause();

        void PlayPause(){
            if(Playing){
                Play();
            }else{
                Pause();
            }
            Playing = !Playing;
        };
        void Stop();

        void UpdateBuffer();

        std::string getPath(){return m_Path;}

        void SetGain(const float& val);
        void Duration(float& val);

    private:
        ALuint a_Source;
        ALint m_state;

        static const int BUFFER_SAMPLES = 8192;
        static const int NUM_BUFFERS = 4;

        ALuint a_Buffers[NUM_BUFFERS];

        SNDFILE * p_sndfile;
        SF_INFO p_Sinfo;
        short* membuf;
        ALenum Format;

        bool Playing = false;

        std::string m_Path;
    };
}
#endif //RCGAMEENGINE_SOUNDBUFFER_H
