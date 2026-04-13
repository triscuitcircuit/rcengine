//
// Created by Tristan Zippert on 5/13/22.
//

#include "SoundBuffer.h"
#include <cstddef>
#include <malloc/malloc.h>
#include <AL/alext.h>
#include "RcEngine/Core/Core.h"

namespace RcEngine {
    SoundBuffer::SoundBuffer(const char *filename): m_Path(filename) {


        alGenSources(1,&a_Source);

        alGenBuffers(NUM_BUFFERS,a_Buffers);

        std::size_t  frame_size;

        p_sndfile = sf_open(filename,SFM_READ,&p_Sinfo);
        RC_CORE_ASSERT(p_sndfile,"Could not open provided music file");

        switch (p_Sinfo.channels) {
            case 1:
                Format = AL_FORMAT_MONO16;
                break;
            case 2:
                Format = AL_FORMAT_STEREO16;
                break;
            case 3:
                if(sf_command(p_sndfile, SFC_WAVEX_GET_AMBISONIC,NULL,0)== SF_AMBISONIC_B_FORMAT)
                    Format = AL_FORMAT_BFORMAT2D_16;
                break;
            case 4:
                if(sf_command(p_sndfile, SFC_WAVEX_GET_AMBISONIC,NULL,0)== SF_AMBISONIC_B_FORMAT)
                    Format = AL_FORMAT_BFORMAT3D_16;
                break;
            break;
        }
        if(!Format){
            sf_close(p_sndfile);
            p_sndfile = nullptr;
            RC_ERROR("Unsupported channel from file");
        }

        frame_size =((size_t)BUFFER_SAMPLES * (size_t)p_Sinfo.channels * sizeof(short));
        membuf = static_cast<short*>(malloc(frame_size));
    }
    SoundBuffer::~SoundBuffer() {
        alDeleteSources(1,&a_Source);
        if(p_sndfile)
            sf_close(p_sndfile);
        p_sndfile = nullptr;
        free(membuf);
        alDeleteBuffers(NUM_BUFFERS,a_Buffers);
    }

    void SoundBuffer::UpdateBuffer() {
        ALint processed, state;

        alGetSourcei(a_Source,AL_SOURCE_STATE,&state);
        alGetSourcei(a_Source,AL_BUFFERS_PROCESSED,&processed);

        while(processed > 0){
            ALuint  bufid;

            sf_count_t slen;

            alSourceUnqueueBuffers(a_Source,1,&bufid);
            processed--;

            slen = sf_readf_short(p_sndfile,membuf,BUFFER_SAMPLES);
            if(slen > 0){
                slen *= p_Sinfo.channels * (sf_count_t)sizeof(short);
                alBufferData(bufid,Format,membuf,(ALsizei)slen,p_Sinfo.samplerate);
                alSourceQueueBuffers(a_Source,1,&bufid);
            }
            RC_ASSERT(alGetError() == AL_NO_ERROR,"Error buffering music Data");
        }
        if(state != AL_PLAYING && state != AL_PAUSED){
            ALint queued;

            alGetSourcei(a_Source,AL_BUFFERS_QUEUED,&queued);
            if(!queued)
                return;
            alSourcePlay(a_Source);
        }
    }

    void SoundBuffer::Play() {
        ALsizei  i;

        RC_INFO(alGetError());

        alSourceRewind(a_Source);
        alSourcei(a_Source,AL_BUFFER,0);

        for (i =0; i < NUM_BUFFERS; ++i) {
            sf_count_t  slen = sf_readf_short(p_sndfile,membuf,BUFFER_SAMPLES);
            if(slen < 1) break;

            slen *= p_Sinfo.channels * (sf_count_t)sizeof(short);

            alBufferData(a_Buffers[i],Format,membuf,(ALsizei)slen, p_Sinfo.samplerate);
        }
        RC_CORE_INFO(alGetError());

//        RC_ASSERT(alGetError() == AL_NO_ERROR, "Error Buffering playback");

        alSourceQueueBuffers(a_Source,i,a_Buffers);
        alSourcePlay(a_Source);

       RC_ASSERT(alGetError() == AL_NO_ERROR, "Error Starting playback");

        m_state = AL_PLAYING;
        std::thread myThread([&](){
            while(m_state == AL_PLAYING){
                UpdateBuffer();
                alGetSourcei(a_Source,AL_SOURCE_STATE, &m_state);
            }
        });
        myThread.detach();

    }
    void SoundBuffer::Pause() {
        alSourcePause(a_Source);
    }

    void SoundBuffer::Stop() {
        alSourceStop(a_Source);
    }

    void SoundBuffer::SetGain( const float &val) {
        float newval = val;
        if(newval < 0)
            newval = 0;
        alSourcef(a_Source,AL_GAIN,val);
    }

    void SoundBuffer::Duration(float &val) {
        return;
    }

}
