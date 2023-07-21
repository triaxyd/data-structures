#include <iostream>
#include <random>

using namespace std;

std::random_device rd;  // random seed
std::mt19937 eng(rd());     // random engine 
std::uniform_real_distribution<double> dist(0.5, 1.0);
const double a = dist(eng);   // generate random number within range



template <class T>
class LinkedList;

// Linear List Node
template <class T>
class LinkedListNode {
    friend class LinkedList<T>;
public:
    LinkedListNode<T>* GetNext() const { return next; }
    T GetData() const { return data; }
private:
    T data;
    LinkedListNode<T>* next;
};


// Linear List
template <class T>
class LinkedList {
public:
    LinkedList() { head = nullptr; }
    void Insert(const T& value);
    bool IsEmpty() const {return (head == nullptr);};
    LinkedListNode<T>* GetMiddleNode() const;
    LinkedListNode<T>* GetHead() const { return head; }
private:
    LinkedListNode<T>* head;
};

//insert to list
template <class T>
void LinkedList<T>::Insert(const T& value) {
    LinkedListNode<T>* newNode = new LinkedListNode<T>;
    newNode->data = value;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode;
    } else {
        LinkedListNode<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

//Middle element of a list will be the root of new subtree
template <class T>
LinkedListNode<T>* LinkedList<T>::GetMiddleNode() const {
    LinkedListNode<T>* slow = head;
    LinkedListNode<T>* fast = head;
    while (fast != nullptr && fast->GetNext() != nullptr) {
        fast = fast->GetNext()->GetNext();
        slow = slow->GetNext();
    }

    return slow;
}




template <class T>
class AWeightTree;

//AWeightTreeNode class
template <class T>
class AWeightTreeNode {
public:
    friend class AWeightTree<T>;

    AWeightTreeNode() { parent = leftChild = rightChild = nullptr; }
    AWeightTreeNode(const T& e) { data = e; leftChild = rightChild = parent = nullptr; }

    AWeightTreeNode<T>* GetLeftChild() const { return leftChild; }
    AWeightTreeNode<T>* GetRightChild() const { return rightChild; }

    void SetLeftChild(AWeightTreeNode<T>* node) { leftChild = node; }
    void SetRightChild(AWeightTreeNode<T>* node) { rightChild = node; }
private:
    T data;
    AWeightTreeNode<T>* leftChild;
    AWeightTreeNode<T>* rightChild;
    AWeightTreeNode<T>* parent;
};


// The A-Weighted Balanced Binary Search Tree
template <class T>
class AWeightTree {
public:
    AWeightTree() { root = nullptr; max_size = 0;}
    bool Empty() const { return (root == nullptr); }
    void Insert(const T& value);
    void Remove(const T& value);
    LinkedList<T> Search(const T& k1, const T& k2) const;
    T Find_Neighbor(const T& k, const string& direction) const;

    int Size(AWeightTreeNode<T>* node) const;
    void InOrderTraversal(AWeightTreeNode<T>* node, LinkedList<T>& linearList) const;
private:
    AWeightTreeNode<T>* root;
    int max_size;
    void SearchRecursive(AWeightTreeNode<T>* node, const T& k1, const T& k2, LinkedList<T>& result) const;
    T FindPredecessor(AWeightTreeNode<T>* node, const T& k) const;
    T FindSuccessor(AWeightTreeNode<T>* node, const T& k) const;
};


//////////////////////////////////////////

//size of a tree node
template <class T>
int AWeightTree<T>::Size(AWeightTreeNode<T>* node) const {
    if (node == nullptr)
        return 0;
    else
        return 1 + Size(node->leftChild) + Size(node->rightChild);
}


//inorder gia metatropi tou dentrou se list se ascending order
template <class T>
void AWeightTree<T>::InOrderTraversal(AWeightTreeNode<T>* node, LinkedList<T>& linearList) const {
    if (node == nullptr) {
        return;
    }
    // get the left child
    InOrderTraversal(node->leftChild, linearList);
    // insert the node to the list
    linearList.Insert(node->data);
    // get the right child
    InOrderTraversal(node->rightChild, linearList);
}




//create a balanced tree from list
template <class T>
AWeightTreeNode<T>* CreateBalancedTree(const LinkedList<T>& linearList) {
    //list is empty
    if (linearList.IsEmpty()) {
        return nullptr;
    }

    //middle node of the linear list
    LinkedListNode<T>* midNode = linearList.GetMiddleNode();

    //create a new tree node with the value of the middle node / root
    AWeightTreeNode<T>* subRoot = new AWeightTreeNode<T>(midNode->GetData());

    //recursive creation of left and right subtrees
    //create the left subtree
    LinkedList<T> leftList;
    LinkedListNode<T>* current = linearList.GetHead();
    while (current != midNode) {
        leftList.Insert(current->GetData());
        current = current->GetNext();
    }
    //left child of node points to the root of the left subtree
    subRoot->SetLeftChild(CreateBalancedTree(leftList));

    //create the right subtree
    LinkedList<T> rightList;
    current = midNode->GetNext();
    while (current != nullptr) {
        rightList.Insert(current->GetData());
        current = current->GetNext();
    }
    //right child of node points to the root of the right subtree
    subRoot->SetRightChild(CreateBalancedTree(rightList));

    return subRoot;
}




//insert a new node to the tree
template <class T>
void AWeightTree<T>::Insert(const T& value) {
    // Insert node just like BST
    cout<<"STARTING INSERTION OF NODE WITH VALUE: "<<value<<endl;
    AWeightTreeNode<T>* newNode = new AWeightTreeNode<T>(value);

    if (root == nullptr) {
        root = newNode;
        max_size = 1;
        cout<<"Root node with value: "<<root->data << " inserted"<<endl;
        return;
    }

    AWeightTreeNode<T>* current = root;
    AWeightTreeNode<T>* parent = nullptr;

    //find the position to insert the new node
    while (current != nullptr) {
        parent = current;

        if (value < current->data) {
            current = current->leftChild;
        } else if (value > current->data){
            current = current->rightChild;
        }else{
            cout<<"Remove duplicate "<<current->data;
            return;
        }
        
    }

    //assign the parent node to the new node
    newNode->parent = parent;

    //place the new node as left or right child
    if (value < parent->data) {
        parent->leftChild = newNode;
    } else {
        parent->rightChild = newNode;
    }

    cout<<"Node with value: "<<newNode->data << " inserted with parent : " << parent->data <<endl; 

    //go all the way to the root and check the balance for every node
    AWeightTreeNode<T>* checkNode = parent;
    while (checkNode != nullptr) {
        if (checkNode->leftChild && (Size(checkNode->leftChild) > Size(checkNode) * a)) {
            //left violation
            cout<<"---Left Child Violation---\n";

            //convert the violated subtree into a linear list in ascending order
            LinkedList<T> linearList;
            InOrderTraversal(checkNode->leftChild, linearList);

            //detach the subtree from the unbalanced node
            checkNode->leftChild = nullptr;

            //create the new subtree of the unbalanced node
            AWeightTreeNode<T>* newLeftSubtree = CreateBalancedTree(linearList);

            //the left child of the node will be the newly created subtree
            checkNode->leftChild = newLeftSubtree;

            cout<<"---Left Subtree of node " << checkNode->data << " Rebalanced---\n";
        }
        if (checkNode->rightChild && (Size(checkNode->rightChild) > Size(checkNode) * a)) {
            //right violation
            cout<<"---Right Child Violation---\n";

            //convert the violated subtree into a linear list in ascending order
            LinkedList<T> linearList;
            InOrderTraversal(checkNode->rightChild, linearList);

            //detach the subtree from the unbalanced node
            checkNode->rightChild = nullptr;

            //create the new subtree of the unbalanced node
            AWeightTreeNode<T>* newRightSubtree = CreateBalancedTree(linearList);

            //the right child of the node will be the newly created subtree
            checkNode->rightChild = newRightSubtree;

            cout<<"---Right Subtree of node " << checkNode->data << " Rebalanced---\n";

        }
        //go to the parent of the old unbalanced node and repeat
        checkNode = checkNode->parent;
    }

    //update the max size if needed
    int size = Size(root);
    if (size > max_size) {
        max_size = size;
        cout<<"Max Size is now " << max_size <<endl; 
    }
    cout<<"INSERTION COMPLETED"<<endl;
}



//remove an element from the tree
template <class T>
void AWeightTree<T>::Remove(const T& value) {
    cout<<"STARTING DELETION OF NODE WITH VALUE: "<<value<<endl;
    //delete a node just like bst
    AWeightTreeNode<T> *p = root;
    AWeightTreeNode<T> *pp = 0;

    while(p && p->data!=value){
        pp = p;
        if(value < p->data) p=p->leftChild;
        else p = p->rightChild;
    }
    if(!p) return;
    if(p->leftChild && p->rightChild){
        AWeightTreeNode<T> *s = p->leftChild, 
                                *ps = p;
        while(s->rightChild){
            ps=s;
            s=s->rightChild;
        }
        p->data = s->data;
        p=s;
        pp=ps;
    }
    AWeightTreeNode<T> *c;
    if(p->leftChild) c = p->leftChild;
    else c = p->rightChild;

    if(p==root) root = c;
    else{
        if(p==pp->leftChild)pp->leftChild=c;
        else pp->rightChild = c;
    }
    //update parent
    if (c)
        c->parent = pp;
    cout<<"Deleting node with value: "<<p->data<<endl; 
    delete p;


    //check if tree is unbalanced
    if (Size(root) < max_size * a) {
        cout<<"Rebalancing Tree"<<endl;

        //do the same procedure as insertion
        LinkedList<T> linearList;
        InOrderTraversal(root, linearList);
        root = CreateBalancedTree(linearList);

        //update max_size 
        max_size = Size(root);
    }
    cout<<"DELETION COMPLETED"<<endl;
}



//search an element between a given interval 
template <class T>
LinkedList<T> AWeightTree<T>::Search(const T& k1, const T& k2) const {
    //the list that contains the results
    LinkedList<T> result;
    //start from root and recusrively search
    SearchRecursive(root, k1, k2, result);

    return result;
}

//recursive search
template <class T>
void AWeightTree<T>::SearchRecursive(AWeightTreeNode<T>* node, const T& k1, const T& k2, LinkedList<T>& result) const {
    if (node == nullptr) {
        return;
    }

    //the node data is between the given interval
    if (node->data >= k1 && node->data <= k2) {
        result.Insert(node->data);
    }
    //go to the left subtree and find more elements that may fall within range
    if (node->data > k1) {
        SearchRecursive(node->leftChild, k1, k2, result);
    }
    //go to the right subtree and find more elements that may fall within range
    if (node->data < k2) {
        SearchRecursive(node->rightChild, k1, k2, result);
    }
}



//find the predecessor or successor of a given value in the tree 
template <class T>
T AWeightTree<T>::Find_Neighbor(const T& k, const string& direction) const {
    if (direction == "predecessor") {
        return FindPredecessor(root, k);
    } else if (direction == "successor") {
        return FindSuccessor(root, k);
    } else {
        return T();
    }
}

template <class T>
T AWeightTree<T>::FindPredecessor(AWeightTreeNode<T>* node, const T& k) const {
    if (node == nullptr) {
        return T();
    }
    if (k <= node->data) {
        //if k <= node data then predecessor in left subtree
        return FindPredecessor(node->leftChild, k);
    } else {
        // If k is greater than the current node data , the predecessor could be in the right subtree or current node 
        T predecessor = FindPredecessor(node->rightChild, k);
        if (predecessor == T()) {
            //if the predecessor was not found in the right subtree , the current node is the predecessor
            return node->data;
        } else {
            return predecessor;
        }
    }
}

template <class T>
T AWeightTree<T>::FindSuccessor(AWeightTreeNode<T>* node, const T& k) const {
    if (node == nullptr) {
        return T();
    }
    if (k >= node->data) {
        //if k >= current node data , successor is on the right subtree
        return FindSuccessor(node->rightChild, k);
    } else {
        //if k is less than the current node data , the successor could be in the left subtree or the current node
        T successor = FindSuccessor(node->leftChild, k);
        if (successor == T()) {
            //if the successor was not found in the left subtree, the current node is the successor
            return node->data;
        } else {
            return successor;
        }
    }
}




int main() {
    AWeightTree<int> tree;
    tree.Insert(10);
    tree.Insert(20);
    tree.Insert(23);
    tree.Insert(25);
    tree.Insert(28);
    tree.Insert(30);
    tree.Insert(3);

    tree.Remove(25);
    tree.Remove(20);
    tree.Remove(10);


    LinkedList<int> result = tree.Search(5, 32);
    LinkedListNode<int>* currentNode = result.GetHead();
    cout<<"The elements in the interval are: ";
    while (currentNode != nullptr) {
        cout <<"-"<<currentNode->GetData() << "-";
        currentNode = currentNode->GetNext();
    }


    int successor = tree.Find_Neighbor(10,"successor");
    cout<<endl<<"The successor of the given value is: "<<successor<<endl;

    return 0;
}

