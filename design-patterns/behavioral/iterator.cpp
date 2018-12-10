// iterator design pattern
// Iterator provides a way to traverse elements of a collection without touching the implementation details
//

#include <iostream>

template <typename T>
class ListIterator;

// ListNode - An item in the collection
template <typename T>
class ListNode {
public:
    T val;
    ListNode<T> * next;
    ListNode<T> * prev;
    ListNode<T>(T value): val(value), next(nullptr), prev(nullptr) {}
};

// List - a collection of items (ListNode)
template <typename T>
class List {
public:
    List(ListNode<T> * hd): head(hd) {}

    ListNode<T> *head;

    // define begin and end - so we can assign an initial/end iterator values
    ListIterator<T> begin()
    {
        return ListIterator<T>(head);
    }

    ListIterator<T> end()
    {
        return ListIterator<T>(nullptr);
    }
};

template <typename T>
class ListIterator {
public:
    // an item in the collection
    ListNode<T> * current;
    // iterator will be initialized with a specific item in the list (typically collection.begin())
    explicit ListIterator(ListNode<T> * cur): current(cur) {}

    bool operator!= (const ListIterator<T> & other)
    {
        return current != other.current;
    }

    ListIterator<T>& operator++()
    {
        current = current->next;
        return *this;
    }

    ListNode<T>* operator*() { return current; }
};

int main()
{
    ListNode<int> *head = new ListNode<int>(10);
    auto second = new ListNode<int>(12);
    head->next = second;
    second->prev = head;
    auto third = new ListNode<int>(14);
    second->next = third;
    third->prev = head;

    List<int> lst(head);

    for (ListIterator<int> itr = lst.begin(); itr != lst.end(); ++itr) {
        std::cout << (*itr)->val << " --> ";
    }
    std::cout << " NULL" << std::endl;
}

