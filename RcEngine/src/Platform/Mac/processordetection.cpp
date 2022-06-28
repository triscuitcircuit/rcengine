//
// Created by Tristan Zippert on 8/12/21.
//
#pragma once

#include "Platform/processordetection.h"
#include <sys/sysctl.h>
namespace RcEngine{
    template< typename T>
    int getCTLValue(const char key[], T * dest){
        size_t len =0;
        int err;

        err = sysctlbyname(key,nullptr,&len, nullptr,0);
        if(!err){
            RC_CORE_ASSERT((len == sizeof(T)), "Mis-matched destination type for SYSCTL() read.\n");
            err = sysctlbyname(key,dest,&len, nullptr,0);
        }
        return err;
    }

    std::string ProcessorDetectionBase::getCPUString() {
        char buffer[64];
        size_t bufferlen = 64;
        sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferlen, nullptr, 0);
        return buffer;
    }

    int32_t ProcessorDetectionBase::getCPUMaxFreq() {
        int32_t freq;
        size_t bufferlen = sizeof(freq);
        // TODO: deal with M1 issue
        sysctlbyname("hw.cpufrequency_max", &freq, &bufferlen, nullptr, 0);
        return freq;
    }
}
