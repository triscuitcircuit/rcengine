//
// Created by Tristan Zippert on 8/11/21.
//
#pragma once
#ifndef RCENGINE_PROCESSORDETECTION_H
#define RCENGINE_PROCESSORDETECTION_H
namespace RcEngine{
    class ProcessorDetectionBase{
    public:
         static std::string getCPUString();
         static int32_t getCPUMaxFreq() ;
    };
}
#endif //RCENGINE_PROCESSORDETECTION_H
