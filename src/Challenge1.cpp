#include <iostream>
#include <memory>

class Node {
public:
    int data;
    std::unique_ptr<Node> next;

    Node(int data) {
        this->data = data;
        next = nullptr;
    }
};

class LinkedList {
private:
    std::unique_ptr<Node> head;

public:
    LinkedList() {
        head = nullptr;
    }

    void insert(int data) {
        std::unique_ptr<Node> newNode = std::make_unique<Node>(data);
        if (head == nullptr) {
            head = std::move(newNode);
        } else {
            Node* temp = head.get();
            while (temp->next != nullptr) {
                temp = temp->next.get();
            }
            temp->next = std::move(newNode);
        }
    }

    void display() {
        Node* temp = head.get();
        while (temp != nullptr) {
            std::cout << temp->data << " ";
            temp = temp->next.get();
        }
        std::cout << std::endl;
    }

    void reverse() {
        Node* prev = nullptr;
        Node* current = head.get();
        Node* next = nullptr;

        while (current != nullptr) {
            next = current->next.get();
            current->next.release();
            current->next.reset(prev);
            prev = current;
            current = next;
        }

        head.release();
        head.reset(prev);
    }
};

int main() {
    LinkedList list;
    list.insert(1);
    list.insert(2);
    list.insert(3);
    list.insert(4);
    list.insert(5);

    std::cout << "Original list: ";
    list.display();

    list.reverse();

    std::cout << "Reversed list: ";
    list.display();

    return 0;
}