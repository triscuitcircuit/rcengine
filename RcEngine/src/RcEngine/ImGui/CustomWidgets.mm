//
// Created by Tristan Zippert on 11/25/21.
//
#include "CustomWidgets.h"
#include "imgui.h"

#import <Cocoa/Cocoa.h>


bool RcEngine::CustomWidget::ColorEdit4(const char* label, float col[4])
{

    typedef NS_ENUM(NSInteger, ColorPickerType) {
        ColorPickerTypeColor = 1002,
        ColorPickerTypeText = 1003,
        ColorPickerTypeStroke = 1004
    };

    NSColorPickerTouchBarItem *colorPickerItem;
    IBOutlet NSButton *customColors;

    ColorPickerType pickerType;

    NSTouchBar *bar = [[NSTouchBar alloc] init];


    static NSTouchBarItemIdentifier ColorPickerItemIdentifier = @"com.TouchBarCatalog.colorPicker";
    static NSTouchBarCustomizationIdentifier ColorPickerCustomizationIdentifier = @"com.TouchBarCatalog.colorPickerViewController";


}