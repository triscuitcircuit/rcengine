//
// Created by Tristan Zippert on 6/23/22.
//

#ifndef RCGAMEENGINE_SOUNDDEVICE_H
#define RCGAMEENGINE_SOUNDDEVICE_H
#include "AL/alc.h"


#define SD_INIT SoundDEvice::Init()
#define LISTENER SoundDevice::Get()

namespace RcEngine{
    class SoundDevice{
    public:
        static SoundDevice* Get();
        static void Init();

        void GetLocation(float &x, float &y, float &z);

        void GetOrientation(float &ori);
        float GetGain();

        void SetLocation(const float &x, const float &y, const float &z);

        void SetGain(const float& val);

    private:
        SoundDevice();
        ~SoundDevice();

        ALCdevice* p_ALCDevice;
        ALCcontext* p_ALCContext;
    };
}

#endif //RCGAMEENGINE_SOUNDDEVICE_H
