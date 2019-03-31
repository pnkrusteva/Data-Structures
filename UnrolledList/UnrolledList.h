#ifndef UNROLLEDLIST_H_INCLUDED
#define UNROLLEDLIST_H_INCLUDED

#include <iostream>
using namespace std;

const int CAPACITY = 10;

template <typename T>
struct Node
{
    int elementsCount;
    T elements[CAPACITY];
    Node<T>* next;

    Node(): elementsCount(0), next(NULL) {}
    Node(int ec, T* e): elementsCount(ec), next(NULL)
    {
        if (elementsCount > CAPACITY)
        {
            elementsCount = 0;
            return;
        }
        for (int i = 0; i < elementsCount; i++)
        {
            elements[i] = e[i];
        }
    }
};

template <typename T>
class UnrolledLinkedList
{
    Node<T>* start;
    Node<T>* last;

public:
    UnrolledLinkedList(): start(NULL), last(NULL) {}
    UnrolledLinkedList(const UnrolledLinkedList& other): start(NULL), last(NULL)
    {
        copyUnrolledList(other);
    }
    UnrolledLinkedList<T>& operator =(const UnrolledLinkedList& other)
    {
        if (this != &other)
        {
            destroyUnrolledList();
            copyUnrolledList(other);
        }
        return *this;
    }
    ~UnrolledLinkedList()
    {
        destroyUnrolledList();
    }

    void toEnd(const T& element)
    {
        if (start == NULL)
        {
            start = new Node<T>();
            last = start;
        }

        addElementToNode(last, element);

        if (last->next != NULL)
        {
            last = last->next;
        }
    }

    void toEndOfNode(int nodeIndex, const T& element)
    {
        Node<T>* p = start;
        int i = nodeIndex;

        while (p != NULL && i > 0)
        {
            p = p->next;
            i--;
        }

        if (i > 0)
        {
            return;
        }

        if (p == last)
        {
            toEnd(element);
        }
        else
        {
            addElementToNode(p, element);
        }
    }

    void toNodeAt(int nodeIndex, int arrayIndex, const T& element)
    {
        Node<T>* p = start;
        int i = nodeIndex;

        while (p != NULL && i > 0)
        {
            p = p->next;
            i--;
        }

        if (i > 0)
        {

            return;
        }
        addElementToNodeAt(p, arrayIndex, element);
    }

    void removeElement(const T& element)
    {
        Node<T>* p = start;

        while (p != NULL)
        {
            removeElemFromArray(p->elements, p->elementsCount, element);
            p = p->next;
        }

        p = start;

        while (p != NULL)
        {
            if (p->elementsCount < CAPACITY/2 && p->next != NULL)
            {
                fillFromNextNode(p);
            }
            p = p->next;
        }
    }

    int countElements()
    {
        int count = 0;

        Node<T>* p = start;

        while (p != NULL)
        {
            count += p->elementsCount;
            p = p->next;
        }

        return count;
    }

    bool empty()
    {
        return start == NULL;
    }

    class Iterator
    {
        Node<T>* nodePtr;
        int index;

    public:
        Iterator(): nodePtr(NULL), index(0) {}

        Iterator(Node<T>* p, int i = 0): nodePtr(p), index(i) {}

        Iterator(const Iterator& other): nodePtr(other.nodePtr), index(other.index) {}

        Iterator operator++()
        {
            nextIter();
            return *this;
        }

        Iterator operator++(int junk)
        {
            Iterator result (nodePtr, index);
            nextIter();
            return result;
        }

        T operator*()
        {
            return nodePtr->elements[index];
        }
        Node<T>* operator->()
        {
            return nodePtr;
        }

        Node<T>* getNodePtr() const
        {
            return nodePtr;
        }

        int getIndex() const
        {
            return index;
        }

        Iterator& operator +(unsigned offset)
        {
            if (index + offset < nodePtr->elementsCount)
            {
                Iterator it = Iterator(nodePtr, index + offset);
                return it;
            }
            Node<T>* p = nodePtr;
            int ind = index;

            for (int i = 0; i < offset; i++)
            {
                if (ind < p->elementsCount - 1)
                {
                    ind++;
                }
                else
                {
                    p = p->next;
                    ind = 0;
                }
            }
            Iterator it = Iterator(p, ind);
            return it;
        }

        bool operator==(const Iterator& other) const
        {
            return nodePtr == other.nodePtr && index == other.index;
        }
        bool operator!=(const Iterator& other) const
        {
            return nodePtr != other.nodePtr || index != other.index;
        }

    private:
        void nextIter()
        {
            if (index < nodePtr->elementsCount - 1)
            {
                index++;
            }
            else
            {
                nodePtr = nodePtr->next;
                index = 0;
            }
        }
    };

    Iterator begin()
    {
        return Iterator(start, 0);
    }
    Iterator end()
    {
        return Iterator(last, last->elementsCount - 1);
    }

    void addElementByIterator(int globalIndex, const T* element)
    {
        Iterator iter = begin() + globalIndex;
        addElementToNodeAt(iter.getNodePtr(), iter.getIndex(), element);
    }

    void removeElementByIterator(int globalIndex)
    {
        Iterator iter = begin() + globalIndex;

        Node<T>* p = iter.getNodePtr();
        int ind = iter.getIndex();

        for (int i = ind; i < p->elementsCount - 1; i++)
        {
            p->elements[i] = p->elements[i+1];
        }
        p->elementsCount--;

        if (p->elementsCount < CAPACITY/2 && p->next != NULL)
        {
            fillFromNextNode(p);
        }
    }

    void print() const
    {
        Node<T>* p = start;

        while (p != NULL)
        {
            printNode(p);
            p = p->next;
        }
    }

private:
    void copyUnrolledList(const UnrolledLinkedList& other)
    {
        start = new Node<T>(other.start->elementsCount, other.start->elements);

        Node<T>* p = start;
        Node<T>* pOther = other.start->next;

        while (pOther != NULL)
        {
            Node<T>* q = new Node<T>(pOther->elementsCount, pOther->elements);
            p->next = q;
            p = p->next;
            pOther = pOther->next;
        }
        last = p;
    }
    void destroyUnrolledList()
    {
        Node<T>* p = start;
        while (p != NULL)
        {
            Node<T>* q = p;
            p = p->next;
            delete q;
        }
    }

    void createNewNode(Node<T>* nodeCreateFrom, T* elements)
    {
        int elemCountFirst = CAPACITY / 2;
        int elemCountSecond = CAPACITY + 1 - elemCountFirst;

        Node<T>* newNode = new Node<T>(elemCountSecond, elements + elemCountFirst);
        newNode->next = nodeCreateFrom->next;
        nodeCreateFrom->next = newNode;
        nodeCreateFrom->elementsCount = elemCountFirst;
    }

    void addElementToNode(Node<T>* nodeToAdd, const T& element)
    {
        if (nodeToAdd->elementsCount < CAPACITY)
        {
            nodeToAdd->elements[nodeToAdd->elementsCount] = element;
            nodeToAdd->elementsCount++;
        }
        else
        {
            T helpArray[CAPACITY + 1];
            for (int i = 0; i < nodeToAdd->elementsCount; i++)
            {
                helpArray[i] = nodeToAdd->elements[i];
            }
            helpArray[nodeToAdd->elementsCount] = element;
            createNewNode(nodeToAdd, helpArray);
        }
    }

    void addElementToNodeAt(Node<T>* nodeToAdd, int index, const T& element)
    {
        if (nodeToAdd->elementsCount < CAPACITY)
        {
            addElementToArrayAt(nodeToAdd->elements, nodeToAdd->elementsCount, index, element);
            nodeToAdd->elementsCount++;
        }
        else
        {
            T helpArray[CAPACITY + 1];
            for (int i = 0; i < nodeToAdd->elementsCount; i++) //all elements to the help array
            {
                helpArray[i] = nodeToAdd->elements[i];
            }
            addElementToArrayAt(nodeToAdd->elements, nodeToAdd->elementsCount, index, element);
            createNewNode(nodeToAdd, helpArray);        //create the new node and distribute the elements of the help array
        }
    }

    void addElementToArrayAt(T* arr, int n, int index, const T& element)
    {
        if (index > n)
        {
            return;
        }

        for (int i = n; i > index; i--)
        {
            arr[i] = arr[i - 1];
        }
        arr[index] = element;
    }

    void removeElemFromArray(T* arr, int& n, const T& element)
    {
        T helpArray[CAPACITY];
        int helpArrayCount = 0;

        for (int i = 0; i < n; i++)
        {
            if (arr[i] != element)
            {
                helpArray[helpArrayCount] = arr[i];
                helpArrayCount++;
            }
        }

        for (int i = 0; i < helpArrayCount; i++) //move the elements from the help array to the given one
        {
            arr[i] = helpArray[i];
        }
        n = helpArrayCount; //the new value for n
    }

    void fillFromNextNode(Node<T>* node)
    {
        int elementsToFillCount = CAPACITY / 2 - node->elementsCount; //now many should we add to reach the half

        Node<T>* nextNode = node->next;

        for (int i = 0; i < elementsToFillCount; i++) //put elements from the next node
        {
            node->elements[node->elementsCount + i] = nextNode->elements[i];
        }

        node->elementsCount += elementsToFillCount; //update the number of elements

        removeFirstElements(nextNode->elements, nextNode->elementsCount, elementsToFillCount); //remove the elements from the next node

        if (nextNode->elementsCount < CAPACITY / 2) //move the other elements if they are less than half of the capacity
        {
            for (int i = 0; i < nextNode->elementsCount; i++) //put elements from the next node
            {
                node->elements[node->elementsCount + i] = nextNode->elements[i];
            }

            node->elementsCount += nextNode->elementsCount;

            //remove the next node
            if (nextNode == last)
            {
                last = node;
                node->next = NULL;
            }
            else
            {
                node->next = nextNode->next;
            }
            delete nextNode;
        }
    }

    void removeFirstElements(T* arr, int& n, int removeElemCount)
    {
        n -= removeElemCount;

        for (int i = 0; i < n; i++)
        {
            arr[i] = arr[i + removeElemCount];
        }
    }

    void printNode(Node<T>* node) const
    {
        cout << "[";

        for (int i = 0; i < node->elementsCount - 1; i++)
        {
            cout << node->elements[i] << ", ";
        }
        cout << node->elements[node->elementsCount - 1] << "]";
    }
};

#endif // UNROLLEDLIST_H_INCLUDED
