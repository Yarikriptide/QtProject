// BinarySearchTree.h
#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <QDebug>

template<typename T>
class BinarySearchTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;

        Node(const T& item) : data(item), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Вспомогательные приватные методы
    void destroyTree(Node* node);
    Node* insertRec(Node* node, const T& item);
    Node* searchRec(Node* node, const T& item) const;
    void inorderRec(Node* node) const;

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() {
        destroyTree(root);
    }

    // Основные операции
    void insert(const T& item) {
        root = insertRec(root, item);
    }

    bool search(const T& item) const {
        return searchRec(root, item) != nullptr;
    }

    void inorder() const {
        inorderRec(root);
        qDebug() << "";
    }
};

// Реализация методов
template<typename T>
void BinarySearchTree<T>::destroyTree(Node* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::insertRec(Node* node, const T& item) {
    if (!node) {
        return new Node(item);
    }

    if (item < node->data) {
        node->left = insertRec(node->left, item);
    }
    else if (item > node->data) {
        node->right = insertRec(node->right, item);
    }

    return node;
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::searchRec(Node* node, const T& item) const {
    if (!node || node->data == item) {
        return node;
    }

    if (item < node->data) {
        return searchRec(node->left, item);
    }

    return searchRec(node->right, item);
}

template<typename T>
void BinarySearchTree<T>::inorderRec(Node* node) const {
    if (node) {
        inorderRec(node->left);
        qDebug() << node->data.getId() << node->data.getName();
        inorderRec(node->right);
    }
}

#endif // BINARYSEARCHTREE_H
