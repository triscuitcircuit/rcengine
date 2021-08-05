//
// Created by Tristan Zippert on 7/10/21.
//
#include "Platform/MacUtils.h"
#import <AppKit/AppKit.h>

namespace RcEngine{
    std::string FileDialogs::OpenFile(const char *filter) {
        NSString *convertedString = [[NSString alloc] initWithCString: filter encoding:NSUTF8StringEncoding];

        // Sets modal properties before  window is shown
        // Generic NSOpenPanel is being used to pick files
        NSOpenPanel *panel = [NSOpenPanel openPanel];
        panel.canChooseFiles = YES;
        panel.canChooseDirectories = YES;
        panel.canCreateDirectories = YES;

        // Captures response value from "modal" window
        NSModalResponse response = [panel runModal];

        //[convertedString release];
        if(response == NSModalResponseOK){
            //If okay response return filepath
            return std::string([[panel.URL path] UTF8String]);
        }else{
            // else return empty string
            return std::string([@"" UTF8String]);
        }
    }
    std::string FileDialogs::SaveFile(const char *filter) {

        NSString *convertedString = [[NSString alloc] initWithCString: filter encoding:NSUTF8StringEncoding];

        // Sets modal properties before window is shown
        //The window type specifically is a NSSavePanel
        NSSavePanel *panel = [NSSavePanel savePanel];
        panel.title = @"Save Scene As...";
        panel.nameFieldStringValue = @"Scene1.rc";
        panel.showsHiddenFiles = YES;
        panel.canCreateDirectories = YES;

        // Captures response value from "modal" window
        NSModalResponse response = [panel runModal];

        //[convertedString release];
        if(response == NSModalResponseOK){
            //If okay response return filepath
            return std::string([[panel.URL path] UTF8String]);
        }else{
            // else return empty string
            return std::string([@"" UTF8String]);
        }

    }
}