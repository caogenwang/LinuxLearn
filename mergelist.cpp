#include <iostream>
using namespace std;
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
	ListNode*head = new ListNode();
  ListNode*h2 = head;
  
  ListNode*t1 = l1;
  ListNode*t2 = l2;
  while(t1 != nullptr && t2 != nullptr){
    if(t1->val < t2->val){
      head->next = t1;
      t1 = t1->next;
    }
    else if(t1->val > t2->val){
      head->next = t2;
      t2 = t2->next;
    }
    else{
      head->next = t1;
      t1 = t1->next;
      head = head->next;
      head->next = t2;
      t2 = t2->next;
    }
    head = head->next;
  }
 	if(t1 != nullptr)
    head->next = t1;
  if(t2 != nullptr)
    head->next = t2;
  return h2->next;
}

int main(){

    ListNode n11(0);
    ListNode n12(2); 
    ListNode n13(4); 
    n11.next = &n12;
    n12.next = &n13;

    ListNode n21(1);
    ListNode n22(2); 
    ListNode n23(3);
    ListNode n24(5);
    ListNode n25(7);
    n21.next = &n22;
    n22.next = &n23;
    n23.next = &n24;
    n24.next = &n25;
    ListNode *g =  mergeTwoLists(&n11,&n21);
    while (g)
    {
        cout<<g->val<<endl;
        g=g->next;
    }
    
    return 0;
}