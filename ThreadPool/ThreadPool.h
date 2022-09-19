//
// Created by Maksym on 18.09.2022.
//

#ifndef FILEFINDER_THREADPOOL_H
#define FILEFINDER_THREADPOOL_H
#pragma once
#include "../library.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>
#include <iostream>

class thread_pool
{
public:
    thread_pool( )
    {
        //runner = std::thread(&thread_pool::run, this);
    }

    ~thread_pool( )
    {
        stop();
        std::cout << m_task_queue.size() << std::endl;
    }

    void run( );

    void stop( );

    void push( std::function< void(Finder*, int) > function );

    void wait( );

    void set_num_threads(int a){
        num_threads = a;
    }

private:
    bool fetch_task( std::function<void(Finder*, int)>& function )
    {
        if (!m_task_queue.empty())
        {
            function = std::move(m_task_queue.front());
            m_task_queue.pop();
            return true;
        }

        return false;
    }

    void worker_thread( )
    {
        while ( m_running )
        {
            std::unique_lock< std::mutex > guard( m_task_queue_lock );
            m_task_ready.wait( guard, [&]( ) {
                return !m_task_queue.empty( ) || !m_running;
            } );

            if ( m_running )
            {
                std::function<void(int)> task;
                if ( fetch_task( task ) )
                {
                    guard.unlock( );

                    task( );
                }
            }
        }
    }

private:
    std::thread runner;
    std::atomic_bool m_running = false;
    int num_threads = 8;
    std::mutex m_task_queue_lock;
    std::queue< std::function<void(Finder*, int)> > m_task_queue;
    std::condition_variable m_task_ready;

    std::vector< std::thread > m_threads;
};

#endif //FILEFINDER_THREADPOOL_H
