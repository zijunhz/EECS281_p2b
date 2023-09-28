// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef BINARYPQ_H
#define BINARYPQ_H

#include <algorithm>
#include <utility>
#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a binary heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

   public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass{comp} {
        // TODO: Implement this function, or verify that it is already done
        data.push_back(TYPE());
    }  // BinaryPQ

    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template <typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass{comp}, data(start, end) {
        // TODO: Implement this function
        data.push_back(TYPE());
        std::swap(data[0], data.back());
        updatePriorities();
    }  // BinaryPQ

    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ() {
    }  // ~BinaryPQ()

    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
        for (size_t i = size(); i >= 1; --i)
            fixDown(i);
    }  // updatePriorities()

    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE& val) {
        // TODO: Implement this function.
        data.push_back(val);
        fixUp(size());
    }  // push()

    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the PQ is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        TYPE back = data.back();
        data.pop_back();
        if (!empty()) {
            data[1] = back;
            fixDown(1);
        }
    }  // pop()

    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE& top() const {
        // TODO: Implement this function.
        return data[1];
    }  // top()

    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function. Might be very simple,
        // depending on your implementation.
        return data.size() - 1;
    }  // size()

    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function. Might be very simple,
        // depending on your implementation.
        return size() == 0;
    }  // empty()

   private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables. You don't need
    //       a "heapSize", since you can call your own size() member
    //       function, or check data.size().

    // TODO: Add any additional member functions you require here.
    //       For instance, you might add fixUp() and fixDown().
    void fixUp(size_t index) {
        while (index != 1 && this->compare(data[index >> 1], data[index])) {
            std::swap(data[index], data[index >> 1]);
            index = index >> 1;
        }
    }
    void fixDown(size_t index) {
        size_t cur = index;
        while (1) {
            size_t target = cur << 1;
            if (target > size()) {
                target = 0;
                break;
            }
            if (target + 1 <= size() && this->compare(data[target], data[target + 1]))
                target++;
            if (this->compare(data[cur], data[target])) {
                std::swap(data[cur], data[target]);
                cur = target;
            } else
                break;
        }
    }
};  // BinaryPQ

#endif  // BINARYPQ_H
