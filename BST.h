/*************************************************
 * Version: 12/16/2022
 * BST.h provides all of the major methods of the implementation of the binary search tree
 * previously implemented function include insert, remove, contains, isempty and clear
 * newly implemented functions are to find the maximum and minimum elements of the tree,
 * the height of tree, and pre,post, and in order traversals of the trees
 * **************************************************/

#ifndef HW3_BST_H
#define HW3_BST_H

#include <algorithm>
#include <iostream>
#include <algorithm>

#include "Exceptions.h"

using namespace std;

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void _insert( x )       --> Insert x
// void _remove( x )       --> Remove x
// bool _contains( x )     --> Return true if x is present
// Comparable FindMin( )  --> Return smallest item
// Comparable FindMax( )  --> Return largest item
// boolean IsEmpty( )     --> Return true if empty; else false
// void Clear( )      --> Remove all items
// void _printTreeInorder( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted


template <typename Comparable>
class BinarySearchTree
{
public:
    BinarySearchTree( ) : _root{nullptr }
    {
    }

    /**
     * Copy constructor
     */
    BinarySearchTree( const BinarySearchTree & rhs ) : _root{nullptr }
    {
        _root = _clone(rhs._root);
    }

    /**
     * Move constructor
     */
    BinarySearchTree( BinarySearchTree && rhs )  noexcept : _root{rhs._root }
    {
        rhs._root = nullptr;
    }

    /**
     * Destructor for the tree
     */
    ~BinarySearchTree( )
    {
        Clear();
    }

    /**
     * Copy assignment
     */
    BinarySearchTree & operator=( const BinarySearchTree & rhs )
    {
        BinarySearchTree copy = rhs;
        std::swap( *this, copy );
        return *this;
    }

    /**
     * Move assignment
     */
    BinarySearchTree & operator=( BinarySearchTree && rhs )
    {
        std::swap(_root, rhs._root );
        return *this;
    }


    /**
     * Find the smallest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & FindMin( ) const
    {
        if(IsEmpty() )
            throw UnderflowException{ };
        return _findMin(_root)->element;
    }

    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & FindMax( ) const
    {
        if(IsEmpty() )
            throw UnderflowException{ };

        return _findMax(_root)->element;
    }

    int Height() {
        return _height(_root);
    }

    int Depth(const Comparable& element){
        cout<<"Depth of node "<<element<<" = ";
        return _depth(_root, element);
    }

    /**
     * Returns true if x is found in the tree.
     */
    bool Contains(const Comparable & x ) const
    {
        return _contains(x, _root);
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool IsEmpty( ) const
    {
        return _root == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void PrintTree(ostream & out = cout) const
    {
        if(IsEmpty() )
            out << "Empty tree" << endl;

        else {
            cout<<"Root: "<<_root->element<<endl;
            cout<<"In-Order: ";
            _printTreeInorder(_root, out);
            cout<<endl;
            cout<<"Pre-Order: ";
            _printTreePreOrder(_root, out);
            cout<<endl;
            cout<<"Post-Order: ";
            _printTreePostOrder(_root, out);
            cout<<endl;
        }

        out<<endl;
    }

    /**
     * Make the tree logically empty.
     */
    void Clear( )
    {
        _makeEmpty(_root);
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void Insert(const Comparable & x )
    {
        _insert(x, _root);
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void Insert(Comparable && x )
    {
        _insert(std::move(x), _root);
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void Remove(const Comparable & x )
    {
        _remove(x, _root);
    }

    Comparable get(const Comparable& x) const {
        return get(x,root);
    }

private:
    struct BinaryNode
    {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt )
                : element{ theElement }, left{ lt }, right{ rt } { }

        BinaryNode( Comparable && theElement, BinaryNode *lt, BinaryNode *rt )
                : element{ std::move( theElement ) }, left{ lt }, right{ rt } { }
    };

    BinaryNode *_root;


    /**
     * Internal method to _insert into a subtree.
     * x is the item to _insert.
     * t is the node that roots the subtree.
     * Set the new _root of the subtree.
     */
    void _insert(const Comparable & x, BinaryNode * & t )
    {
        if( t == nullptr )
            t = new BinaryNode{ x, nullptr, nullptr };
        else if( x < t->element )
            _insert(x, t->left);
        else if( t->element < x )
            _insert(x, t->right);
        else
            ;  // Duplicate; do nothing
    }

    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the subtree.
     */
    bool _contains(const Comparable & x, BinaryNode *t ) const
    {
        if( t == nullptr )
            return false;
        else if( x < t->element )
            return _contains(x, t->left);
        else if( t->element < x )
            return _contains(x, t->right);
        else
            return true;    // Match
    }
    /**
     * Internal method to make subtree empty.
     */
    void _makeEmpty(BinaryNode* & t )
    {
        if( t != nullptr )
        {
            _makeEmpty(t->left);
            _makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }

    /**
     * Internal method to _insert into a subtree.
     * x is the item to _insert.
     * t is the node that roots the subtree.
     * Set the new _root of the subtree.
     */
    void _insert(Comparable && x, BinaryNode * & t )
    {
        if( t == nullptr )
            t = new BinaryNode{ std::move( x ), nullptr, nullptr };
        else if( x < t->element )
            _insert(std::move(x), t->left);
        else if( t->element < x )
            _insert(std::move(x), t->right);
        else
            ;  // Duplicate; do nothing
    }

    /**
     * Internal method to _remove from a subtree.
     * x is the item to _remove.
     * t is the node that roots the subtree.
     * Set the new _root of the subtree.
     */
    void _remove(const Comparable & x, BinaryNode * & t )
    {
        if( t == nullptr )
            return;   // Item not found; do nothing
        if( x < t->element )
            _remove(x, t->left);
        else if( t->element < x )
            _remove(x, t->right);
        else if( t->left != nullptr && t->right != nullptr ) // Two children
        {
            t->element = _findMin(t->right)->element;
            _remove(t->element, t->right);
        }
        else
        {
            BinaryNode *oldNode = t;
            t = ( t->left != nullptr ) ? t->left : t->right;
            delete oldNode;
        }
    }

    Comparable get(const Comparable & x,BinaryNode* t) const
    {
        auto t = this ->root;

        if( t == nullptr )
            return Comparable("");
        else if( x < t->element )
            return get(x, t->left);
        else if( t->element < x )
            return get(x, t->right);
        else
            return t->element;    // Match
    }

    /**
     * Internal method to _clone subtree.
     */
    BinaryNode * _clone(BinaryNode *t ) const
    {
        if( t == nullptr )
            return nullptr;
        else
            return new BinaryNode{t->element, _clone(t->left), _clone(t->right) };
    }


    /**
    Add the following methods to the BinarySearchTree class.
	Add an implementation of _findMax and _findMin private methods
    to find the maximum and minimum items of the tree respectively.
	Implement pre-order, post-order and in-order traversal private methods.
    Call each method in the public interface called PrintTree.
	Write a method that computes the height of the tree.
	Write a method the computes the depth of a given node.
	Inside main, construct the following tree and demonstrate each method.
    **/

    /**
     * Internal method to GetHashValue the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    BinaryNode * _findMin(BinaryNode *t ) const
    {
        // recurisve to find the min going through the left and right
        // subtrees
        if ( t == nullptr){
            return nullptr;
        }
        if (t->left == nullptr) {
            return t;
        }
        return _findMin(t->left);
    }

    /**
     * Internal method to GetHashValue the largest item in a subtree t.
     * Return node containing the largest item.
     */
    BinaryNode * _findMax(BinaryNode *t ) const
    {
        // since the max will always be on the right subtree
        // the right subtree is traversed to find the maximum
        // element
        if ( t != nullptr ) {
            while ( t->right != nullptr ) {
                t = t -> right;
            }
        }
        return t;
    }

    /**
     * Internal method to print a subtree rooted at t in sorted order.
     */
     // root is t
    void _printTreeInorder(BinaryNode *t, ostream & out ) const
    {
        if (t != nullptr) {
            // look through the left tree
            _printTreeInorder(t->left,out);
            // space out
            out << t->element <<  " ";
            // look through thr right tree
            _printTreeInorder(t->right,out);
        }
    }

    void _printTreePostOrder(BinaryNode *t, ostream& out) const{
        if (t != nullptr) {
            // look through the left tree
            _printTreePostOrder(t->left, out);
            // look through the right tree
            _printTreePostOrder(t->right,out);
            // space out the outputs
            out << t->element << " ";
        }
    }

    void _printTreePreOrder(BinaryNode *t, ostream& out) const {
        if (t != nullptr) {
            // space out to being the output
            out << t->element << " ";
            // left tree traversal
            _printTreePreOrder(t->left, out);
            // right tree traversal
            _printTreePreOrder(t->right,out);
        }
    }

    // function to determine the height of the binary tree
    int _height(BinaryNode* t) {
        // initialize the initial variables
        int h = 0;
        // loop to make sure null pointers aren't included
        if (t!= nullptr) {
            // find the heights of the right and left subtrees
            // and then find the maximum plus 1 due to indexing at 0
            auto left = _height(t->left);
            auto right = _height(t->right);

            h = max(left,right) + 1;
        }
        return h;
    }
};
#endif //HW3_BST_H
