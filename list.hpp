#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        node(const T &value, node *p = nullptr, node *n = nullptr)
            : data(new T(value)), prev(p), next(n) {}
        ~node() {
            delete data;
        }
    };

protected:
    node *head;
    node *tail;
    size_t list_size;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->next = pos;
        cur->prev = pos->prev;
        pos->prev->next = cur;
        pos->prev = cur;
        list_size++;
        return cur;
    }
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        list_size--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    protected:
        node *ptr;
        const list *container;

    public:
        iterator(node *p = nullptr, const list *c = nullptr) : ptr(p), container(c) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            if (ptr == nullptr || ptr == container->tail) {
                throw invalid_iterator();
            }
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            if (ptr == nullptr || ptr == container->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            if (ptr == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            if (ptr == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const;
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const;

        friend class const_iterator;
        friend class list;
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    protected:
        const node *ptr;
        const list *container;

    public:
        const_iterator(const node *p = nullptr, const list *c = nullptr) : ptr(p), container(c) {}
        const_iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        const_iterator operator++(int) {
            if (ptr == nullptr || ptr == container->tail) {
                throw invalid_iterator();
            }
            const_iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        const_iterator & operator++() {
            if (ptr == nullptr || ptr == container->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator--(int) {
            if (ptr == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            const_iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        const_iterator & operator--() {
            if (ptr == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }
        const T & operator *() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        const T * operator ->() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class iterator;
        friend class list;
    };
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        list_size = 0;
    }
    list(const list &other) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        list_size = 0;

        for (node *cur = other.head->next; cur != other.tail; cur = cur->next) {
            push_back(*(cur->data));
        }
    }
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        for (node *cur = other.head->next; cur != other.tail; cur = cur->next) {
            push_back(*(cur->data));
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(head->next->data);
    }
    const T & back() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(tail->prev->data);
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head->next, this);
    }
    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(tail, this);
    }
    const_iterator cend() const {
        return const_iterator(tail, this);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return list_size == 0;
    }
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return list_size;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (!empty()) {
            pop_front();
        }
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) {
            throw invalid_iterator();
        }
        node *new_node = new node(value);
        insert(pos.ptr, new_node);
        return iterator(new_node, this);
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty() || pos.container != this || pos.ptr == head || pos.ptr == tail) {
            throw invalid_iterator();
        }
        node *next_node = pos.ptr->next;
        erase(pos.ptr);
        delete pos.ptr;
        return iterator(next_node, this);
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        insert(tail, new node(value));
    }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) {
            throw container_is_empty();
        }
        node *to_delete = tail->prev;
        erase(to_delete);
        delete to_delete;
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        insert(head->next, new node(value));
    }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) {
            throw container_is_empty();
        }
        node *to_delete = head->next;
        erase(to_delete);
        delete to_delete;
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (size() <= 1) return;

        // Use selection sort - simple and doesn't require extra memory
        for (node *cur = head->next; cur != tail; cur = cur->next) {
            node *min_node = cur;
            for (node *inner = cur->next; inner != tail; inner = inner->next) {
                if (*(inner->data) < *(min_node->data)) {
                    min_node = inner;
                }
            }
            if (min_node != cur) {
                // Swap data pointers
                T *temp = cur->data;
                cur->data = min_node->data;
                min_node->data = temp;
            }
        }
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;

        node *cur1 = head->next;
        node *cur2 = other.head->next;

        while (cur1 != tail && cur2 != other.tail) {
            if (*(cur2->data) < *(cur1->data)) {
                // Insert cur2 before cur1
                node *next2 = cur2->next;
                cur2->prev->next = cur2->next;
                cur2->next->prev = cur2->prev;

                cur2->prev = cur1->prev;
                cur2->next = cur1;
                cur1->prev->next = cur2;
                cur1->prev = cur2;

                cur2 = next2;
            } else {
                cur1 = cur1->next;
            }
        }

        // Append remaining elements from other
        if (cur2 != other.tail) {
            tail->prev->next = cur2;
            cur2->prev = tail->prev;
            other.tail->prev->next = tail;
            tail->prev = other.tail->prev;
        }

        // Reset other list
        other.head->next = other.tail;
        other.tail->prev = other.head;
        list_size += other.list_size;
        other.list_size = 0;
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        node *cur = head;
        while (cur != nullptr) {
            node *tmp = cur->next;
            cur->next = cur->prev;
            cur->prev = tmp;
            cur = tmp;
        }
        // Swap head and tail
        node *tmp = head;
        head = tail;
        tail = tmp;
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (size() <= 1) return;

        node *cur = head->next;
        while (cur != tail && cur->next != tail) {
            if (*(cur->data) == *(cur->next->data)) {
                node *to_delete = cur->next;
                cur->next = to_delete->next;
                to_delete->next->prev = cur;
                delete to_delete;
                list_size--;
            } else {
                cur = cur->next;
            }
        }
    }
};

// Implementation of iterator comparison operators
template<typename T>
bool list<T>::iterator::operator==(const const_iterator &rhs) const {
    return ptr == rhs.ptr;
}

template<typename T>
bool list<T>::iterator::operator!=(const const_iterator &rhs) const {
    return ptr != rhs.ptr;
}

}

#endif //SJTU_LIST_HPP