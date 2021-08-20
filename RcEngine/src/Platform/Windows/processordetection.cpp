//
// Created by Tristan Zippert on 8/13/21.
//

#pragma once
#include "Platform/processordetection.h"

namespace RcEngine{
    std::string ProcessorDetectionBase::getCPUString() {
        std::string vendor;
        int reg[4];
        __asm__ __volatile__ ("mov $0x80000002 , %eax\n\t");
        __asm__ __volatile__ (
                "cpuid"
                : "=a"(reg[0]),"=b"(reg[1]),"=c"(reg[2]),"=d"(reg[3])
        );
        for (int & i : reg) {
            vendor += std::string((const char *)&i, 4);
        }
        return vendor;
    }

    int32_t ProcessorDetectionBase::getCPUMaxFreq() {
        return 0;
    }
}