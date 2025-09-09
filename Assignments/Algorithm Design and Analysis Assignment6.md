# Algorithm Design and Analysis Assignment 6

12310520 芮煜涵

## **Choose 3-SAT $\leq_p$ Traveling-Salesman**

## Step1:**Prove Directed Hamiltonian Cycle problem is NP**

The certificate for this problem is a sequence of vertices. To verify the solution, we need to **check two things**: first, whether every vertex in the graph is visited exactly once in the sequence; and second, whether each consecutive pair of vertices in the sequence is connected by an edge in the graph. This can be done in polynomial time, as we only need to verify the existence of the edges between consecutive vertices in the list. 

---

## Step2:Prove 3-SAT $\leq_p$ Directed Hamiltonian Cycle

### 2. **Reduction Explanation:**

We reduce the **3SAT** problem to the **Directed Hamiltonian Cycle (DHC)** problem. Given a 3SAT instance:  
$$ X = (x_1, x_2, \dots, x_n) $$, with clauses $$ C = (C_1, C_2, \dots, C_m) $$, we construct a directed graph $$ G = (V, E) $$ where a Hamiltonian cycle corresponds to a satisfying assignment of the 3SAT instance.

#### **(1) Construct Vertices:**
For each variable $$ x_i $$, create 3 vertices:  
$$ v_{i, 3m+1}, v_{i, 3m+2}, v_{i, 3m+3} $$ and add edges for each $$ 1 \leq j \leq 3m+2 $$:  
$$ (v_{i,j}, v_{i,j+1}) $$ and $$ (v_{i,j+1}, v_{i,j}) $$.

#### **(2) Add Variable Paths:**
For each $$ 1 \leq i \leq n $$, add paths with edges from $$ (v_{i,1}, v_{i,11}) $$ and $$ (s, v_{3m+3}) $$, where $$ s $$ is the start vertex.

#### **(3) Add Extra Nodes:**
Introduce two extra nodes $$ s $$ and $$ t $$, the start and end points, and connect them to the paths to ensure a valid Hamiltonian cycle.

#### **(4) Construct Graph:**
The graph is constructed such that the paths correspond to truth assignments of variables. A Hamiltonian cycle in this graph corresponds to a satisfying assignment for the 3SAT instance.

#### **(5) For Each Clause $$ C_j $$:**
For each clause $$ C_j $$, define a corresponding node $$ c_j $$ and add edges from $$ v_{i,j} $$ to $$ c_j $$ . The cycle will pass through $$ c_j $$ depending on whether the variable is assigned true or false.(Picture from:https://blog.csdn.net/xiazdong/article/details/8333854)

![image-20250606185759663](/Users/ruiyuhan/Library/Application Support/typora-user-images/image-20250606185759663.png)

### **Proof of the Reduction:**

1. **Graph Construction:**
   - Each variable $$ x_i $$ is represented by 3 vertices.
   - Edges are added to connect the vertices, ensuring that the graph's structure corresponds to the variable's truth assignment.
   - Extra nodes $$ s $$ and $$ t $$ are added for starting and ending the cycle.

2. **Hamiltonian Cycle Corresponds to Satisfying Assignment:**
   - If the 3SAT formula is satisfiable, there exists a truth assignment where each clause is satisfied.
   - The Hamiltonian cycle will follow the paths corresponding to the truth assignment, ensuring each clause is satisfied.

3. **Reduction Validity:**
   - A Hamiltonian cycle exists in the graph if and only if the 3SAT instance is satisfiable. Thus, solving the DHC problem solves the 3SAT problem.

### **Time Complexity:**
The graph construction takes $$ O(n + m) $$, where $$ n $$ is the number of variables and $$ m $$ is the number of clauses, so the reduction is polynomial-time.

In conclusion, we have shown that the **Directed Hamiltonian Cycle** problem is NP-complete by providing a polynomial-time reduction from the **3SAT** problem.

---

## Step3:Prove Traveling Salesman Problem is NP

To show that the **Traveling Salesman Problem (TSP)** is in NP, we need to demonstrate that a proposed solution can be verified in polynomial time.

1. **Solution (Certificate)**: A solution consists of a specific tour, which is a permutation of cities. This sequence can be easily proposed as a potential solution.
2. **Verification in Polynomial Time**: Given the proposed tour, the verifier checks:
   - **All cities are visited exactly once** (which can be done by checking if the list contains each city exactly once).
   - **The total length of the tour is at most the given bound** (by summing the distances between consecutive cities and comparing it to the bound, which takes $O(n)$ time).

Since these checks can be performed in polynomial time, **TSP is in NP**.

---

## Step4:Prove Directed Hamiltonian Cycle $\leq_p$ Traveling-Salesman

Given a directed graph $$ G = (V, E) $$, we define the following instance of TSP.

#### **Construct the TSP Instance:**

- For each vertex $$ v_i $$ in the graph $$ G $$, create a corresponding city $$ v_i' $$ in the TSP instance.
- We define the distance between cities as follows:
  - $$ d(v_i', v_j') = 1 $$ if there is an edge $$ (v_i, v_j) $$ in $$ G $$,
  - $$ d(v_i', v_j') = 2 $$ otherwise.

#### **Claim:**

We claim that $$ G $$ has a Hamiltonian cycle if and only if there exists a tour of length at most $$ n $$ in the corresponding TSP instance.

- **Forward direction**: If $$ G $$ has a Hamiltonian cycle, the ordering of the corresponding cities defines a tour of length $$ n $$, where each pair of consecutive cities is connected by an edge with distance 1.

- **Backward direction**: If there is a tour of length at most $$ n $$, the length of the tour is the sum of $$ n $$ terms, each of which is at least 1. Therefore, all terms must be equal to 1, and each pair of cities corresponding to consecutive nodes in the tour must be connected by an edge. This implies that the ordering of cities corresponds to a Hamiltonian cycle in the original graph $$ G $$.

Thus, the **Hamiltonian Cycle** problem can be reduced to the **Traveling Salesman Problem**, proving that **TSP** is NP-complete.

---

## Step5:Transistivity

3-SAT $\leq_p$ Directed Hamiltonian Cycle ,

Directed Hamiltonian Cycle $\leq_p$ Traveling-Salesman ,

Therefore, 3-SAT $\leq_p$ Traveling-Salesman.