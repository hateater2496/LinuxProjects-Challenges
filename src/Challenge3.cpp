#include <iostream>
#include <queue>
#include <memory>

class BinarySearchTree {
private:
    struct Node {
        int data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(int value) : data(value), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(int value) {
        root = insertRecursive(std::move(root), value);
    }

    bool containsRecursive(int value) {
        return containsRecursiveHelper(root.get(), value);
    }

    bool containsIterative(int value) {
        if (root == nullptr) {
            return false;
        }

        std::queue<Node*> queue;
        queue.push(root.get());

        while (!queue.empty()) {
            Node* current = queue.front();
            queue.pop();

            if (current->data == value) {
                return true;
            }

            if (current->left != nullptr) {
                queue.push(current->left.get());
            }

            if (current->right != nullptr) {
                queue.push(current->right.get());
            }
        }

        return false;
    }

private:
    std::unique_ptr<Node> insertRecursive(std::unique_ptr<Node> node, int value) {
        if (node == nullptr) {
            return std::make_unique<Node>(value);
        }

        if (value < node->data) {
            node->left = insertRecursive(std::move(node->left), value);
        } else if (value > node->data) {
            node->right = insertRecursive(std::move(node->right), value);
        }

        return node;
    }

    bool containsRecursiveHelper(Node* node, int value) {
        if (node == nullptr) {
            return false;
        }

        if (node->data == value) {
            return true;
        } else if (value < node->data) {
            return containsRecursiveHelper(node->left.get(), value);
        } else {
            return containsRecursiveHelper(node->right.get(), value);
        }
    }
};

int main() {
    BinarySearchTree bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(2);
    bst.insert(4);
    bst.insert(6);
    bst.insert(8);

    std::cout << "Recursive search: " << bst.containsRecursive(4) << std::endl;
    std::cout << "Iterative search: " << bst.containsIterative(4) << std::endl;

    return 0;
}