//
// Created by Tristan Zippert on 8/12/21.
//
#pragma once

#include "Platform/processordetection.h"
#include <sys/sysctl.h>
namespace RcEngine{

    std::string ProcessorDetectionBase::getCPUString() {
        char buffer[64];
        size_t bufferlen = 64;
        sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferlen, nullptr, 0);
        return buffer;
    }

    int32_t ProcessorDetectionBase::getCPUMaxFreq() {
        int32_t freq;
        size_t bufferlen = sizeof(freq);
        sysctlbyname("hw.cpufrequency_max", &freq, &bufferlen, nullptr, 0);
        return freq;
    }
}
