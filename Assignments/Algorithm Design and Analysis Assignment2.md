# Algorithm Design and Analysis Assignment2 

12310520 芮煜涵

## Lecture implementation 

The algorithm is composed of two major steps: **contraction** and **expansion.**

Contraction: find cycles with all edges from the min-cost edge set, and contract them as a **supernode**; then keep the in-edges to the circle and the out-edges from the circle, adding them to the supernode. Repeat contraction until only one vertex left.

Expansion: reconstruct tge MDST in the original graph by **reversing the contraction step**; split the super nodes back into their original vertices and restore the edges that were contracted; in every min-cost in-edge circle, we will keep the parents of each vertex, and add all the edges of the circle expect the one, and we will find the vertex the circle starts and find its parent. In the end, we obtain the complete MDST of the original graph.

Here are the implementation of Chu-Liu algorithm in lecture notes,

```text
Chu-Liu(G,r,c){
Initialization:
    - create an empty set of incoming edges E*.
    - for each node v ≠ r:
        - choose one min-cost edge entering v and add it to set E*.
        - find the min-cost one of any edge entering v: y(v) ← min(c(u, v)).
        - c'(u, v) ← c(u, v) - y(v) for each edge (u, v) entering v, adjust the edge weights
          
Cycle Detection:
    - if (E* forms an arborescene)
    		return T = (V, E*) (the MDST has been found)
   	- else
    - Otherwise:
      - C ← directed cycle in E*.
      - Contract C to a single supernode and get G' = (V', E').
     - recursively apply the algorithm:
          T' ← Chu-Liu(G', r, c')
          
     - expand T' back to an arborescence T in G by adding all but one edge of cycle C.
     - Return T.
}
```

## More efficient implementation 

### Contraction

The contraction phase reduces the graph to a single super-vertex by ==selecting minimum-weight incoming edges with a priority queue and merging cycles as they appear==. 

1. **Edge Selection**: For each node in the graph, identify the incoming edge with the smallest weight. Each node maintains a priority queue of all edges entering it, sorted by cost in asending order, and picks the edge at the top (lowest weight).
2. **Traversal and Cycle Detection**: Start at any node and follow its selected minimum-weight incoming edge to the next node. Mark each visited node. Continue this process, moving from node to node along the chosen edges. **If a node is revisited (already marked), a cycle is detected.**
3. **Cycle Contraction:** When a cycle is found, combine all nodes in the cycle into a supernode. Update the graph by:
   - Replacing the cycle with this supernode.
   - Adjusting the incoming edges to the supernode: for any edge pointing to a node in the cycle from outside, redirect it to the super-vertex, subtracting the cost of the cycle’s internal edge (if applicable) to preserve cost consistency. The priority queue is mantained, but only to update those of the supernode, and the vertices the supernode has entering edges in. Also reserve the edges pointing from points in circle to outside.
4. **Repeat**: From the new supernode, repeat the process of selecting the minimum-weight incoming edge and checking for cycles. Continue until the entire graph is reduced to one supernode.

### Expansion

The expansion phase reconstructs the MDST by ==reversing the contractions in a bottom-up manner==. It uses the history of contractions to restore the original graph with the optimal tree.

1. **Contraction Tracking**: During the contraction phase, maintain a record of how supernodes were formed—each supernode is linked to the nodes it contains from the cycle it replaced.
2. **Reverse Process:** Start with the final supernode. For each supernode:
   - Identify the nodes and edges that were merged to form it.
   - Restore these nodes to the graph.
   - Reattach the minimum-weight incoming edge that was chosen for each node during contraction, ensuring it connects the correct source and destination nodes.
3. **Ajustment of circle**: Process supernodes in reverse order of contraction. We need to choose one edge in the directed circle to exclude by figuring how outside vertex enter the circle (for every vertex only has one entering edge, so the vertex now has two entering edges need to give up the entering edge in the circle). 
4. **Final Adjustment**: Once all nodes are restored and edges reattached, remove the incoming edge to the root (since an MDST has no edge entering its root).

## Missing details 

### Priority Queues

For each vertex v, we maintains queue Q[v] for all in-edges, sorted by cost. The top of the queue always holds the min-cost in-edge for selection.

- **Implementation**: 

  Use **Binomial Heap**, nodes have cost, from_vertex, to_vertex.

  Create empty Q[v] for each v, for each edge (u, v, w) in the graph, insert it into P[v] (as it is an in-edge to v).

- **Merge two priority queues** (this is why we choose binomial heap, two priority queues, say P[u] and P[v])

  1. Extract the root lists of P[u] and P[v] (each root represents a binomial tree).
  2. Merge these lists into a single list sorted by tree degree.
  3. Scan the merged list to handle trees of equal degree:
     - If two trees have the same degree (e.g., degree k), compare their root node cost
     - Make the root with the smaller cost the new root, and the other root its child (pointing to the smaller root).
     - Update the new tree’s degree to k+1.
  4. Repeat step 3 until no trees of equal degree remain.

### Union-Find

Manage supernodes, and to track which vertices are merged into the same supernode.

- **Implementation**

  Each v has root[v] (parent or root, initially v), rank[v] (it represents tree depth, initially 0).

  Find(v): If root[v] != v, recursively find root and compress path.

  Union(u, v): Find roots ru=Find(u), rv=Find(v); If ru == rv, u and v are already in the same set; otherwise, compare rank[ru] and rank[rv]:

  ​	If rank[ru] < rank[rv], set parent[ru] = rv.

  ​	If rank[ru] > rank[rv], set parent[rv] = ru.

  ​	If rank[ru] == rank[rv], arbitrarily choose one root (e.g., ru) and increment rank[ru] += 1.

### Other records

 In addition to priority queues and the Union-Find data structure, each vertex v requires several key records: 

- min_in[v] stores the currently selected minimum-weight in-edge, initially set to null; 
- prev_v[v] records the previous vertex, initially set to null; 
- super_v[v] indicates the ID of the supernode to which v currently belongs, initially set to v itself; 
- out_edges[v] maintains a list of outgoing edges from v to external vertices, initially empty. 
- For each edge, the records include its source vertex src, destination vertex dst, and cost c. 
- A history list history is maintained to store information about each new supernode created during contraction, including the supernode ID, the set of original vertices, and the supernode’s minimum in-edge.

### Contraction Phase

Contraction initializes each vertex v’s Q[v], root[v]=v, rank[v]=0, and sets min_in[v], prev_v[v], super_v[v], out_edges[v] to defaults. 

Start from any v, extract min-cost in-edge e=(u,v,c) from Q[v], set min_in[v]=e, prev_v[v]=u. 

If u visited (root[u]≠u and in cycle), backtrack prev_v to find cycle C. Create new supernode s, set root[c]=s, super_v[c]=s for all c in C. 

Redirect external in-edges to s, subtract cycle edge cost. Merge Q[c] into Q[s], record in history. 

Repeat until one supernode remains.

### Expansion Phase

Expansion selects root r, backtracks along root[r] to final supernode, collects R as all u where super_v[u]≠u. 

For each u in R, set super_v[u]=u, restore min_in[u]. Finally, remove min_in[r] to ensure MDST root has no in-edge.

## Time complexity

### **Initialization Phase**:

- Initialize auxiliary data structures for each vertex v (e.g., Q[v], root[v], rank[v], min_in[v]), taking O(1) per vertex, totaling O(n).  Similar for each edge, totaling O(m).
- Insert each edge into its priority queue Q[v], taking O(m log n) in total.

### **Contraction Phase**:

- Each minimum-cost in-edge extraction from Q[v] takes O(log n), processing all m edges worst-case yields O(m log n). 
- Merging priority queues during supernode contraction also takes O(log n) per merge, with at most O(n) merges, totaling O(n log n).
- The dominant time is O(m log n).

### **Expansion Phase**:

- Backtracking from root r along root[r] to the top takes O(n). 
- Updating hierarchy for each of the n vertices (e.g., super_v[v], min_in[v]) takes O(log n) each, totaling O(n log n). Expansion phase total is O(n log n).

### **Overall Time Complexity**:

- Combining all phases, the time complexity is:

  **O(m log n) + O(m log n) + O(n log n) = O(m log n).** 

## Space complexity

### **Priority Queue Storage**:

- Each vertex v’s Q[v] stores all in-edges, totaling m edges, requiring O(m) space. 
- Each edge appears in exactly one queue, no duplication.

### **Supernodes and Contraction Tree**:

- Supernodes and contraction tree store each supernode’s children and parent, at most O(n) relationships, totaling O(n) space.

### **Auxiliary Data Fields**:

- Each vertex v stores min_in[v], prev_v[v], super_v[v], out_edges[v], etc., fixed space per vertex, totaling O(n).

### **Overall Space Complexity**:

- Total space is O(m) (queues) + O(n) (supernodes) + O(n) (auxiliary fields) **= O(m+n).** 