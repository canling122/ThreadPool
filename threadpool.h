#ifndef _THREADPOOL_H
#define _THREADPOOL_H

//taskqueue struct
typedef struct Task{
	void (*function)(void* arg);
	void* arg;
}Task;

//threadpool struct
struct ThreadPool
{

	Task* taskQ; //任务队列
	int queueCapacity; //容量
	int queueSize; //当前任务个数
	int queueFront; //取数据
	int queueRear; //存数据
	pthread_cond_t notFull; //判断queue是否为满
	pthread_cond_t notEmpty; //判断queue是否为空

	pthread_t managerID; //管理者线程ID
	pthread_t* threadIDs; //工作线程ID
	int minNum; //最小线程量
	int maxNum; //最大线程量
	int busyNum; //忙碌线程数
	int liveNum; //存活线程数
	int exitNum; //销毁线程数

	pthread_mutex_t mutexPool; //锁整个线程池
	pthread_mutex_t mutexBusy; //锁busyNum
	

	int shutdown; //是否销毁线程池，1销毁，0反之
};


typedef struct ThreadPool ThreadPool;
//创建线程池并初始化
ThreadPool* threadPoolCreate(int min, int max, int queueSize);

//销毁线程池
int threadPoolDestroy(ThreadPool* pool);

//给线程池添加任务
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg);

//获取线程池中工作的线程个数
int threadPoolBusyNum(ThreadPool* pool);

//获取线程池中活着的线程的个数
int threadPoolAliveNum(ThreadPool* pool);

//工作线程的任务函数
void* worker(void* arg);

//管理者线程任务函数
void* manager(void* arg);

//单个线程退出
void threadExit(ThreadPool* pool);
#endif