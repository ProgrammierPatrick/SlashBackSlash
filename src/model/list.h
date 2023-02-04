#ifndef _LIST_H
#define _LIST_H

#include <stdexcept>
#include <memory>
#include <variant>
#include <vector>
#include <unordered_set>
#include <functional>
#include <assert.h>

#include <iostream>

// immutable persistent list data structure for lists of elements
// implemented as non-rebalancing ropes

template<typename T>
class List {

private:
    struct Element;
    struct Node {
        const std::shared_ptr<Element> left;
        const std::shared_ptr<Element> right;
        Node(const std::shared_ptr<Element>& left, const std::shared_ptr<Element>& right) : left(left), right(right) { }
    };
    struct Element {
        std::variant<Node,std::shared_ptr<T>> data;
        Element(const std::shared_ptr<T>& leaf): data(leaf) { }
        Element(const std::shared_ptr<Element>& left, const std::shared_ptr<Element>& right) : data(Node(left, right)) { }

        bool isLeaf() const { return std::holds_alternative<std::shared_ptr<T>>(data); }
        bool isNode() const { return std::holds_alternative<Node>(data); }
        const T& getLeaf() const { return *std::get<std::shared_ptr<T>>(data); }
        const Node& getNode() const { return std::get<Node>(data); }
        std::shared_ptr<T> getLeafPtr() const { return std::get<std::shared_ptr<T>>(data); }
    };
    
    std::shared_ptr<Element> elem;
    // elem = nullptr: empty list

    int elementCount = 0;

public:
    List() : elem(nullptr) { }
    List(const std::shared_ptr<Element>& elem, int elementCount) : elem(elem), elementCount(elementCount) { }
    List(std::initializer_list<T> list) {
        elem = nullptr;
        elementCount = 0;
        for(auto it = std::begin(list); it != std::end(list); it++) {
            if(!elem) elem = std::make_shared<Element>(std::make_shared<T>(*it));
            else elem = std::make_shared<Element>(elem, std::make_shared<Element>(std::make_shared<T>(*it)));
            elementCount++;
        }
    }

    static List<T> append(const List<T>& l1, const List<T>& l2) {
        if(!l1.elem) return l2;
        if(!l2.elem) return l1;
        return List(std::make_shared<Element>(l1.elem, l2.elem), l1.size() + l2.size());
    }

    int size() const { return elementCount; }

    const T& front() const {
        if(!elem) throw std::runtime_error("List<T>::front() called on list with size 0");
        const   Element* e = elem.get();
        while(e->isNode()) e = e->getNode().left.get();
        return e->getLeaf();
    }

    const T& back() const {
        if(!elem) throw std::runtime_error("List<T>::back() called on list with size 0");
        const   Element* e = elem.get();
        while(e->isNode()) e = e->getNode().right.get();
        return e->getLeaf();
    }

    void push_front(const T& value) {
        auto leaf = std::make_shared<Element>(std::make_shared<T>(value));
        if(!elem) elem = leaf;
        else elem = std::make_shared<Element>(leaf, elem);
        elementCount++;
    }
    
    void push_back(const T& value) {
        auto leaf = std::make_shared<Element>(std::make_shared<T>(value));
        if(!elem) elem = leaf;
        else elem = std::make_shared<Element>(elem, leaf);
        elementCount++;
    }

    void append_front(const List<T>& l2) {
        if(!elem) elem = l2.elem;
        if(elem && l2.elem) elem = std::make_shared<Element>(l2.elem, elem);
        elementCount += l2.size();
    }
    void append_back(const List<T>& l2) {
        if(!elem) elem = l2.elem;
        if(elem && l2.elem) elem = std::make_shared<Element>(elem, l2.elem);
        elementCount += l2.size();
    }

    struct const_iterator {
        // empty interator: currentLeaf == nullptr
        std::vector<const Node*> nodeStack;
        const T* currentLeaf;

        const_iterator(Element* e) {
            if(e == nullptr) {
                nodeStack = {};
                currentLeaf = nullptr;
                return;
            }
            Element *p = e;
            while(!p->isLeaf()) {
                nodeStack.push_back(&p->getNode());
                p = nodeStack.back()->left.get();
            }
            currentLeaf = &p->getLeaf();
        }
        const_iterator() : currentLeaf(nullptr) { }
        bool operator==(const_iterator other) { return currentLeaf == other.currentLeaf && nodeStack == other.nodeStack; }
        bool operator!=(const_iterator other) { return !operator==(other); }
        void operator++() {
            if(nodeStack.empty()) {currentLeaf = nullptr; return; }
            Element* p = nodeStack.back()->right.get();
            nodeStack.pop_back();
            while(!p->isLeaf()) {
                nodeStack.push_back(&p->getNode());
                p = nodeStack.back()->left.get();
            }
            currentLeaf = &p->getLeaf();
        }
        const T& operator*() { if(currentLeaf) return *currentLeaf; throw std::runtime_error("List<T>::iterator::operator*: dereferenced invalid iterator"); }
        const T* operator->() { return currentLeaf; }
        using difference_type = int;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::forward_iterator_tag;
    };

    const_iterator begin() const { return const_iterator(elem.get()); }
    const_iterator end() const { return const_iterator(); }

    const T& operator[](int i) const {
        if(i >= size()) throw std::runtime_error("List<T>::operator[] out of range");
        const_iterator it = begin();
        for(int j = 0; j < i; j++) ++it;
        const T& result = *it;
        return result;
    }

};

#endif