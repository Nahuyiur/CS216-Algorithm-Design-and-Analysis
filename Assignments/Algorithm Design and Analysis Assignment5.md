# Algorithm Design and Analysis Assignment 5

12310520 芮煜涵

## Pseudocode and sufficient details

```txt
Function KM_Match(cost, n):
    Input:
        cost[x][y] — cost of assigning node x ∈ X to node y ∈ Y
    Output:
        totalCost — total cost of the minimum-cost perfect matching

    matchY ← [-1] * n                     // matchY[y] = x ∈ X currently assigned to y ∈ Y , -1 means not assigned yet
    pX ← [0] * n                          // price function for nodes in X
    pY ← [0] * n                          // price function for nodes in Y

    // Initialize prices on Y-side so that all reduced costs are non-negative
    for x = 0 to n-1:
        for y = 0 to n-1:
            pY[y] ← max(pY[y], cost[x][y])

    // Iteratively grow the matching from each unmatched x ∈ X
    for x = 0 to n-1:
        KM_BFS(x, cost, n, pX, pY, matchY)

    // Compute total cost of final matching
    totalCost ← 0
    for y = 0 to n-1:
        if matchY[y] ≠ -1:
            totalCost += cost[matchY[y]][y]

    return totalCost
```

```txt
Function KM_BFS(x0, cost, n, pX, pY, matchY):

    visitedY ← [False] * n                 // Y-side nodes visited in the alternating tree
    parent ← [-1] * n                      // Track the augmenting path
    slack ← [∞] * n                        // slack[y] = min reduced cost to reach y ∈ Y

    y ← -1                                 // Start with virtual unmatched Y node
    matchY[y] ← x0                         // Temporarily match x0 to this virtual y

    while True:
        x ← matchY[y]
        delta ← ∞
        visitedY[y] ← True

        for j = 0 to n-1:
            if visitedY[j]: continue
            reduced ← cost[x][j] - pX[x] - pY[j]           // reduced cost: r(x,y)
            if reduced < slack[j]:
                slack[j] ← reduced
                parent[j] ← y
            if slack[j] < delta:
                delta ← slack[j]
                nextY ← j

        // Update prices to maintain feasibility and expose new tight edges
        for j = 0 to n-1:
            if visitedY[j]:
                pX[matchY[j]] += delta
                pY[j] -= delta
            else:
                slack[j] -= delta

        y ← nextY
        if matchY[y] == -1:
            break

    // Augment the matching along the path found
    while y ≠ -1:
        prev ← parent[y]
        matchY[y] ← matchY[prev]
        y ← prev
```

---

## Explain the pseudocode

## Core Idea

The main idea of the Kuhn-Munkres algorithm is to solve the minimum-cost perfect matching problem in a bipartite graph. The graph has two parts, called `X` and `Y`, and every edge between a node `x ∈ X` and a node `y ∈ Y` has a known cost `c(x, y)`. The goal is to find a one-to-one matching between `X` and `Y` that minimizes the total cost.

To do this, the algorithm assigns a **price** to every node: `p(x)` for each `x` in `X`, and `p(y)` for each `y` in `Y`. Based on these prices, it defines the **reduced cost** of an edge as:
$$
r(x,y)=c(x,y)−p(x)−p(y)
$$
An edge is called a **tight edge** if its reduced cost is zero. That is, `r(x, y) = 0`. Tight edges are considered cost-balanced under the current price setting and are the only edges eligible to be used when constructing or extending the matching.

In each iteration, the algorithm attempts to find an **augmenting path**. This is a path that starts from an unmatched node in `X`, follows alternating matched and unmatched tight edges, and ends at an unmatched node in `Y`. If such a path exists, the algorithm updates the matching by flipping the matched status along this path, thereby increasing the size of the matching.

If no augmenting path can be found under the current set of tight edges, the algorithm will **adjust the prices** of certain nodes to create new tight edges. This is done carefully to ensure the reduced cost of existing tight edges remains non-negative, and feasibility is preserved.

Apart from that, the price function `p(x)` and `p(y)` is a core part of the algorithm. It is initialized as follows:

- Set `p(x) = 0` for all `x` in `X`.
- Set `p(y) = max { c(x, y) | x ∈ X }` for each `y` in `Y`.

This guarantees that all initial reduced costs are non-negative. The price values are adjusted during the algorithm to help guide the search for tight edges and to ensure that eventually, all nodes can be perfectly matched using only tight edges. By following this approach, the algorithm ensures that the final matching is both perfect and of minimum total cost.

### Critical Procedures

The Kuhn-Munkres algorithm mainly includes the following important procedures:

#### 1. Price Initialization

Before matching starts, the algorithm needs to initialize the prices of the nodes:

- Set `p(x) = 0` for every node `x` in set `X`.
- For each node `y` in set `Y`, set `p(y)` to the largest cost of any edge connected to `y`. That means:

```
p(y) = max { c(x, y) | for all x in X }
```

This ensures that all reduced costs `r(x, y) = c(x, y) - p(x) - p(y)` are non-negative at the beginning.

#### 2. Finding Augmenting Path

For each unmatched node in `X`, the algorithm tries to find an **augmenting path** by building a tree. The path must:

- Start at an unmatched node `x ∈ X`.
- Follow only tight edges (`r(x, y) = 0`).
- End at an unmatched node `y ∈ Y`.

If such a path is found, the algorithm flips the matching along the path to increase the size of the matching by one.

#### 3. Slack Value Maintenance

During the search, the algorithm keeps track of **slack values** for all unvisited nodes in `Y`. For each such node `y`, the slack is:

```
slack(y) = min { c(x, y) - p(x) - p(y) | x is in the tree }
```

The slack value tells us how close `y` is to becoming connected by a tight edge. A smaller slack means the edge is closer to being valid for matching.

#### 4. Price Update

If no augmenting path can be found using current tight edges, the algorithm updates the prices to create new tight edges. The smallest slack value `Δ` among all unvisited `Y` nodes is calculated:

```
Δ = min { slack(y) | y ∉ T }
```

Then the price functions are updated as follows:

- Increase `p(x)` by `Δ` for all `x` in the current tree `S` (from `X` side).
- Decrease `p(y)` by `Δ` for all `y` in the current tree `T` (from `Y` side).
- Subtract `Δ` from the slack values of all `y ∉ T`.

This update guarantees that at least one new tight edge appears, so that the search can continue.

#### 5. Augment the Matching

Once a new unmatched node in `Y` is reached through a tight edge, the algorithm traces back the path using the parent pointers and flips all edges on that path:

- Matched edges become unmatched.
- Unmatched edges become matched.

This completes one successful augmentation and increases the size of the matching by one.

The above steps are repeated for every node in `X` until a perfect matching is found.

---

## **Correctness**, **running** **time**, and **space complexity**

### Correctness

To prove the correctness of the algorithm, we need to verify two properties:

1. Every update of prices preserves the feasibility of the reduced cost constraints.
2. The algorithm terminates with a perfect matching of minimum total cost.

Let us consider how the reduced costs behave during a price adjustment. Suppose the current alternating tree includes a subset `S` of nodes from `X` and a subset `T` from `Y`. Let `Δ` be the minimum slack over all edges `(x, y)` where `x ∈ S` and `y ∉ T`, defined as:

```
Δ = min { c(x, y) - p(x) - p(y) | x in S, y not in T }
```

Then we update prices as follows:

- For every `x` in `S`, increase `p(x)` by `Δ`
- For every `y` in `T`, decrease `p(y)` by `Δ`

Now consider how the reduced cost `r(x, y)` changes:

- If `x ∈ S` and `y ∈ T`: both prices change by `Δ` in opposite directions, so `r(x, y)` remains unchanged.
- If `x ∉ S` and `y ∈ T`: only `p(y)` decreases, so `r(x, y)` increases and remains non-negative.
- If `x ∈ S` and `y ∉ T`: `r(x, y)` decreases by `Δ`. But since `Δ` is chosen to be the minimum slack over such edges, `r(x, y)` becomes exactly 0,which means a new tight edge is formed.
- If `x ∉ S` and `y ∉ T`: no price change; `r(x, y)` is unchanged.

Thus, the update preserves the condition `r(x, y) ≥ 0` for all edges, maintaining dual feasibility.

Since at least one new edge becomes tight, the alternating tree can grow. When the tree reaches an unmatched node in `Y`, we obtain an augmenting path, which increases the matching size by one.

As there are only `n` nodes in `X`, and each augmentation adds one match, the algorithm performs at most `n` augmentations and eventually finds a perfect matching.

### Time Complexity

- The algorithm performs `n` rounds, one for each node in `X`.
- In each round, it tries to find an augmenting path using tight edges. Finding such a path involves scanning up to `n` nodes and updating prices and slack values, which takes $O(n^2)$ time.
- Therefore, the total running time of the algorithm is $O(n^3)$.

### Space Complexity

- The algorithm needs $O(n^2)$ space to store the original cost matrix `c(x, y)`.
- Arrays for prices (`p(x)`, `p(y)`), matchings, visited flags, parent pointers, and slack values each use $O(n)$ space.
- So the total space required is:

$$
O(n^2) + O(n) = O(n^2)
$$

---

## **Formulate** a **max-cost** **max-matching** problem 

To solve a **maximum-cost perfect matching** problem using a **minimum-cost algorithm**, we can transform the cost matrix in the following way:

### Transformation Strategy

Let `C` be the original cost matrix. Let `C_max` be the largest value among all `c(x, y)` in the matrix. We define a new cost matrix `C'` by:

```
C'(x, y) = C_max - C(x, y)
```

Then we apply the Kuhn-Munkres algorithm on `C'` to compute a minimum-cost perfect matching.

### Why This Works

- Since `C_max` is a constant, this transformation inverts the cost structure: edges with higher original cost now have smaller values in `C'`.
- Because all perfect matchings have the same number of edges, subtracting each edge's cost from the same constant only changes the total by a fixed amount:

```
Sum C'(x, y) = k * C_max - Sum C(x, y)
```

where `k` is the number of edges in the perfect matching (which is always `n`).

- Minimizing the total in `C'` is equivalent to maximizing the total in the original cost matrix `C`.

Using this transformation, we can solve a maximum-cost perfect matching problem by applying the Kuhn-Munkres algorithm as a minimum-cost solver. The result will yield the correct solution when mapped back to the original costs.