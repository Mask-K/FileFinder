//
// Created by Maksym on 18.09.2022.
//
#pragma once
#include "../Core.h"

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
    thread_pool(int size)
    {
        num_threads = size;
        //runner = std::thread(&thread_pool::run, this);
    }

    ~thread_pool( )
    {
        stop();
        std::cout << m_task_queue.size() << std::endl;
    }

    void run( );

    void stop( );

    void push( std::function< void(int) > function );

    void wait( );

private:
    bool fetch_task( std::function<void(int)>& function )
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
    int num_threads;
    std::mutex m_task_queue_lock;
    std::queue< std::function<void(int)> > m_task_queue;
    std::condition_variable m_task_ready;

    std::vector< std::thread > m_threads;
};