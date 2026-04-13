//
// Created by Tristan Zippert on 6/20/21.
//
extern "C" void rc_tcp_server();
extern "C" void rc_tcp_client();
extern "C" void rc_entry();

#pragma once

#ifndef RCENGINE_CLION_NETWORK_H
#define RCENGINE_CLION_NETWORK_H

namespace RcEngine{
    static void TcpServer(){
        rc_tcp_server();
    }
    static void TcpClient(){
        rc_tcp_client();
    }

}
#endif //RCENGINE_CLION_NETWORK_H
