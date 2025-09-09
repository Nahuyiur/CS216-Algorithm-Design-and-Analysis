# Algorithm Design and Analysis Assignment1

12310520 芮煜涵

## 1. Algorithm Design

```txt
Initialize each student and each college to be free
while(some student is free and hasn't applied to every college)
	choose such a student s
	c=1st college on s'list to which s has not yet applied
	if(s has negative evaluation on c)
		s goes to no college
		delete s from the free student list
	else
  	if(c has positive evaluation to s)
  		if(c still has capacity)
  			assign s to c
  			delete s from the free student list
  		else if(there exists student w that c has enrolled, but c has worse evaluation on w than on s)
          c rejects w,w becomes free
          assign s to c
          delete s from the free student list
  			else
  				c rejects s
  	else
  		c rejects s
```

## 2. Complexity

We assume that there are N students applying for M colleges.

**time complexity: **O(MN)

For the worst case, all the preferences are positive, and students's preferences for the colleges are in descending order, while colleges' preferences for the students are in correspondingly ascending order. In this case, if the capacity is limited, every student may apply to each college, leading to O(MN) time complexity.

**space complexity: **O(MN)

For we need to use N lists to store the students' preferences for the colleges, each list's length is M;

And also need to use M lists to store the colleges' preferences for the students, each list's length is N;

## 3. Stable matching

### Definition

No pair of student and college has incentive to undermine the current matching by joint action.

This means for any current pair (s1,c1),(s2,c2), there exists NO following scenario:

- s1 is not enrolled to c2
- s1 prefers c2 than c1
- either c2 is capable of enrolling more students, or c2 prefers s1 than s2

### Stability judge

The extended GS algorithm outputs stable matching.

### Proof

(By contradiction)

Suppose s1-c2 is an unstable pair in the Gale-Shapley matching S\*, which means that S\* is unstable.

Case 1: s1 never applies for c2.

​	this means that s1 prefers c1 than c2, for students apply for colleges in decreasing order of preference. Therefore, s1-c2 is stable.

Case 2: s1 applied for c2 but got rejected(right away or later) because of lack of capacity

​	this means that there exists $s_{c1},s_{c2}...,s_{cp}$​ , all these students are preferred by c2 than s1.

Although  $s_{c1},s_{c2}...,s_{cp}$ may not be the final students enrolled by c2, but if updation is made, the final students enrolled by c2 will still be better than s1. Therefore, s1-c2 is stable.

In both cases, s1-s2 can't be unstable, leading to a contradiction to the assumption. Thus, the Gale-Shapley matching S\* must be stable.

## 4. Student-optimal matching

### Definition

Student-optimal matching means that every student is enrolled in their best valid college. It means that students are assigned to the colleges that they prefer most and they can be enrolled in.

### Judge student-optimal matching

GS algorithm produces the student-optimal matching.

## Proof

(By contradiction)

Suppose that the GS matching S\* is student-optimal, which means that some student is not enrolled in he/her best valid college. Since students apply for colleges in decreasing order of preference, some student is rejected by he/her valid college.

Let s be the first student and let c be the first valid college of s that rejects s.

When s is rejected, c has enrolled the enough students $s_{1},s_{2}...,s_{p}$ , whom c prefers to s. We know $s_{1},s_{2}...,s_{p}$ were not rejected by any valid partner at this point, so $s_{1},s_{2}...,s_{p}$​ prefers c to any other valid colleges.

There exists a stable matching S where s is enrolled in c, since c is a valid college of s. Then at least one student in $s_{1},s_{2}...,s_{p}$  can no more be enrolled in c, we say, $s_k$ . Let $c_2$​ be the college that $s_k$ is enrolled in S. From the above:

- $s_k$ prefers c over $c_2$
- c prefers $s_k$ over s

This creates an unstable pair of ($s_k,c$) in S, leading to a contradiction to the assumption that S is stable. Therefore, the GS matching S\* must be student-optimal.



