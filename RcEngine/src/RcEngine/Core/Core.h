//
// Created by Tristan Zippert on 5/21/21.
//

#ifdef RC_PLATFORM_WINDOWS
    #ifdef RC_BUILD_DLL
        #define RC_API __declspec(dllexport)
    #else
        #define RC_API __declspec(dllimport)
    #endif
#else
    #ifdef RC_BUILD_DLL
        #define RC_API __attribute__ ((visibility ("default")))
       // #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define RC_API
      //  #define DLL_LOCAL
    #endif
#endif
