#include <iostream>
using namespace std;
struct node {
    float time, fuel;
    node *next, *prev;
    int index;
};

class dList {
public:
    dList();
    dList (float times[], float fuel[], int size);
    void out (char c);
    void out (int i, char c);
    int insert(float time, float fuel);
    //Project 3
    ~dList() {
        delete [] listNodes;
        node *temp = new node;
        node *next = new node;
        for (temp = head; temp != nullptr; temp = temp->next) {
            next = temp->next;
            delete temp;
        }
        delete next;
    } // destructor
    
    dList& operator=(const dList& src) { // assignment operator
        head = src.head; tail = src.tail;
        listNodes = new node*[src.size];
        size = src.size; counter = src.counter;
        for (int i = 0; i < size; i++) listNodes[i] = src.listNodes[i];
        return *this;
    }
    
    dList(const dList & src) { // copy constructor
        *this = src; }

    void increase_time(int i, float t); //increases the time of the item at index i by adding t to its time. This may cause the item to be dominated.
    void decrease_time(int i, float t); //decreases the time of the item at index i by subtracting t from its time. This may cause it to dominate some entries in the list.
    void decrease_fuel(int i, float f); //decreases the fuel usage of the item at index i by subtracting f from its fuel usage. This may cause it to dominate some netries in the list.
    int index_before(int i); //returns the index of the run that is immediately before the run at index i in the list.
    int index_after(int i); //returns the index of the run that is immediately after the run at index i in the list. In other words, the index of run that uses less fuel, but the closest to run i in the list.
private:
    void mergeSort(float times[], float fuel[], int i, int k, int index[]);
    void merge(float times[], float fuel[], int i, int j, int k, int index[]);
    node *createNode(float time, float fuel, node *next, int index);
    node *createNode(float time, float fuel, int index);
    void removeDominatedNodes();
    bool isNodeDominated(node* b);
    void addAtFront(node* newNode);
    void addAtRear(node* newNode);
    void addInMiddle(node* front, node* back);
    void removeNode(node* del);
    void removeDominatedNodes(node* ptr);
    void removeDominatedNodesBackwards(node* ptr);
    bool isNodeDominated(node* a, node* b);
    

    node *head, *tail;
    node **listNodes;
    int size, counter;
};

dList::dList() {
    size = counter = 0;
    head = nullptr;
    tail = nullptr;
    listNodes = new node*[2];
}

dList::dList(float times[], float fuel[], int s) {
    size = s;
    listNodes = new node*[s];
    counter = size - 1;
    node* temp = new node;
    int unsortedIndex[s];
    for (int i = 0; i < s; i++) unsortedIndex[i] = i;
    int *sortedIndex = new int[size];
    float *sortedTimes = new float[size];
    float *sortedFuel = new float[size];
    sortedTimes = times;
    sortedFuel = fuel;
    sortedIndex = unsortedIndex;
    
    mergeSort(sortedTimes, sortedFuel, 0, size - 1, sortedIndex);
    
    head = tail = createNode(sortedTimes[size - 1], sortedFuel[size - 1], sortedIndex[size - 1]);
    listNodes[sortedIndex[size-1]] = tail;
    for (int i = size - 2; i >= 0; i--) { //links list going forward
        temp = head;
        head = createNode(sortedTimes[i], sortedFuel[i], head, sortedIndex[i]);
        listNodes[sortedIndex[i]] = head;
        temp->prev = head;
    }
    head->prev = nullptr;
    temp = nullptr;
    delete temp;
    removeDominatedNodes();
}

void dList::mergeSort(float times[], float fuel[], int i, int k, int index[]) {
    int j;
    
    if (i < k) {
        j = (i + k) / 2;
        
        mergeSort(times, fuel, i, j, index);
        mergeSort(times, fuel, j + 1, k, index);
        merge(times, fuel, i, j, k, index);
    }
}

void dList::merge(float times[], float fuel[], int i, int j, int k, int index[]) {
    int mergedSize, mergePos, leftPos, rightPos;
    float *mergedTimes, *mergedFuel = nullptr;
    int *mergedIndex = nullptr;
    
    mergePos = 0;
    mergedSize = k - i + 1;
    leftPos = i;
    rightPos = j + 1;
    mergedTimes = new float[mergedSize];
    mergedFuel = new float[mergedSize];
    mergedIndex = new int[mergedSize];
    
    while (leftPos <= j && rightPos <= k) {
        if (times[leftPos] < times[rightPos]) {
            mergedTimes[mergePos] = times[leftPos];
            mergedFuel[mergePos] = fuel[leftPos];
            mergedIndex[mergePos] = index[leftPos];
            leftPos++;
        }
        else {
            mergedTimes[mergePos] = times[rightPos];
            mergedFuel[mergePos] = fuel[rightPos];
            mergedIndex[mergePos] = index[rightPos];
            rightPos++;
        }
        mergePos++;
    }
    
    while (leftPos <= j) {
        mergedTimes[mergePos] = times[leftPos];
        mergedFuel[mergePos] = fuel[leftPos];
        mergedIndex[mergePos] = index[leftPos];
        leftPos++;
        mergePos++;
    }
    
    while (rightPos <= k) {
        mergedTimes[mergePos] = times[rightPos];
        mergedFuel[mergePos] = fuel[rightPos];
        mergedIndex[mergePos] = index[rightPos];
        rightPos++;
        mergePos++;
    }
    
    for (mergePos = 0; mergePos < mergedSize; mergePos++) {
        times[i + mergePos] = mergedTimes[mergePos];
        fuel[i + mergePos] = mergedFuel[mergePos];
        index[i + mergePos] = mergedIndex[mergePos];
    }
    delete [] mergedTimes;
    delete [] mergedFuel;
    delete [] mergedIndex;
    mergedIndex = nullptr;
    mergedTimes = nullptr;
    mergedFuel = nullptr;
}

void dList::out(char c = 'a') {
    node *ptr = new node;
    if (c == 'a') {
        ptr = head;
        while (ptr != nullptr) {
            cout << "Time " << ptr->time << "  Fuel " << ptr->fuel << endl;
            ptr = ptr->next;
        }
    }
    else if (c == 'd') {
        ptr = tail;
        while (ptr != nullptr) {
            cout << "Time " << ptr->time << "  Fuel " << ptr->fuel << endl;
            ptr = ptr->prev;
        }
    }
    ptr = nullptr;
    delete ptr;
}

void dList::out(int i, char c = 'a') {
    node *ptr = new node;
    int count = 0;
    if (c == 'a') {
        ptr = head;
        while (ptr != nullptr) {
            cout << "Time " << ptr->time << "  Fuel " << ptr->fuel << endl;
            
            ptr = ptr->next;
            count++;
            if (count == i) break;
        }
    }
    else if (c == 'd') {
        ptr = tail;
        while (ptr != nullptr) {
            cout << "Time " << ptr->time << "  Fuel " << ptr->fuel << endl;
            ptr = ptr->prev;
            count++;
            if (count == i) break;
        }
    }
    ptr = nullptr;
    delete ptr;
}

node* dList::createNode(float time, float fuel, node *next, int index) {
    node *Node = new node;
    Node->time = time;
    Node->fuel = fuel;
    Node->next = next;
    Node->index = index;
    return Node;
}

node* dList::createNode(float time, float fuel, int index) {
    node *Node = new node;
    Node->time = time;
    Node->fuel = fuel;
    Node->next = nullptr;
    Node->prev = nullptr;
    Node->index = index;
    return Node;
}

void dList::removeDominatedNodes() {
    node* temp = new node;
    for (temp = head; temp != NULL; temp = temp->next) {
        if (isNodeDominated(temp)) removeNode(temp);
    }
    temp = nullptr;
    delete temp;
}

void dList::removeDominatedNodes(node* ptr) {
    node* temp = new node;
    if (isNodeDominated(ptr)) {
        removeNode(ptr);
        return;
    }
    
    for (temp = ptr->next; temp != NULL; temp = temp->next) {
        if (isNodeDominated(ptr, temp)) removeNode(temp);
        else
            break;
    }
    temp = nullptr;
    delete temp;
}

void dList::removeDominatedNodesBackwards(node* ptr) {
    node* temp = new node;
    if (isNodeDominated(ptr)) { removeNode(ptr); return; }
    
    for (temp = ptr->prev; temp != NULL; temp = temp->prev) {
        if (isNodeDominated(ptr, temp)) removeNode(temp);
        else break;
    }
    temp = nullptr;
    delete temp;
}

bool dList::isNodeDominated(node* b) {
    if (b->next == nullptr && b->prev == nullptr) //only item in list
        return false;
    if (b->prev == nullptr) { // item is head
        if (b->time >= b->next->time) {
            if (b->fuel < b->next->fuel) return false;
            else return true;
        }
        else return false; // time is less so it can't be dominated
    }
    node* a = new node;
    a = b->prev;
    if (a->time <= b->time && a->fuel <= b->fuel && a != b) {
        a = nullptr;
        delete a;
        return true;
    }
    a = nullptr;
    delete a;
    return false;
}

bool dList::isNodeDominated(node* a, node* b) {
    if (a->time <= b->time && a->fuel <= b->fuel && a != b) { // a dominates b
        return true;
    }
    return false;
}

void dList::removeNode(node* del) {
    if (head == tail) { //list is only one node long
        del = nullptr; delete del;
    }
    else if (del == head) {
        (head->next)->prev = nullptr;
        head = (head->next);
    }
    else if (del == tail) {
        (tail->prev)->next = nullptr;
        tail = (tail->prev);
    }
    else {
        if (del->next != nullptr) (del->next)->prev = del->prev;
        if (del->prev != nullptr) (del->prev)->next = del->next;
    }
}

int dList::insert(float time, float fuel) {
    counter++;
    if (counter == size) { //reallocates dynamic array
        size *= 2;
        node **temp = new node*[size];
        for (int i = 0; i < counter; i++) temp[i] = listNodes[i];
        delete listNodes;
        listNodes = temp;
    }
    
    node* newNode = createNode(time, fuel, counter);
    listNodes[counter] = newNode;
    
    //case 1: addAtFront
    if (newNode->time < head->time) addAtFront(newNode);
    else if (newNode->time == head->time && newNode->fuel < head->fuel) addAtFront(newNode);
    
    //case 2: addAtRear
    else if (newNode->time > tail->time) addAtRear(newNode);
    else if (newNode->time == tail->time) {
        if (newNode->fuel > tail->fuel) addAtRear(newNode);
        else if (newNode->fuel < tail->fuel) addInMiddle(newNode, tail);
    }
    
    //case 3: addInMiddle
    else {
        node* ptr = new node;
        for (ptr = head; ptr != NULL; ptr = ptr->next) {
            if (newNode->time < ptr->time){
                if (ptr == head) addAtFront(newNode);
                else addInMiddle(newNode, ptr);
                break;
            }
            else if (newNode->time == ptr->time) {
                if (newNode->fuel < ptr->fuel) { addInMiddle(newNode, ptr); break; }
            }
        }
        ptr = nullptr;
        delete ptr;
    }
    removeDominatedNodes(newNode);

    return counter;
}

void dList::addAtFront(node* newNode) {
    newNode->next = head;
    newNode->prev = nullptr;
    head->prev = newNode;
    head = newNode;
}

void dList::addAtRear(node* newNode) {
    newNode->prev = tail;
    newNode->next = nullptr;
    tail->next = newNode;
    tail = newNode;
}

void dList::addInMiddle(node* front, node* back) {
    front->next = back;
    front->prev = (back->prev);
    (back->prev)->next = front;
    back->prev = front;
}

// Project 3 additions

void dList::increase_time(int i, float t) {
    listNodes[i]->time += t;
    if (listNodes[i]->next == nullptr) return;
    else if (listNodes[i]->time >= listNodes[i]->next->time) removeNode(listNodes[i]);
}

void dList::decrease_time(int i, float t) {
    listNodes[i]->time -= t;
    if (listNodes[i]->prev == nullptr) return;
    else if (listNodes[i]->time <= listNodes[i]->prev->time)
        removeDominatedNodesBackwards(listNodes[i]);
}

void dList::decrease_fuel(int i, float f) {
    listNodes[i]->fuel -= f;
    if (listNodes[i]->next == nullptr) return;
    else if (listNodes[i]->fuel <= listNodes[i]->next->fuel) removeDominatedNodes(listNodes[i]);
}

int dList::index_before(int i) {
    if (listNodes[i]->prev == nullptr) return -1;
    return listNodes[i]->prev->index;
}

int dList::index_after(int i) {
    if (listNodes[i]->next == nullptr) return -1;
    return listNodes[i]->next->index;
}
