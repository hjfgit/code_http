#include "Http_server.hpp"
#include <signal.h>
static void Usage(string proc_)
{
    cout << " Usage: " << proc_ << " port " << endl;
}

int main(int argc, char *argv[])
{   

    signal(SIGPIPE, SIG_IGN);
    if(argc != 2){
        Usage(argv[0]);
        exit(1);
    }
    HttpdServer *serp = new HttpdServer(atoi(argv[1]));
    serp->InitServer();
    serp->Start();

    delete serp;
    return 0;

}

