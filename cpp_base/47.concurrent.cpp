// atomic::compare_exchange_weak example:
#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread
#include <vector>         // std::vector
#include <pthread.h>
// a simple global linked list:
struct Node 
{ 
    int value; 
    Node* next; 
};
std::atomic<Node*> list_head (nullptr);
std::atomic_llong total{0};
std::mutex mtx;  
void append (int val) {     // append an element to the list
  Node* oldHead = list_head;
  Node* newNode = new Node {val,oldHead};

  // what follows is equivalent to: list_head = newNode, but in a thread-safe way:
  while (!list_head.compare_exchange_weak(oldHead,newNode))
    newNode->next = oldHead;
}

void append_mutex(int val){
   
    mtx.lock();
    Node* oldHead = list_head;
    Node* newNode = new Node {val,oldHead};
    list_head = newNode;
    mtx.unlock();
}

int main ()
{
  // spawn 10 threads to fill the linked list:
  std::vector<std::thread> threads;
  for (int i=0; i<50000; ++i) 
    threads.push_back(std::thread(append_mutex,i));
  for (auto& th : threads) 
    th.join();

  // print contents:
//   for (Node* it = list_head; it!=nullptr; it=it->next)
//     std::cout << ' ' << it->value;
//   std::cout << '\n';

  // cleanup:
//   Node* it; 
//   while (it=list_head) 
//   {
//       list_head=it->next; 
//       delete it;
//   }
  return 0;
}
