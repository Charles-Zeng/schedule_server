//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE  
///    Copyright (c) 2014, iNOKNOK  
///    All rights reserved.  
///  
/// @file		synclist.h
/// @brief	线程安全链表头文件
///  
///	封装TFS文件系统操作
///  
/// @version	1.0
/// @author	谢崇竹
/// @date		2014.7.13
//////////////////////////////////////////////////////////////////////////


#ifndef _SYNC_LIST_H_
#define _SYNC_LIST_H_

#include <list>
#include <pthread.h>

using namespace std;


/** 
* @class CSyncList
* @brief 线程安全链表类
*/
template<typename T>
class CSyncList
{
public:
	///构造函数
	CSyncList()
		:m_cur_size(0), m_max_size(max_size)
	{
		pthread_mutex_init(&m_mutex, NULL);
	}
	///析构函数
	virtual ~CSyncList()
	{
		pthread_mutex_destroy(&m_mutex);
	}
	///设置队列最大容量
	///@param	max_size	最大容量
	void set_max_size(size_t max_size)
	{
		m_max_size = max_size;
	}
	///压入队列操作
	///@param	t	要压入队列的对象
	///@return	成功或失败
	bool push(const T& t, bool front = false)
	{
		pthread_mutex_lock(&m_mutex);
		if(m_cur_size >= m_max_size)
		{
			pthread_mutex_unlock(&m_mutex);
			return false;
		}
		if(front)
		{
			m_list.push_front(t);
		}
		else
		{
			m_list.push_back(t);
		}
		m_cur_size++;
		pthread_mutex_unlock(&m_mutex);
		return true;
	}
	///从队列中取出数据
	///@param	t	取出的数据副本
	///@return	成功或失败
	bool pop(T& t)
	{
		pthread_mutex_lock(&m_mutex);
		if(!m_cur_size)
		{
			pthread_mutex_unlock(&m_mutex);
			return false;
		}
		t = m_list.front();
		m_list.pop_front();
		m_cur_size--;
		pthread_mutex_unlock(&m_mutex);
		return true;
	}
protected:
private:
	///队列当前大小
	volatile size_t m_cur_size;
	///队列最大容量
	volatile size_t m_max_size;
	///队列链表
	list<T> m_list;
	///互斥锁
	pthread_mutex_t m_mutex;
	///队列默认大小
	static const size_t max_size = 100000;
};

#endif//_SYNC_LIST_H_

