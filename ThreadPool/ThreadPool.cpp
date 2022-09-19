#include "ThreadPool.h"

void thread_pool::run() {
    if ( m_running ) return;

    m_running = true;

    for ( int i = 0; i < num_threads; ++i )
    {
        m_threads.push_back( std::thread( &thread_pool::worker_thread, this ) );
    }
}

void thread_pool::stop()
{
    if ( !m_running ) return;

    wait();

    m_running = false;
    m_task_ready.notify_all( );

    for ( auto& thread : m_threads )
    {
        if ( thread.joinable( ) )
        {
            thread.join( );
        }
    }
}

void thread_pool::push( std::function< void(int) > function )
{
    if ( function )
    {
        {
            std::unique_lock< std::mutex > lock( m_task_queue_lock );
            m_task_queue.push( function );
        }

        m_task_ready.notify_one( );
    }
}

void thread_pool::wait()
{
    if ( !m_running )
    {
        return;
    }

    while ( true )
    {
        std::unique_lock< std::mutex > lock( m_task_queue_lock );
        if ( m_task_queue.empty( ) )
        {
            break;
        }

        lock.unlock( );
        std::this_thread::yield( );
    }
}


