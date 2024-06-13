#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node{
   Node* prev;
   Node* next;
   int key;
   int value;
   Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
   Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{
   
   protected: 
   map<int,Node*> mp; //map the key to the node in the linked list
   int cp;  //capacity
   Node* tail; // double linked list tail pointer
   Node* head; // double linked list head pointer
   virtual void set(int, int) = 0; //set function
   virtual int get(int) = 0; //get function

};

class LRUCache: public Cache
{
public:
    LRUCache(int capacity)
    {
        cp = capacity;
        head = nullptr;
        tail = nullptr;
    }

    void set(int key, int value)
    {
        if (mp.find(key) != mp.end())
        {
            mp[key]->value = value;
        }
        else
        {
            if (static_cast<int>(mp.size()) >= cp)
            {
                mp.erase(tail->key);
                Node* temp = tail->prev;
                delete tail;
                tail = temp;
                if (tail)
                {
                    tail->next = nullptr;
                }
            }

            Node* newNode = new Node(nullptr, head, key, value);
            if (head)
            {
                head->prev = newNode;
            }
            head = newNode;
            if (!tail)
            {
                tail = head;
            }
            mp[key] = newNode;
        }
        printList();
    }

    int get(int key)
    {
        int val = -1;
        if (mp.find(key) != mp.end())
        {
            Node* node = mp[key];
            if (node != head)
            {
                if (node == tail)
                {
                    tail = node->prev;
                }
                node->prev->next = node->next;
                if (node->next)
                {
                    node->next->prev = node->prev;
                }
                node->next = head;
                node->prev = nullptr;
                head->prev = node;
                head = node;
            }
            val = node->value;
        }
        printList();
        return val;
    }

    ~LRUCache()
    {
        while (head)
        {
            Node* temp = head->next;
            delete head;
            head = temp;
        }
    }

    void printList()
    {
        Node* temp = head;
        while (temp)
        {
            cout << temp->value << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

int main() {
   int n, capacity,i;
   cin >> n >> capacity;
   LRUCache l(capacity);
   for(i=0;i<n;i++) {
      string command;
      cin >> command;
      if(command == "get") {
         int key;
         cin >> key;
         cout << l.get(key) << endl;
      } 
      else if(command == "set") {
         int key, value;
         cin >> key >> value;
         l.set(key,value);
      }
   }
   return 0;
}
