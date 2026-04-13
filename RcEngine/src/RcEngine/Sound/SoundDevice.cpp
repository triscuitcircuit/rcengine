//
// Created by Tristan Zippert on 6/23/22.
//

#include "SoundDevice.h"
#include <AL/al.h>
#include <stdio.h>
#include <vector>
#include "RcEngine/Core/Core.h"



namespace RcEngine{
    static SoundDevice* _instance = nullptr;

    SoundDevice::SoundDevice() {
        p_ALCDevice = alcOpenDevice(nullptr);

        RC_ASSERT(p_ALCDevice,"Failed to get Sound Device");

        p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
        RC_ASSERT(p_ALCContext,"Failed to set context");

        RC_ASSERT(alcMakeContextCurrent(p_ALCContext),"Failed to make context current");

        const ALCchar * name = nullptr;
        if(alcIsExtensionPresent(p_ALCDevice,"ALC_ENUMERATE_ALL_EXT"))
            name = alcGetString(p_ALCDevice,ALC_ALL_DEVICES_SPECIFIER);
        if(!name|| alcGetError(p_ALCDevice)!= AL_NO_ERROR)
            name = alcGetString(p_ALCDevice,ALC_DEVICE_SPECIFIER);
        RC_INFO("Opened %s",name);

    }
    SoundDevice::~SoundDevice() {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(p_ALCContext);
        alcCloseDevice(p_ALCDevice);
    }
    float SoundDevice::GetGain() {
        float curr_gain;
        alGetListenerf(AL_GAIN,&curr_gain);
        return curr_gain;
    }
    void SoundDevice::GetOrientation(float &ori) {
        alGetListenerfv(AL_ORIENTATION,&ori);
    }
    void SoundDevice::GetLocation(float &x, float &y, float &z) {
        alGetListener3f(AL_POSITION,&x,&y,&z);
    }
    void SoundDevice::SetLocation(const float &x, const float &y, const float &z) {
        alListener3f(AL_POSITION,x,y,z);
    }

    void SoundDevice::SetGain(const float &val) {
        float newVol = val;
        if (newVol < 0.f)
            newVol = 0.f;
        else if (newVol > 5.f)
            newVol = 5.f;

        alListenerf(AL_GAIN, newVol);
    }
    SoundDevice* SoundDevice::Get(){
        Init();
        return _instance;
    }
    void SoundDevice::Init() {
        if(_instance == nullptr)
            _instance = new SoundDevice();
    }

}