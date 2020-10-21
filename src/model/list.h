#ifndef _LIST_H
#define _LIST_H

#include <stdexcept>
#include <memory>
#include <unordered_set>
#include <functional>
#include <iostream>

// immutable persistent list data structure (Haskell-style)

template<typename T>
struct Cons {
    const T head;
    const std::shared_ptr<Cons<T>> tail;

    Cons(T head, const std::shared_ptr<Cons<T>>& tail)
        : head(head), tail(tail) { }
    Cons(T head) : head(head), tail(nullptr) { }
};

template<typename T>
class List {
public:
    List() : cons(nullptr) { }
    List(std::initializer_list<T> list) {
        cons = nullptr;
        for(auto it = std::rbegin(list); it != std::rend(list); it++)
            cons = std::make_shared<Cons<T>>(*it, cons);
    }
    List(const std::shared_ptr<Cons<T>>& cons) : cons(cons) { }
    List(const List<T>& list) : cons(list.cons) { }
    void operator=(const List<T>& list) { cons = list.cons; }

    // O(n)
    int size() const {
        Cons<T>* c = cons.get();
        int size = 0;
        while(c) {
            size++;
            c = c->tail.get();
        }
        return size;
    }

    std::shared_ptr<Cons<T>> getCons() const { return cons; }

    // O(1)
    static List<T> push_front(const List<T>& list, const T& val) {
        return List<T>(std::make_shared<Cons<T>>(val, list.cons));
    }

    // O(n)
    static List<T> reverse(const List<T>& list) {
        std::shared_ptr<Cons<T>> result { };
        for(const auto& t : list) {
            result = std::make_shared<Cons<T>>(t, result);
        }
        return result;
    }

    // O(1)
    void push_front(const T& val) {
        cons = std::make_shared<Cons<T>>(val, cons);
    }

    // O(2*n), n = |list|
    void append_front(const List<T>& list) {
        auto rev = reverse(list);
        for(const auto& t : rev)
            push_front(t);
    }

    // O(n), n = |this|
    void append_back(const List<T>& list) {
        List<T> l2 = list;
        l2.append_front(*this);
        cons = l2.getCons();
    }

    // O(n), n = |this|
    void push_back(const T& val) {
        append_back(List<T>{val});
    }

    // O(i)
    const T& operator[](int i) const {
        Cons<T>* ptr = cons.get();
        for(int j = 0; j < i; j++) {
            if(!ptr) throw std::runtime_error("List<T>::operator[] out of range");
            ptr = ptr->tail.get();
        }
        return ptr->head;
    }

    const T& back() const {
        if(!cons) throw std::runtime_error("List<T>::back() called on list with size 0");
        Cons<T>* ptr = cons.get();
        while(ptr->tail) ptr = ptr->tail.get();
        return ptr->head;
    }
    
    struct const_iterator {
        Cons<T>* ptr;
        const_iterator(Cons<T>* ptr) : ptr(ptr) { }
        bool operator==(const_iterator other) { return ptr == other.ptr; }
        bool operator!=(const_iterator other) { return ptr != other.ptr; }
        void operator++() { if(ptr) ptr = ptr->tail.get(); }
        T operator*() { if(ptr) return ptr->head; throw std::runtime_error("List<T>::iterator::operator*: dereferenced invalid iterator"); }
        const T* operator->() { if(ptr) return &ptr->head; else return nullptr; }
        using difference_type = int;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::forward_iterator_tag;
    };
    const_iterator begin() const { return const_iterator(cons.get()); }
    const_iterator end() const { return const_iterator(nullptr); }

    // More complex operations

    template<typename K>
    static List<T> append_unique(const List<T>& list1, const List<T>& list2, const std::function<K(const T&)>& elementToKey) {

        std::unordered_set<K> elementsFromL1;
        std::unordered_set<K> elementsFromL2;
        Cons<T>* l1Cons = list1.cons.get();
        Cons<T>* l2Cons = list2.cons.get();

        if(!l1Cons) return list2;
        if(!l2Cons) return list1;
        
        K lastKey;
        bool found = false;

        while(true) {
            if(!l1Cons && !l2Cons) break;

            if(l1Cons) {
                K key1 = elementToKey(l1Cons->head);

                if(elementsFromL2.find(key1) != elementsFromL2.end()) {
                    lastKey = key1;
                    found = true;
                    break;
                } else elementsFromL1.insert(key1);
            }
            if(l2Cons) {
                K key2 = elementToKey(l2Cons->head);

                if(elementsFromL1.find(key2) != elementsFromL1.end()) {
                    lastKey = key2;
                    found = true;
                    break;
                } else elementsFromL2.insert(key2);
            }

            if(l1Cons) l1Cons = l1Cons->tail.get();
            if(l2Cons) l2Cons = l2Cons->tail.get();
        }
        
        // list1 | shared | list2

        List<T> result;

        if(!found) {
            result = list2;
            result.append_front(list1);
        }
        else {
            result = List<T>::reverse(list1);
            for(auto e : list2) {
                if(elementToKey(e) == lastKey)
                    break;
                result.push_front(e);
            }
            result = List<T>::reverse(result);
        }

        
        // std::cerr << "==================\nAppend list1\n";
        // for(auto e : list1) {
        //     std::cerr << elementToKey(e) << " ";
        // }
        // std::cerr << "\n To list2:\n";
        // for(auto e : list2) {
        //     std::cerr << elementToKey(e) << " ";
        // }
        // std::cerr << "\n=== RESULT (found:" << found << "), key:" << lastKey << "\n";
        // for(auto e : result) {
        //     std::cerr << elementToKey(e) << " ";
        // }
        // std::cerr << "\n=================="<<std::endl;
        

        return result;
    }

private:
    std::shared_ptr<Cons<T>> cons;
};

#endif