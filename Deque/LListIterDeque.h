#ifndef LLISTITERDEQUE_H_INCLUDED
#define  LLISTITERDEQUE_H_INCLUDED

#include"LListIter.h"
#include "Deque.h"

void sortLListiterDeque(LListIter<Deque<int>> list)
{
	list.sort();
}
void insertAtSortedList(LListIter<Deque<int>> list, Deque<int> elem)
{
	LListIter<Deque<int>>::Iterator l(list.begin),m(list.end);
	if (elem < l.operator*) {
		list.insertAt(elem, 0); return;
	}
	if (m.operator* < elem) {
		list.insertAt(elem, list.size()); return;
	}

	m = l;
	m++;
	int index = 0;
	while (elem < m.operator*&&l.operator*<=elem)
	{
		index++;
		l++;
		m++;
	}
	list.insertAt(elem, index);

	
}
#endif //LLISTITERDEQUE_H_INCLUDED