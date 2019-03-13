#ifndef __PTHREADPOOL___HPP__
#define __PTHREADPOOL___HPP__

#include <queue>
#include <pthread.h>
#include "Log.hpp"

#define NUM 5
typedef int(*Handler)(int);

class Task{
public:
    Task(int sock_ = -1, Handler handler_ = NULL)
        :sock(sock_),handler(handler_)
    {}
    void Run()
    {
        if(handler == NULL)
            return;
        handler(sock);
    }
private:
    int sock;
    Handler handler;

};

class Pthreadpool
{
public:
    Pthreadpool(int number_ = NUM, int idle_number_ = 0)
        :number(number_),
         idle_number(idle_number_),
         is_quit(false)
    {}
    void Lock()
    {
        pthread_mutex_lock(&mutex);
    }

    void UnLock()
    {
        pthread_mutex_unlock(&mutex);
    }
    bool IsEmpty()
    {
        return thread_queue.size() == 0;
    }
    void PthreadIdle()
    {
        if(is_quit){
            UnLock();
            LOG(INFO, "pthread quit");
            number --;
            pthread_exit(0);
        }
        idle_number ++;
        pthread_cond_wait(&cond, &mutex);
        idle_number --;
    }
    void WakeOneUpThread()
    {
        pthread_cond_signal(&cond);
    }
    void WakeAllUpThread()
    {
        pthread_cond_broadcast(&cond);
    }
    void PopTask(Task& t)
    {
        t = thread_queue.front();
        thread_queue.pop();
    }
    void PushTask(Task& t)
    {
        Lock();
        if(is_quit){
            UnLock();
            return;
        }
        LOG(INFO, "push task into queue");
        thread_queue.push(t);
        WakeOneUpThread();
        UnLock();
    }

    static void *thread_routine(void *arg)
    {
        Pthreadpool* tp = (Pthreadpool*)arg;
        pthread_detach(pthread_self());
        LOG(INFO, "thread start run");

        while(1){
            tp->Lock();
            while(tp->IsEmpty()){
                tp->PthreadIdle();
            }
            Task t;
            tp->PopTask(t);
            LOG(INFO, "thread strat task");
            tp->UnLock();
            t.Run();
        }
    }
    void InitPthread()
    {
        LOG(INFO,"thread pool init");
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond, NULL);
        for( int i = 0; i < number; ++i)
        {
            pthread_t tid;
            LOG(INFO, "create one thread");
            pthread_create(&tid,NULL,thread_routine,this);

        }
    }
    void Stop()
    {
        Lock();
        is_quit = true;
        UnLock();
        while(idle_number > 0){
            WakeAllUpThread();
        }
    }
    ~Pthreadpool()
    {
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);
    }

private:
    int number;
    int idle_number;
    std::queue<Task> thread_queue;
    bool is_quit;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

};
#endif 
