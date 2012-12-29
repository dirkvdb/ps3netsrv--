//    Copyright (C) 2012 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef UTILS_WORKER_THREAD_H
#define UTILS_WORKER_THREAD_H

#include <string>
#include <thread>
#include <deque>
#include <list>

#include "log.h"

namespace utils
{
    
class WorkerThread
{
public:
    void start()
    {
        std::lock_guard<std::mutex> lock(m_JobMutex);
        if (!m_Thread)
        {
            m_Stop = false;
            m_Thread.reset(new std::thread(&WorkerThread::workerThread, this));
        }
    }
    
    void stop()
    {
        if (m_Thread)
        {
            m_Stop = true;
            m_JobCondition.notify_all();
            
            if (m_Thread->joinable())
            {
                m_Thread->join();
            }
            
            m_Thread.reset();
        }
    }
    
    void addJob(const std::function<void()>& job)
    {
        if (!m_Stop)
        {
        
            {
                std::lock_guard<std::mutex> lock(m_QueueMutex);
                m_JobQueue.push_back(job);
            }
            
            m_JobCondition.notify_all();
        }
    }
    
    utils::Signal<void(std::exception&)> ErrorOccurred;

private:
    void clearJobs()
    {
        std::lock_guard<std::mutex> lock(m_QueueMutex);
        m_JobQueue.clear();
    }
    
    std::function<void()> getJob()
    {
        std::lock_guard<std::mutex> lock(m_QueueMutex);
        auto func = m_JobQueue.front();
        m_JobQueue.pop_front();
        
        return func;
    }

    void workerThread()
    {
        while (!m_Stop)
        {
            std::unique_lock<std::mutex> lock(m_JobMutex);
            m_JobCondition.wait(lock);
            
            if (m_Stop)
            {
                continue;
            }
            
            while (!m_Stop && !m_JobQueue.empty())
            {
                try
                {
                    getJob()();
                }
                catch (std::exception& e)
                {
                    ErrorOccurred(e);
                }
            }
        }
        
        clearJobs();
    }
    
    bool                                m_Stop;

    std::unique_ptr<std::thread>        m_Thread;
    std::condition_variable             m_JobCondition;
    std::mutex                          m_JobMutex;
    std::mutex                          m_QueueMutex;


    std::deque<const std::function<void()>>   m_JobQueue;
};

}

#endif