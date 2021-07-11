#include "Platform/Mac/MacUtils.h"
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

namespace RcEngine{
    std::string FileDialogs::OpenFile(const char *filter) {
        NSString *convertedString = [[NSString alloc] initWithCString: filter encoding:NSUTF8StringEncoding];
        NSFileManager *fileManager = [NSFileManager defaultManager];

        NSOpenPanel *panel = [NSOpenPanel openPanel];
        panel.canChooseFiles = YES;
        panel.canChooseDirectories = YES;
        panel.canCreateDirectories = YES;


        [panel runModal];
        auto filepath = [panel.URL path];

        [convertedString release];

        return std::string([filepath UTF8String]);
    }
    std::string FileDialogs::SaveFile(const char *filter) {

        NSString *convertedString = [[NSString alloc] initWithCString: filter encoding:NSUTF8StringEncoding];

        NSSavePanel *panel = [NSSavePanel savePanel];
        panel.title = @"Save Scene";
        panel.nameFieldStringValue = @"Scene1.rc";
        panel.showsHiddenFiles = YES;
        panel.canCreateDirectories = YES;

        [panel runModal];

        auto filepath = [panel.URL path];

        [convertedString release];

        return std::string([filepath UTF8String]);
    }
}