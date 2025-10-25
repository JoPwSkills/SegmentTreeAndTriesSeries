#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

// 1. Array based Segment Tree implementatoin
class ArrayBasedSegmentTree {
    private:
        vector<int> tree;
        int n;
    
    void build(const vector<int>& arr, int node, int start, int end) {
        if(start == end) {
            // Leaf node
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            int leftChild = 2 * node + 1;
            int rightChild = 2 * node + 2;

            // Build left and right subtrees
            build(arr, leftChild, start, mid);
            build(arr, rightChild, mid + 1, end);

            // Internal node stores sum of children
            tree[node] = tree[rightChild] + tree[leftChild];
        }
    }

    void visualizeTree(int node, int start, int end, string prefix) {
        cout<< prefix<< "Node[" << node << "] Range[" <<start<<","<<end<<"] ="<<tree[node] <<endl;

        if(start!= end) {
            int mid = (start + end) / 2;
            visualizeTree(2*node + 1, start, mid, prefix + " |--L: ");
            visualizeTree(2*node + 2, mid + 1, end, prefix + " |--R: ");
        } 
    }

    public:
        ArrayBasedSegmentTree(const vector<int>& arr) {
            n = arr.size();
            tree.resize(4*n);
            build(arr, 0, 0, n-1);
        }

        void visualize() {
            cout << "\n== Array-Based Segment Tree==="<<endl;
            // cout<< "Tree array: [";

            visualizeTree(0, 0, n-1, "");
              
        }
};

// 2. Recursive Segment Tree (Different merge operation)
class RecursiveSegmentTree {
    private:
    vector<int> tree;
    int n;
    string operation;

    int merge(int left, int right) {
        if(operation == "sum") return left + right;
        if(operation == "min") return min(left, right);
        if(operation == "max") return max(left, right); 
        if(operation == "product") return left * right;

        return left + right;
    }

    void build(const vector<int>& arr, int node, int start, int end) {
        if(start == end) {
            tree[node] = arr[start];
        }else {
            int mid = (start + end) / 2;
            build(arr, 2 * node + 1, start, mid);
            build(arr, 2* node + 2 , mid + 1, end);

            // Merge based on operation
            tree[node] = merge(tree[2* node + 1], tree[2*node +2]);
        }
    }
    
    void visualizeTree(int node, int start, int end, string prefix) {
        cout<< prefix << "Node[" << node << "] Range[" << start<<","<<end<<"] = "<<tree[node] << endl;

        if(start!= end) {
            int mid = (start + end) / 2;
            visualizeTree(2 * node + 1, start, mid, prefix + " |--L: ");
            visualizeTree(2* node + 2, mid + 1, end, prefix + " |--R: ");
        }
    }

    public:

        RecursiveSegmentTree(const vector<int>& arr, string op) {
            n = arr.size();
            tree.resize(4*n);
            operation = op;
            build(arr, 0, 0, n-1);
        }

        void visualize() {
            cout << "\n ===Recursive Segment Tree====";
            visualizeTree(0, 0, n-1, "");
        }

        vector<int>& getTree() {
            return tree;
        }
};


// 3. Bottom-up (Iterative) Segment Tree
class BottomUpSegmentTree {
    private:
        vector<int> tree;
        int n;
        
        void visualizeLevelOrder(int height) {
            for(int level = 0; level < height; level ++ ){
                int start = (int) pow(2, level);
                int end = min((int) pow(2, level + 1), 2 * n);

                cout<< "Level" << level <<": ";
                for(int i= start; i< end && i < 2* n; i++) {
                    if(i<n) {
                        cout<< "Node[" << i << "] = (" << tree[i]<<")";
                    } else {
                        cout<< "Leaf[" << (i-n) << "] = (" << tree[i] <<")";
                    }
                }

                cout<< endl;
            }
        }

        public:

        BottomUpSegmentTree(const vector<int>& arr) {
            n = arr.size();
            tree.resize(2*n); 

            // Copy array elements to second half of tree
            for(int i=0; i<n ;i++){
                tree[n+i] = arr[i];
            }
            // Build tree by calculating parents
            for(int i = n-1; i>0; i--) {
                tree[i] = tree[2*i] + tree[2*i + 1]; 
            }
        }

        void visualize() {
            cout<< "\n === Bottom up (Iterative) Segment tree=====";
            cout<< "Tree Array: [";
            for(int i =0; i< 2*n ; i++) {
                cout<< tree[i];
                if(i< 2*n-1) cout<< ", ";
            }
            cout<< "]"<< endl;

            int height = (int) ceil(log2(n)) + 1;

            visualizeLevelOrder(height);
        }
};

// 4. Object based node segment tree
class NodeBasedSegmentTree {
    private: 
    struct Node {
        int value;
        int start, end;
        Node* left;
        Node* right;

        // constructor
        Node(int s, int e) : start(s), end(e), value(0), left(nullptr), right(nullptr) {}

        ~Node() {
            delete left;
            delete right;
        }
    };

    Node* root;


    Node* build(const vector<int>& arr, int start, int end) {
        Node* node = new Node(start, end);

        if(start==end) {
            node->value = arr[start];
        }

        else {
            int mid = (start + end) / 2;
            node->left = build(arr, start, mid);
            node->right = build(arr, mid + 1, end);

            node->value = node->left->value + node->right->value;
        }

        return node;
    }

    void visualizeTree(Node* node, string prefix) {
        if(node == nullptr) return;

        cout<< prefix << "Node = " << node->value << " Range["<< node->start << ","<< node->end<<"]" << endl;

        if(node->left != nullptr || node->right != nullptr) {
            if(node->left != nullptr) {
                visualizeTree(node->left, prefix + " |--L ");
            }
            if(node->right != nullptr) {
                visualizeTree(node->right, prefix + " |--R ");
            }
        }

    }

    public:

    NodeBasedSegmentTree(const vector<int>& arr) {
        root = build(arr, 0, arr.size() - 1);
    }

    ~NodeBasedSegmentTree() {
        delete root;
    }

    void visualize() {
        visualizeTree(root, "");
    }

};


// Helper function to print array
void printArray(const vector<int>& arr) {
    cout << "[";
    for(size_t i=0; i<arr.size(); i++) {
        cout<< arr[i];
        if(i<arr.size() - 1) cout << ", ";
    }
    cout <<"]" << endl;
}

// Main demo with test cases
int main() {
    cout << "================================================" << endl;
    cout << "============== SEGMENT TREE CODE================" << endl;
    cout << "================================================" << endl;

    // Test case 1
    // cout<< "TEST CASE -1 "<<endl;
    // vector<int> arr1 = {1, 3, 5, 7, 9, 11};
    // cout<<"Input Array";
    // printArray(arr1);

    // ArrayBasedSegmentTree tree1(arr1);
    // tree1.visualize();


    // Test case 2
    // cout<< "TEST CASE - 2 Different operations" << endl;
    // vector<int> arr2 = {4, 2, 8, 1 , 5};

    // cout<< "Input array: ";
    // printArray(arr2); 
    // cout<< "Seg Tree Array: ";

    // RecursiveSegmentTree sumTree(arr2, "sum");
    // printArray(sumTree.getTree());

    // sumTree.visualize();
    
    // RecursiveSegmentTree minTree(arr2, "min");
    // minTree.visualize();

    // RecursiveSegmentTree maxTree(arr2, "max");
    // maxTree.visualize();

    // RecursiveSegmentTree productTree(arr2, "product");
    // productTree.visualize();

    // Test case 4
    // cout<<" TEST CASE 4 - ITERATIVE APPROACH";
    // vector<int> arr3 = {4, 2, 8 , 1 ,5, 1, 2, 1};

    // cout<< "Input Array: ";
    // printArray(arr3);

    // BottomUpSegmentTree tree3(arr3);

    // tree3.visualize();


    // TEST CASE 4 - NODE BASED APPROACH
    cout<< "TEST CASE 4 - NODE BASED APPROACH"<<endl;

    vector<int> arr4 = {4, 2, 8 , 1 ,5, 1, 2, 1};

    cout<< "Input array: ";
    printArray(arr4);

    NodeBasedSegmentTree tree4(arr4);

    tree4.visualize();


   vector<int> arr5 = {4, 2, 8 , 1 ,5, 0, 0, 0};

    cout<< "Input array: ";
    printArray(arr5);

    NodeBasedSegmentTree tree5(arr5);

    tree5.visualize();
    return 0;

}
