# Algorithm Design and Analysis Assignment4

12310520 芮煜涵

## **Pseudocode** of SPFA + Tarjan's trick

```txt
function TarjanOptimizedSPFA(Graph G = (V, E), weight ℓ, target t):
    Initialize:
        for each node u in V:
            dist[u] ← ∞
            first[u] ← null      
            reverse[u] ← ∅       
            
    dist[t] ← 0

    active ← {t}                
    dormant ← ∅                 

    while active is not empty:
        Select and remove a node w from active

        for each incoming edge (v → w) in E:
            if v is in dormant:
                Skip to next edge

            if dist[v] > dist[w] + ℓ[v, w]:
                // Check for cycle in the pointer graph
                if path from w reaches v through first pointers(w in v's subtree):
                    return "Negative cycle detected"

                // Perform relaxation
                dist[v] ← dist[w] + ℓ[v, w]

                if first[v] ≠ null:
                    Remove v from reverse[first[v]]

                first[v] ← w
                reverse[w].insert(v)

                if v is not in dormant:
                    active.insert(v)

                // Subtree disassembly: mark descendants of v as dormant
                Stack S ← [v]
                while S is not empty:
                    x ← S.pop()
                    dormant.insert(x)

                    if first[x] ≠ null:
                        reverse[first[x]].remove(x)

                    first[x] ← null

                    for each y in reverse[x]:
                        S.push(y)
                    reverse[x] ← ∅

    return dist
```

---

## **Core idea and critical procedures**

**Core Idea:**

The main idea of *Tarjan's Subtree Disassembly Trick* is to make the SPFA algorithm faster by skipping parts of the graph that no longer help in finding the shortest path. It uses a structure called the **pointer graph**, where each node points to the next node along its current shortest path to the target.

When a shorter path to a node is found and its pointer is updated, the algorithm checks if this causes a cycle in the pointer graph. If a cycle appears, it means the original graph has a **negative-weight cycle**, and the algorithm stops immediately.

If there is no cycle, but the change makes some earlier paths invalid (eg. the subtree depending on that node), the algorithm "breaks down" that subtree. All nodes in it are marked as **dormant**, which means they won’t be used again until a better path updates them. This avoids wasting time on paths that are no longer useful and helps the algorithm finish faster, especially in graphs with few cycles or sparse edges.

------

**Critical Procedures:**

1. **Pointer Graph Maintenance:**
   - For each node `v`, a pointer `first[v]` stores the next node on the current best path to the target `t`.
   - A reverse mapping `reverse[u]` is maintained to track which nodes point to `u`, making it easy to explore the subtree rooted at any node.
2. **Relaxation and Cycle Detection:**
   - The algorithm processes nodes using a queue (like SPFA) and applies relaxation on incoming edges `(v → w)`.
   - If a shorter path to `v` is found via `w`, and assigning `first[v] = w` introduces a cycle in the pointer graph, a **negative cycle** is detected and reported immediately.
3. **Dormancy:**
   - If no cycle is formed, but `first[v]` changes, the subtree rooted at `v` (eg. all nodes relying on `v` in their shortest paths) is now invalid.
   - The algorithm recursively traverses the subtree using the reverse pointers and marks all affected nodes as **dormant**.
   - Dormant nodes are removed from the pointer structure and skipped in future relaxations until they are reactivated.
4. **Reactivation :**
   - If a new shorter path to a dormant node is found, it is removed from the dormant set and reinserted into the processing queue.
   - This ensures correctness without redundant work.

---

## Time and Space Complexity Analysis

### **Time Complexity**

Let n = |V| and m = |E|. The algorithm iteratively processes nodes whose distance estimates may improve. Each such node is handled via edge relaxations, pointer updates, cycle detection, and potentially subtree disassembly.

- **Edge Relaxation:**
   For each node dequeued from the active set, the algorithm inspects all incoming edges. Each edge can be relaxed at most O(n) times, corresponding to each time a distance estimate improves. This leads to a total edge processing time of **O(mn)**.
- **Cycle Detection in the Pointer Graph:**
   When a node `v` receives a better distance via a new parent `w`, the algorithm checks whether this creates a cycle in the pointer graph (eg. whether `v` is reachable from `w` via `first` pointers). Each such check may take O(n) time in the worst case, but since each pointer is updated at most once per iteration, the **total cost across all iterations is still O(mn)**.
- **Subtree Disassembly (Tarjan's Trick):**
   When `first[v]` is updated, the old subtree rooted at `v` may no longer represent valid shortest paths. The algorithm traverses this subtree (using reverse pointers) and marks all its nodes as dormant. Although this traversal may cost up to O(n) per update, each node is disassembled at most when its pointer changes, so the **total disassembly cost over the entire algorithm is also O(mn)**.

**Summary:** in total, the worst case is **O(mn)**

### **Space Complexity**

The algorithm maintains several data structures to track distances, pointer relationships, and update status:

- **`dist[v]`** stores the shortest known distance from each node to the target → **O(n)**
- **`first[v]`** records the parent pointer in the current shortest path tree → **O(n)**
- **`reverse[v]`** tracks which nodes point to `v`, used during subtree disassembly → **O(n)**
- **`active` and `dormant` sets** manage nodes to be processed and skipped, respectively → **O(n)**
- **A temporary stack** is used for DFS during subtree cleanup. Its size is bounded by the size of the subtree and reused across iterations → **O(n)** in the worst case

**Summary:** in total, the worst case is **O(n)**

---

## Reasons why Tarjan's trick gain a considerable speedup

In regular SPFA, some nodes may be added to the queue and processed many times, even if their current path is no longer the best. These extra updates happen because old or less optimal paths are still being used, which wastes time.

Tarjan’s trick helps solve this problem by keeping track of the current best path for each node using a pointer. When a better path to a node is found, the old path and all nodes that relied on it are marked as **dormant**. These dormant nodes are skipped in future steps, which avoids wasting time on paths that are no longer useful.

Dormant nodes are not gone forever—they will be reactivated only if a truly better path reaches them later. This way, the algorithm focuses only on the important parts of the graph, and avoids doing the same work again and again.As a result, even in graphs without negative cycles, this method helps the algorithm finish faster by cutting down on useless updates.

### Example

<img src="/Users/ruiyuhan/Library/Application Support/typora-user-images/image-20250509221524252.png" alt="image-20250509221524252" style="zoom:30%;" />

**Initialization:**

- for all vertices v but t, d[v]= ∞; d[t]=0
- for all vertices v, first[v]=null
- for all vertices v, reverse[v]=∅ 
- activate={t}
- dormant=∅

**Step1:**

Relax edges to t:

- d[a]=7,first[a]=t,reverse[t]={a}
- d[b]=5,first[b]=t,reverse[t]={a,b}

Enqueue:activate={a,b}

Tarjan freeze:

- push a,b to stack, mark dormant
- Clear first[a],first[b], remove from reverse[t]

State:

d=[0,7,5,∞,∞], dormant={a,b}, reverse[t]=∅

**Step2:**

Relax edges to a:

- b∈dormant, pass
- d[d]=8,first[d]=a,reverse[a]={d}

Enqueue:activate={b,d}

Tarjan freeze:

- push d to stack,mark dormant
- Clear first[d], remove from reverse[a]

State:

d=[0,7,5,∞,8], dormant={a,b,d}, reverse[a]=∅

**Step3:**

Relax edges to b:

- d[c]=8,first[c]=b,reverse[b]={d}

Enqueue:activate={d,c}

Tarjan freeze:

- push c to stack,mark dormant
- Clear first[c], remove from reverse[b]

State:

d=[0,7,5,8,8],dormant={a,b,d,c},reverse[b]=∅

**Step4:**

Relax edges to d:

- c∈dormant, pass

No updates, remove d from activate; activate={c}

State remains unchanged 

**Step5:**

Relax edges to c:

- B∈dormant, pass

No updates, remove c from activate; activate=∅

**Final state:**

d=[0,7,5,8,8],dormant={a,b,d,c}, no negative cycles.

