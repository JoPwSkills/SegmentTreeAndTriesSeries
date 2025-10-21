import java.util.*;

// arr → nums
// node → i
// Tree → segTree 
// build() → buildtree()
// start → l
// end → r

// 1. Array-based Segment Tree Construction
class ArrayBasedSegmentTree {
    private int[] tree;
    private int n;

    public ArrayBasedSegmentTree(int[] arr){
        n = arr.length;
        tree = new int[4*n]; // Size is 4*n to accomodate all nodes.
        build(arr, 0, 0, n-1);
    }   

    private void build(int[] arr, int node, int start, int end){
        if(start == end){
            // leaf node
            tree[node] = arr[start];
        }
        else {
            int mid = (start + end )/2;
            int leftChildIdx = 2 * node + 1;
            int rightChildIdx = 2 * node + 2;

            // Build left and right subtrees
            build(arr, leftChildIdx, start, mid);
            build(arr, rightChildIdx, mid+1, end);

            tree[node] = tree[leftChildIdx] + tree[rightChildIdx];            
        }
    }

    public void visualize() {
        System.out.println("\n ==== Array Based Segment Tree======");
        System.out.println("Tree array: " + Arrays.toString(Arrays.copyOf(tree, getTreeSize())));
        visualizeTree(0, 0, n-1, 0, "");
    }

    private void visualizeTree(int node, int start, int end, int depth, String prefix){
        System.out.println(prefix+ "Node[" + node + "] Range[" + start + "," + end+"] = " + tree[node]);

        if(start!=end) {
            int mid = (start + end) / 2;

            visualizeTree(2* node + 1, start, mid, depth+1, prefix + " |--L: ");
            visualizeTree(2*node + 2, mid+1, end, depth + 1, prefix + " |--R: ");
        }
    }

    private int getTreeSize(){
        for(int i = tree.length - 1; i>=0; i--) {
            if(tree[i]!=0) return i+1;
        }
        return 0;
    }

}


// 2. Recursive Segment Tree (Different merge operations)
class RecursiveSegmentTree {
    private int[] tree;
    private int n;
    private String operation;

    public RecursiveSegmentTree(int[] arr, String op) {
        n = arr.length;
        tree = new int[4*n];
        operation = op; // 'sum', 'min', 'max', product
        build(arr, 0, 0, n-1);
    }

    private void build(int[] arr, int node, int start, int end) {
        if(start == end) {
            tree[node] = arr[start]; 
            return;
        }

        int mid = (start + end) / 2;

        build(arr, 2 * node + 1 , start, mid);
        build(arr, 2 * node + 2, mid + 1 , end);

        // Merge based on opeartion

        tree[node] = merge(tree[2* node + 1] , tree[2*node + 2]);

    }

    private int merge(int x, int y) {
        switch(operation) {
            case "sum": return x+y;
            case "min": return Math.min(x, y);
            case "max": return Math.max(x, y);
            case "product": return x*y;
            default: return x+y;
        }
    }

    public void visualize() {
        System.out.println("\n======= Recursive Segment Tree (" + operation + ") ===" );
        visualizeTree(0, 0, n-1,0, "");
    }

    private void visualizeTree(int node, int start, int end, int depth, String prefix) {
       System.out.println(prefix+ "Node[" + node + "] Range[" + start + "," + end+"] = " + tree[node]);

        if(start!=end) {
            int mid = (start + end) / 2;

            visualizeTree(2* node + 1, start, mid, depth+1, prefix + " |--L: ");
            visualizeTree(2*node + 2, mid+1, end, depth + 1, prefix + " |--R: ");
        }
    }

}

// 3 Bottom-up (Iterative) Segment Tree
class BottomUpSegmentTree {
    private int[] tree;
    private int n;


    public BottomUpSegmentTree(int[] arr) {
        n = arr.length;
        tree = new int[2*n];

        // Copy arr elements to second half of tree 

        for(int i = 0; i<n ; i++) {
            tree[n+i] = arr[i];
        }

        // Build tree by calculating parents
        for(int i=n-1; i>0; i--) {
            tree[i] = tree[2*i] + tree[2*i+1];
        }
    }

    public void visualize() {
        System.out.println("\n===== Bottom-up (Iterative) Segment Tree=====\n");
        int height = (int) Math.ceil(Math.log(n)/Math.log(2)) + 1;

        visualizeLevelOrder(height);
    }

    private void visualizeLevelOrder(int height) {
        for( int level = 0; level<height; level++) {
            int start = (int) Math.pow(2, level); 
            int end = Math.min((int) Math.pow(2, level+1), 2*n);

            System.out.println("Level" + level + ": ");
            for(int i=start; i<end && i< 2*n; i++) {
                if(i<n) {
                    System.out.println("Node[" + i + "] = (" + tree[i] + ") ");
                }
                else {
                    System.out.println("Leaf [" + (i-n) + "] = (" + tree[i] + ") ");
                }
            }
        }
        System.out.println();
    }
}

// 4. Object-based Node Segmenet Tree
class NodeBasedSegmentTree {
    static class Node {
        int value;
        int start, end;
        Node left, right;

        Node(int s, int e) {
            start = s;
            end = e;
        }

    }

    private Node root;

    public NodeBasedSegmentTree(int[] arr) {
        root = build(arr, 0, arr.length-1);
    }

    private Node build(int[] arr, int start, int end) {
        Node node = new Node(start, end);
        if(start == end) {
            node.value = arr[start];
            return node;
        }
        int mid = (start+end)/2;
        node.left = build(arr, start, mid); 
        node.right = build(arr, mid+1, end);
        node.value = node.left.value + node.right.value;

        return node;
    }

    public void visualize() {
        System.out.println("\n======Object-Based Node Segment Tree====");
        visualizeTree(root, "");
    }

    private void visualizeTree(Node node, String prefix) {
        if(node == null) return;

        System.out.println(prefix + "Range [" + node.start + "," + node.end + "] = " + node.value);


        if (node.left !=null || node.right!=null) {
            if(node.left!=null) {
                visualizeTree(node.left, prefix + " |--L: ");
            }
            if(node.right!=null) {
                visualizeTree(node.right, prefix + " |--R: ");
            }
        }
    }

}

// Main Demo with test cases class
public class SegmentTreeCreation {
    public static void main(String[] args) {
        System.out.println("========= SEGMENT TREE CREATION & VISUALIZATION========== \n");

        System.out.println("================================================");
        // Test case 1
        int[] arr1 = {1, 3, 5, 7, 9, 11};

        System.out.println("Input Array: " + Arrays.toString(arr1));
        
        ArrayBasedSegmentTree segTree1 = new ArrayBasedSegmentTree(arr1);

        segTree1.visualize();
        System.out.println("================================================");
        // Test case 2: Different merge operations
        System.out.println("TEST CASE 2: Different Operations\n");

        int[] arr2 = {4, 2, 8, 1, 5};
        System.out.println("Input Array: "+ Arrays.toString(arr2));

        RecursiveSegmentTree sumTree = new RecursiveSegmentTree(arr2, "sum");

        sumTree.visualize();

        RecursiveSegmentTree maxTree = new RecursiveSegmentTree(arr2, "max");

        maxTree.visualize();

        RecursiveSegmentTree minTree = new RecursiveSegmentTree(arr2, "min");

        minTree.visualize();
        System.out.println("================================================");

        // Test case 3: Bottom up approach
        System.out.println("======= TEST CASE 3: BOTTOM UP APPROACH=====\n");

        int[] arr3 = {1, 3, 5, 7, 9 ,11};

        BottomUpSegmentTree tree3 = new BottomUpSegmentTree(arr3);
        tree3.visualize();

        System.out.println("================================================");
        // Test case 4: Node-based structure
        System.out.println("TEST CASE 4: NODE-BASED STRUCTURE\n");

        int[] arr4 = {1, 3, 5, 7, 9, 11};

        NodeBasedSegmentTree tree4 = new NodeBasedSegmentTree(arr4); 
        tree4.visualize();

        // Test case 5: Power of 2 vs non-power of 2
        System.out.println("TEST CASE 5; Non power of 2 array size");
        int[] arr5 = {5, 10, 15, 20, 25, 30, 35};

        ArrayBasedSegmentTree tree5 = new ArrayBasedSegmentTree(arr5);
        tree5.visualize();

        System.out.println("================================================");
        System.out.println("TEST CASE 6; Power of 2 array size");
        int[] arr6 = {5, 10, 15, 20, 25, 30, 35, 40};

        ArrayBasedSegmentTree tree6 = new ArrayBasedSegmentTree(arr6);
        tree6.visualize();


    }
}
