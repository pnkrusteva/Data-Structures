#ifndef INCLUDE_DEQUE_INCLUDE
#define INCLUDE_DEQUE_INCLUDE
#include<iostream>
using namespace std;

template<typename T>
struct Node
{
	T data;
	Node<T>* next;
	Node<T>(T inf):next(NULL),data(inf)
	{}
};
template<typename T>
class Deque
{
public:
	
	Deque() :rear(NULL), front(NULL)
	{
	}

	Deque<T>& operator=(const Deque<T>& other)
	{
		if (this != &other)
		{
			destroy();
			copy(other);
		}
		return *this;
	}
	
	Deque(const Deque<T>& other)
	{
		copy(other);
	}

	~Deque()
	{
		destroy();
	}
	
	bool empty()const
	{
		return front == NULL && rear == NULL;
	}

	
	void pushBack(T element)
	{
		if (empty())
		{
			front = new Node<T>(element);
			rear = front;
			return;
		}
		Node<T>* p = new Node<T>(element);
		rear->next = p;
		rear = p;
	}
	
	void popBack()
	{
		if (empty())
		{
			return;
		}
		Node<T>*p = front;
		while (p->next->next != NULL)
		{
			p = p->next;
		}
		rear = p;
		delete p->next;
	}
	
	void pushFront(T element)
	{
		if (empty())
		{
			front = new Node<T>(element);
			rear = front;
			return;
		}
		Node<T>*p = new Node<T>(element);
		p->next = front;
		front = p;
	}
	
	void popFront()
	{
		if (empty())
		{
			return;
		}
		Node<T>* p = front;
		if (front == rear)
		{
			front = rear = NULL;
		}
		else
		{
			front = front->next;
		}
		delete p;
		p = NULL;
	}
	
	T getFront()const
	{
		if (empty())return;
		return front->data;
	}

	T getBack()const
	{
		if (empty())return;
		return rear->data;
	}
	
	unsigned size()const
	{
		if (empty())
		{
			return 0;
		}
		Node<T>* p = front;
		unsigned counter = 0;
		while (p != NULL)
		{
			counter++;
			p = p->next;
		}
		return counter;
	}
private:

	
	void copy(const Deque<T>& other)
	{
		Node<T>* q = other.front;
		while (q != NULL)
		{
			pushBack(q->data);
			q = q->next;
		}
	}

	
	void destroy()
	{
		while (!empty())
		{
			popFront();
		}
	}
	Node<T>* front;
	Node<T>* rear;
};
#endif //INCLUDE_DEQUE_INCLUDE
