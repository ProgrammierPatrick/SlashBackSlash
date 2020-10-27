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

    static List<T> append_unique(const List<T>& list1, const List<T>& list2) {
        if(list1.size() == 0) return list2;
        if(list2.size() == 0) return list1;

        // DISBALE THIS FUNCTION, this function is bad for speed
        return append(list1, list2);

        const int maxDepth = 16;
        int currentDepth = 0;

        // store visited nodes in set for O(1) contains query
        std::unordered_set<const Node*> l1Set;
        std::unordered_set<const Node*> l2Set;

        // stacks store nodes of which the right tree has not been searched 
        std::vector<const Node*> l1Stack;
        std::vector<const Node*> l2Stack;

        // if shared subtree is found, this is its first shared node
        const Node* firstSharedNode = nullptr;
        
        // advance to the first leaf. Fill stack with all nodes on the leftmost path
        for(Element *p = list1.elem.get(); p->isNode(); p = p->getNode().left.get()) {
            const Node* n = &p->getNode();
            l1Stack.push_back(n);
            l1Set.insert(n);
            currentDepth++;
            if(currentDepth >= maxDepth) return append(list1, list2);
        }
        for(Element *p = list2.elem.get(); p->isNode(); p = p->getNode().left.get()) {
            const Node* n = &p->getNode();
            if(l1Set.find(n) != l1Set.end()) { firstSharedNode = n; break; }
            l2Stack.push_back(n);
            l2Set.insert(n);
            currentDepth++;
            if(currentDepth >= maxDepth) return append(list1, list2);
        }

        // iterate tree structures in order to find shared node
        while(!firstSharedNode && !(l1Stack.empty() && l2Stack.empty())) {
            if(!l1Stack.empty()) {
                Element* p = l1Stack.back()->right.get();
                l1Stack.pop_back();
                for(; p->isNode(); p = p->getNode().left.get()) {
                    const Node* n = &p->getNode();
                    if(l2Set.find(n) != l2Set.end()) { firstSharedNode = n; break; }
                    l1Stack.push_back(n);
                    l1Set.insert(n);
                    currentDepth++;
                    if(currentDepth >= maxDepth) return append(list1, list2);
                }
                if(firstSharedNode) break;
            }
            if(!l2Stack.empty()) {
                Element* p = l2Stack.back()->right.get();
                l2Stack.pop_back();
                for(; p->isNode(); p = p->getNode().left.get()) {
                    const Node* n = &p->getNode();
                    if(l1Set.find(n) != l1Set.end()) { firstSharedNode = n; break; }
                    l2Stack.push_back(n);
                    l2Set.insert(n);
                    currentDepth++;
                    if(currentDepth >= maxDepth) return append(list1, list2);
                }
            }
        }

        if(firstSharedNode) {
            // shared subtree found
            // result: list1 + (list2 without shared part)

            // search for firstSharedNode in list2

            // stack of Nodes which still need to check right part
            std::vector<const Node*> stack;
            std::vector<bool> path;
            bool found = false;

            // advance to first leaf
            for(Element *p = list2.elem.get(); p->isNode(); p = p->getNode().left.get()) {
                const Node* n = &p->getNode();
                if(n == firstSharedNode) { found = true; break; }
                stack.push_back(n);
                path.push_back(false);
                currentDepth++;
                if(currentDepth >= maxDepth) return append(list1, list2);
            }

            while(!found) {
                assert(!stack.empty());
                assert(stack.size() == path.size());

                // backtrack to first node with needs its right side checked
                while(path.back() == true) {
                    path.pop_back();
                    stack.pop_back();
                    assert(!stack.empty());
                    assert(stack.size() == path.size());
                    currentDepth++;
                    if(currentDepth >= maxDepth) return append(list1, list2);
                }

                // get right part and change path to "right"
                Element* p = stack.back()->right.get();
                path.pop_back();
                path.push_back(true);

                // advance to next leaf
                for(; p->isNode(); p = p->getNode().left.get()) {
                    const Node* n = &p->getNode();
                    if(n == firstSharedNode) { found = true; break; }
                    stack.push_back(n);
                    path.push_back(false);
                    currentDepth++;
                    if(currentDepth >= maxDepth) return append(list1, list2);
                }
            }

            // firstSharedNode found, stack now contains all nodes above firstSharedNode
            // create new list without the firstSharedNode tree by traversing the stack in reverse order
            
            std::shared_ptr<Element> newList2;

            if(stack.empty()) {
                // found element is at root of list2, just return list1
                return list1;
            }

            if (     stack.back()->left->isNode() && &stack.back()->left->getNode() == firstSharedNode)
                newList2 = stack.back()->right;
            else if (stack.back()->right->isNode() && &stack.back()->right->getNode() == firstSharedNode)
                newList2 = stack.back()->left;
            assert(newList2 != nullptr);

            for(int i = path.size() - 2; i >= 0; i--) {
                if(path[i] == false) { // left
                    newList2 = std::make_shared<Element>(newList2, stack[i]->right);
                } else { // right
                    newList2 = std::make_shared<Element>(stack[i]->left, newList2);
                }
                currentDepth++;
                if(currentDepth >= maxDepth) return append(list1, list2);
            }

            // count size of new list
            int list2Size = 0;
            for(const_iterator it(newList2.get()); it != const_iterator(); ++it)
                list2Size++;

            return append(list1, List<T>(newList2, list2Size));
        }
        else return append(list1, list2);
    }

};

#endif