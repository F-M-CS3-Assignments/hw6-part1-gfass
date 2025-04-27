#include "RedBlackTree.h"
#include <stdexcept>
#include <sstream>
#include <cassert>

using namespace std;

// Constructor: Initialize an empty Red-Black Tree
RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}

// Constructor: Create a Red-Black Tree with a single black root node
RedBlackTree::RedBlackTree(int newData) {
    RBTNode* node = new RBTNode;
    node->data = newData;
    node->color = COLOR_BLACK;
    root = node;
    numItems = 1;
}

// Copy Constructor: Create a deep copy of an existing Red-Black Tree
RedBlackTree::RedBlackTree(const RedBlackTree &rbt) {
    root = CopyOf(rbt.root);
    numItems = rbt.numItems;
}

// Insert a new node into the Red-Black Tree
void RedBlackTree::Insert(int newData) {
    if (Contains(newData)) {
        throw invalid_argument("Duplicate value not allowed in RedBlackTree");
    }

    RBTNode* node = new RBTNode;
    node->data = newData;
    node->color = COLOR_RED;

    if (root == nullptr) {
        node->color = COLOR_BLACK;
        root = node;
        numItems++;
        return;
    }

    BasicInsert(node);

    // Fix Red-Black properties if violated
    if (node->parent != nullptr && node->parent->color == COLOR_RED) {
        InsertFixUp(node);
    }

    numItems++;
    root->color = COLOR_BLACK;
}

// Basic binary search tree insert (no balancing)
void RedBlackTree::BasicInsert(RBTNode* node) {
    RBTNode* curr = root;
    RBTNode* parent = nullptr;

    while (curr != nullptr) {
        parent = curr;
        if (node->data < curr->data) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    node->parent = parent;
    if (node->data < parent->data) {
        parent->left = node;
    } else {
        parent->right = node;
    }
}

// Fix violations of Red-Black Tree properties after insertion
void RedBlackTree::InsertFixUp(RBTNode* node) {
    RBTNode* parent = node->parent;
    RBTNode* uncle = GetUncle(node);
    RBTNode* grand_parent = parent->parent;

    if (uncle != nullptr && uncle->color == COLOR_RED) {
        // Case 1: Uncle is red -> recolor
        parent->color = COLOR_BLACK;
        uncle->color = COLOR_BLACK;
        if (grand_parent != nullptr) {
            grand_parent->color = COLOR_RED;
            if (grand_parent->parent != nullptr && grand_parent->parent->color == COLOR_RED) {
                InsertFixUp(grand_parent);
            }
        }
    } else if (grand_parent != nullptr) {
        // Uncle is black or null -> rotations needed
        grand_parent->color = COLOR_RED;

        if (IsLeftChild(node) && IsLeftChild(parent)) {
            // Left-Left Case
            RightRotate(grand_parent);
            parent->color = COLOR_BLACK;
        } else if (IsRightChild(node) && IsRightChild(parent)) {
            // Right-Right Case
            LeftRotate(grand_parent);
            parent->color = COLOR_BLACK;
        } else if (IsLeftChild(node) && IsRightChild(parent)) {
            // Left-Right Case
            RightRotate(parent);
            LeftRotate(grand_parent);
            node->color = COLOR_BLACK;
            parent->color = COLOR_RED;
        } else if (IsRightChild(node) && IsLeftChild(parent)) {
            // Right-Left Case
            LeftRotate(parent);
            RightRotate(grand_parent);
            node->color = COLOR_BLACK;
            parent->color = COLOR_RED;
        } else {
            throw invalid_argument("impossible state!");
        }
    }
}

// Check if a given value exists in the tree
bool RedBlackTree::Contains(int data) const {
    RBTNode* curr = root;
    while (curr != nullptr) {
        if (data == curr->data) return true;
        curr = (data < curr->data) ? curr->left : curr->right;
    }
    return false;
}

// Get minimum value in the tree (leftmost node)
int RedBlackTree::GetMin() const {
    if (root == nullptr) throw invalid_argument("Tree is empty");
    RBTNode* curr = root;
    while (curr->left != nullptr) curr = curr->left;
    return curr->data;
}

// Get maximum value in the tree (rightmost node)
int RedBlackTree::GetMax() const {
    if (root == nullptr) throw invalid_argument("Tree is empty");
    RBTNode* curr = root;
    while (curr->right != nullptr) curr = curr->right;
    return curr->data;
}

// Helper to find a node with given value
RBTNode* RedBlackTree::Get(int data) const {
    RBTNode* curr = root;
    while (curr != nullptr) {
        if (curr->data == data) return curr;
        curr = (data < curr->data) ? curr->left : curr->right;
    }
    return nullptr;
}

// Infix (in-order) traversal to string
string RedBlackTree::ToInfixString(const RBTNode* n) {
    if (n == nullptr) return "";
    string result = "";
    result += ToInfixString(n->left);
    result += " " + GetNodeString(n) + " ";
    result += ToInfixString(n->right);
    return result;
}

// Prefix (pre-order) traversal to string
string RedBlackTree::ToPrefixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return " " + GetNodeString(n) + " " + ToPrefixString(n->left) + ToPrefixString(n->right);
}

// Postfix (post-order) traversal to string
string RedBlackTree::ToPostfixString(const RBTNode* n) {
    if (n == nullptr) return "";
    string result = "";
    result += ToPostfixString(n->left);
    result += ToPostfixString(n->right);
    result += " " + GetNodeString(n) + " ";
    return result;
}

// Helper to return node's color as a string
string RedBlackTree::GetColorString(const RBTNode* n) {
    return n->color == COLOR_RED ? "R" : "B";
}

// Helper to return node's color and data as string
string RedBlackTree::GetNodeString(const RBTNode* n) {
    return GetColorString(n) + to_string(n->data);
}

// Check if a node is a left child of its parent
bool RedBlackTree::IsLeftChild(RBTNode* node) const {
    return node->parent != nullptr && node->parent->left == node;
}

// Check if a node is a right child of its parent
bool RedBlackTree::IsRightChild(RBTNode* node) const {
    return node->parent != nullptr && node->parent->right == node;
}

// Get the uncle node of a given node
RBTNode* RedBlackTree::GetUncle(RBTNode* node) const {
    RBTNode* parent = node->parent;
    RBTNode* grandparent = parent ? parent->parent : nullptr;
    if (!grandparent) return nullptr;
    return (grandparent->left == parent) ? grandparent->right : grandparent->left;
}

// Perform a left rotation around node x
void RedBlackTree::LeftRotate(RBTNode* x) {
    RBTNode* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Perform a right rotation around node x
void RedBlackTree::RightRotate(RBTNode* x) {
    RBTNode* y = x->left;
    x->left = y->right;
    if (y->right != nullptr) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// Deep copy a subtree rooted at node
RBTNode* RedBlackTree::CopyOf(const RBTNode* node) {
    if (!node) return nullptr;
    RBTNode* newNode = new RBTNode;
    newNode->data = node->data;
    newNode->color = node->color;
    newNode->IsNullNode = node->IsNullNode;
    newNode->left = CopyOf(node->left);
    newNode->right = CopyOf(node->right);
    if (newNode->left) newNode->left->parent = newNode;
    if (newNode->right) newNode->right->parent = newNode;
    return newNode;
}

// Tests for private helper methods
void RedBlackTree::PrivateTests() {
    cout << "Running PrivateTests()..." << endl;

    // Create a simple manual tree for testing
    RBTNode* node1 = new RBTNode();
    node1->data = 20;
    node1->color = COLOR_BLACK;

    RBTNode* node2 = new RBTNode();
    node2->data = 10;
    node2->color = COLOR_RED;
    node2->parent = node1;
    node1->left = node2;

    RBTNode* node3 = new RBTNode();
    node3->data = 30;
    node3->color = COLOR_RED;
    node3->parent = node1;
    node1->right = node3;

    root = node1;
    numItems = 3;

    // Test child relationships
    assert(IsLeftChild(node2));
    assert(!IsRightChild(node2));
    assert(IsRightChild(node3));
    assert(!IsLeftChild(node3));

    // Test GetUncle
    RBTNode* node4 = new RBTNode();
    node4->data = 5;
    node4->color = COLOR_RED;
    node4->parent = node2;
    node2->left = node4;
    assert(GetUncle(node4) == node3);

    // Test LeftRotate and RightRotate
    LeftRotate(node1);
    assert(root == node3);
    assert(root->left == node1);
    assert(root->left->data == 20);

    RightRotate(root);
    assert(root == node1);
    assert(root->right == node3);

    // Test BasicInsert and Get
    RBTNode* newNode = new RBTNode();
    newNode->data = 25;
    newNode->color = COLOR_RED;
    BasicInsert(newNode);

    assert(Get(25) != nullptr);
    assert(Get(25)->data == 25);
    assert(Get(100) == nullptr);

    // Test CopyOf
    RBTNode* rootCopy = CopyOf(root);
    assert(rootCopy != nullptr);
    assert(rootCopy->data == root->data);
    assert(rootCopy->left->data == root->left->data);
    assert(rootCopy->right->data == root->right->data);

    // Manual memory cleanup
    delete node4;
    delete newNode;
    delete rootCopy->left;
    delete rootCopy->right;
    delete rootCopy;
    delete node3;
    delete node2;
    delete node1;

    root = nullptr;
    numItems = 0;

    cout << "PrivateTests() PASSED!" << endl << endl;
}

/**
 * 
 * Sources:
 *
 * Zybook chapters & sections: 15.1 - 15.10 and 16.1 - 16.2
 * For insertion method: video lecture 9
 * 
 * Websites used to help understand main concepts:
 * https://www.geeksforgeeks.org/introduction-to-red-black-tree/
 * https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
 * http://btechsmartclass.com/data_structures/red-black-trees.html
 * https://www.tutorialspoint.com/insertion-in-the-red-black-tree-in-data-structure
 * 
**/
