// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include <deque>
// #include <iostream>
#include <utility>
#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

   public:
    // Each node within the pairing heap
    class Node {
       public:
        // TODO: After you add add one extra pointer (see below), be sure
        // to initialize it here. DONE
        explicit Node(const TYPE& val)
            : elt{val}, child{nullptr}, sibling{nullptr}, previous(nullptr) {}

        // Description: Allows access to the element at that Node's
        // position.  There are two versions, getElt() and a dereference
        // operator, use whichever one seems more natural to you.
        // Runtime: O(1) - this has been provided for you.
        const TYPE& getElt() const { return elt; }
        const TYPE& operator*() const { return elt; }

        // The following line allows you to access any private data
        // members of this Node class from within the PairingPQ class.
        // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
        // function).
        friend PairingPQ;

       private:
        TYPE elt;
        Node* child;
        Node* sibling;
        // TODO: Add one extra pointer (parent or previous) as desired. DONE

        Node* previous;
        bool isFirstChild() {
            return previous == nullptr || previous->child == this;
        }
    };  // Node

    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass{comp}, root{nullptr}, cnt{0} {
        // TODO: Implement this function. DONE
    }  // PairingPQ()

    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template <typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass{comp} {
        // TODO: Implement this function. DONE
        root = nullptr, cnt = 0;
        std::vector<TYPE> vec(start, end);
        for (TYPE& val : vec)
            addNode(val);
    }  // PairingPQ()

    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other)
        : BaseClass{other.compare}, root{copyHelper(other.root)}, cnt{other.cnt} {
        // TODO: Implement this function. DONE
        // NOTE: The structure does not have to be identical to the original,
        //       but it must still be a valid pairing heap.
    }  // PairingPQ()

    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ& operator=(const PairingPQ& rhs) {
        // TODO: Implement this function. DONE
        // HINT: Use the copy-swap method from the "Arrays and Containers"
        // lecture.
        PairingPQ temp(rhs);
        std::swap(root, temp.root);
        std::swap(cnt, temp.cnt);
        return *this;
    }  // operator=()

    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function. DONE
        destructorHelper(root);
    }  // ~PairingPQ()

    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n) DONE
    virtual void updatePriorities() {
        Node* theRoot = root;
        root = nullptr;
        updatePrioritiesHelper(theRoot);
        // traversing(root);
    }  // updatePriorities()

    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely
    //              in the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE& val) {
        addNode(val);
    }  // push()

    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the pairing heap is empty. Though you are welcome to
    // if you are familiar with them, you do not need to use exceptions in
    // this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function. DONE
        cnt--;
        if (cnt == 0) {
            delete root;
            root = nullptr;
            return;
        }
        Node* theRoot = root;
        std::vector<Node*> vec;
        for (Node* node = root->child; node != nullptr; node = node->sibling) {
            vec.push_back(node);
        }
        delete theRoot;
        for (Node* node : vec) {
            node->previous = nullptr;
            node->sibling = nullptr;
        }
        for (size_t i = 1; i < vec.size(); i += 2)
            vec[i >> 1] = meld(vec[i - 1], vec[i]);
        int32_t vecNewSize = int32_t(vec.size() + 1) >> 1;
        if ((vec.size() & 1) == 1)
            vec[vecNewSize - 1] = vec.back();
        root = vec[vecNewSize - 1];
        for (int32_t i = vecNewSize - 1 - 1; i >= 0; i--) {
            root = meld(root, vec[i]);
        }
    }  // pop()

    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE& top() const {
        // TODO: Implement this function DONE
        return root->getElt();
    }  // top()

    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function DONE
        return cnt;
    }  // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function DONE
        return cnt == 0;
    }  // empty()

    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE& new_value) {
        // TODO: Implement this function DONE
        node->elt = new_value;
        if (node == root)
            return;
        if (node->isFirstChild())
            node->previous->child = node->sibling;
        else
            node->previous->sibling = node->sibling;
        if (node->sibling != nullptr) {
            node->sibling->previous = node->previous;
            node->sibling = nullptr;
        }
        node->previous = nullptr;
        root = meld(root, node);
    }  // updateElt()

    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node* addNode(const TYPE& val) {
        // TODO: Implement this function DONE
        Node* newNode = new Node(val);
        root = meld(root, newNode);
        cnt++;
        return newNode;
    }  // addNode()

   private:
    // TODO: Add any additional member variables or member functions you
    // require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap
    // papers). DONE
    Node* root;
    size_t cnt;
    void makeAChildOfB(Node* a, Node* b) {
        if (b->child != nullptr)
            b->child->previous = a;
        a->sibling = b->child;
        a->previous = b;
        b->child = a;
    }
    Node* meld(Node* a, Node* b) {
        if (a == nullptr)
            return b;
        if (b == nullptr)
            return a;
        if (this->compare(a->getElt(), b->getElt())) {
            // std::cout << b->getElt() << " is father of " << a->getElt() << std::endl;
            makeAChildOfB(a, b);
            return b;
        } else {
            // std::cout << a->getElt() << " is father of " << b->getElt() << std::endl;
            makeAChildOfB(b, a);
            return a;
        }
    }
    Node* copyHelper(Node* curNode) {
        if (curNode == nullptr)
            return nullptr;
        Node* newNode = new Node(curNode->getElt());
        newNode->sibling = copyHelper(curNode->sibling);
        newNode->child = copyHelper(curNode->child);
        return newNode;
    }
    void destructorHelper(Node* curNode) {
        if (curNode == nullptr)
            return;
        destructorHelper(curNode->sibling);
        destructorHelper(curNode->child);
        delete curNode;
    }
    void updatePrioritiesHelper(Node* curNode) {
        if (curNode == nullptr)
            return;
        updatePrioritiesHelper(curNode->sibling);
        updatePrioritiesHelper(curNode->child);
        curNode->child = nullptr;
        curNode->sibling = nullptr;
        curNode->previous = nullptr;
        root = meld(root, curNode);
    }

    // void traversing(Node* node) {
    //     if (node == nullptr) {
    //         std::cout << "nullptr" << std::endl;
    //         return;
    //     }
    //     std::cout << "----node " << node->getElt() << std::endl;
    //     std::cout << "----find siblings of " << node->getElt() << std::endl;
    //     traversing(node->sibling);
    //     std::cout << "----find child of " << node->getElt() << std::endl;
    //     traversing(node->child);
    // }
    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};

#endif  // PAIRINGPQ_H
