#ifndef __QUEUE_THREAD_HPP__
#define __QUEUE_THREAD_HPP__

template< class T>
class QueueThread
{
public:
    virtual ~QueueThread()
        {
        }
public:
    static QueueThread* getInstance()
        {
            if(NULL == instance)
            {
                instance = new QueueThread();
            }else{
                // pass
            }
            return instance;
        }
    size_t initialLength(size_t num)
        {
            for (size_t i = 0; i < num; ++i)
            {
                T* c = new T();
                if (NULL == c)
                {
                    continue;
                }

                connQueue.push(c);
                ++ length;
            }
            return length;
        }
    bool push(T* c)
        {
            bool ret = true;

            pthread_lock(&lock);
            dataQueue.push(c);
            pthread_unlock(&lock);
        }
    T* pop(){
        pthread_lock(lock);
        T* item = item_queue.pop();            
        pthread_unlock(lock);
        return item;        
    }

private:
    QueueThread(){};
    pthead_lock_t lock;
    queue< T*> dataQueue;
    size_t length;
    static QueueThread* instance;
};

#endif
