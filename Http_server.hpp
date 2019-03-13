#ifndef __HTTPD_SERVER_HPP__
#define __HTTPD_SERVER_HPP__
#include <pthread.h>
#include "Protocolutil.hpp"
#include "Pthreadpool.hpp"

class HttpdServer{
private:
    int listen_sock;
    int port;
    Pthreadpool *tp;
public:
    HttpdServer(int port_):listen_sock(-1),port(port_),tp(NULL)
    {}
    void InitServer()
    {
        listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(listen_sock < 0){
            LOG(ERROR, " create socket error!");
            exit(2);
        }
        int opt = 1;
        setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;

        if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0){
            LOG(ERROR, " bind socket error!");
            exit(3);
        }

        if(listen(listen_sock, 5) < 0){
            LOG(ERROR, " listen socket error!");
            exit(4);
        }
        LOG(INFO, " InitServer success!");
        tp = new Pthreadpool;
        tp->InitPthread(); 
    }
    void Start()
    {
        LOG(INFO, " Start Server Begin!");
        for( ; ;){
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
            if(sock < 0 ){
                LOG(WARNING, " accept error");
                continue;
            }
            LOG(INFO, "Get New Clinet, Create Thread Handler Rquest...");
            Task* t = new Task(sock,Entry::HandelRequest);
            tp->PushTask(*t);
            // pthread_t tid;
            // int *sockp = new int(sock);
            // pthread_create(&tid, NULL, Entry::HandelRequest, (void*)sockp);
        }
    }
    ~HttpdServer()
    {
        if(listen_sock != -1){
            close(listen_sock);
        }
        if(tp != NULL){
            delete tp;
            tp->Stop();
                                            
        }
        port = -1;
    }
};
#endif


