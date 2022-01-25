#include<bits/stdc++.h>


using namespace std;

struct ListNode {
    int val;
    ListNode* next;

};
ListNode* detectcycle(ListNode* head) {
    ListNode* slow = head, * fast = head;

    do {
        if (!fast || !fast->next)
            return nullptr;
        fast = fast->next->next;
        slow = slow->next;
    } while (fast != slow);
    fast = head;
    while (fast != slow) {
        slow = slow->next;
        fast = fast->next;
    }
    return fast;

}

int main(int argc, char* argv[]) {

    return 0;
}