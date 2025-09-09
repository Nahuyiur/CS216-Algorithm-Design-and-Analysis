# Algorithm Design and Analysis Assignment 3

12310520 芮煜涵

# Accelerating String Matching Using FFT

## 1. Standard String Matching (Without Wildcards)

String matching is a fundamental problem in computer science, with applications in text editing, bioinformatics (e.g., DNA sequence matching), and search engines.  
Given:

- A **pattern string**  A  of length  m ,
- A **text string**  B  of length  n ,

we aim to find all positions  x  such that the substring of  B  starting from  x  matches  A  exactly.

Instead of using traditional methods like KMP, which run in $O(n)$ time, we explore an FFT-based method. Although its theoretical complexity is $O(n\log n) $, FFT has a small constant factor and better performance on large-scale or parallel systems.

---

### 1.1 Core Idea

Define a basic difference function:
$$
C(x, y) = A(x) - B(y)
$$
Construct the total matching function:
$$
P(x) = \sum_{i=0}^{m-1} (A[i] - B[x-m+i+1])^2
$$
Only when $ P(x) = 0 $ do we consider the match successful.

---

### 1.2 Derivation and Decomposition

Expanding the square:
$$
P(x) = \sum_{i=0}^{m-1} A[i]^2 + \sum_{i=0}^{m-1} B[x-m+i+1]^2 - 2\sum_{i=0}^{m-1} A[i]B[x-m+i+1]
$$

This splits into three parts:
- The constant term $T = \sum A[i]^2 $,
- The cumulative squares of  B , computed using prefix sums,
- The sliding dot product between  A  and  B , computed via convolution.

Thus, we can reformulate:
$$
P(x) = T + (f(x) - f(x-m)) - 2g(x)
$$
where:

-  f(x)  is the prefix sum of squares of  B ,
-  g(x)  is the convolution of  A  (reversed) and  B .

FFT efficiently computes the convolution in $O(n\log n)$ time.

---

### 1.3 Code Implementation (Without Wildcards)

```c
void FFT_Match(char *s1, char *s2, int m, int n)
{
    for (int i = 0; i < m; i++) 
        A[i].r = s1[i] - 'a' + 1;
    for (int i = 0; i < n; i++) 
        B[i].r = s2[i] - 'a' + 1;

    reverse(A, A + m);

    double T = 0;
    for (int i = 0; i < m; i++) 
        T += A[i].r * A[i].r;

    f[0] = B[0].r * B[0].r;
    for (int i = 1; i < n; i++) 
        f[i] = f[i-1] + B[i].r * B[i].r;

    FFT(A, len, 1);
    FFT(B, len, 1);

    for (int i = 0; i < len; i++) 
        g[i] = A[i] * B[i];

    FFT(g, len, -1);

    for (int x = m-1; x < n; x++)
    {
        double P = T + f[x] - f[x-m] - 2 * g[x].r;
        if (fabs(P) < eps) 
            printf("%d ", x - m + 2);
    }
}
```

## 2. Wildcard String Matching (Supporting '*')

In many real-world scenarios, the pattern string may contain **wildcards** (e.g., `'*'`), meaning the character at that position can match any character.  
Given:

- A **pattern string**  A  (may contain wildcards),
- A **text string**  B  (may also contain wildcards),

our task is to find all positions where  B  matches  A  considering wildcard rules.

Traditional KMP cannot handle wildcards easily; thus, an FFT-based approach provides an elegant and efficient solution.

---

### 2.1 Redefining the Matching Function

Set wildcard characters to have value 0.  
The modified difference function is defined as:
$$
C(x, y) = (A(x) - B(y))^2 \times A(x)B(y)
$$
Thus:
- If either  A(x)  or  B(y)  is a wildcard (i.e., 0), the product becomes 0, allowing free matching.
- Otherwise, check strict equality.

Define the total matching function:
$$
P(x) = \sum_{i=0}^{m-1} (A[i] - B[x-m+i+1])^2 A[i]B[x-m+i+1]
$$

---

### 2.2 Derivation and Decomposition

Expanding:
$$
P(x) = \sum_{i=0}^{m-1} S(m-i-1)^3 B(x-m+i+1) + \sum_{i=0}^{m-1} S(m-i-1) B(x-m+i+1)^3 - 2\sum_{i=0}^{m-1} S(m-i-1)^2 B(x-m+i+1)^2
$$

where  S(i)  is the reversed  A .

This can be expressed as three convolutions:
$$
P(x) = \sum_{i+j=x} S(i)^3 B(j) + \sum_{i+j=x} S(i) B(j)^3 - 2 \sum_{i+j=x} S(i)^2 B(j)^2
$$
Thus, the problem reduces to three standard convolutions.

---

### 2.3 Code Implementation (With Wildcards)

```c
void FFT_match(char *s1, char *s2, int m, int n)
{
    reverse(ss1, ss1 + m);

    for (int i = 0; i < m; i++) 
        A[i] = (s1[i] != '*') ? (s1[i] - 'a' + 1) : 0;
    for (int i = 0; i < n; i++) 
        B[i] = (s2[i] != '*') ? (s2[i] - 'a' + 1) : 0;

    for (int i = 0; i < len; i++) 
        a[i] = Comp(A[i]*A[i]*A[i], 0), b[i] = Comp(B[i], 0);
    FFT(a, len, 1); FFT(b, len, 1);
    for (int i = 0; i < len; i++) 
        P[i] = P[i] + a[i]*b[i];

    for (int i = 0; i < len; i++) 
        a[i] = Comp(A[i], 0), b[i] = Comp(B[i]*B[i]*B[i], 0);
    FFT(a, len, 1); FFT(b, len, 1);
    for (int i = 0; i < len; i++) 
        P[i] = P[i] + a[i]*b[i];

    for (int i = 0; i < len; i++) 
        a[i] = Comp(A[i]*A[i], 0), b[i] = Comp(B[i]*B[i], 0);
    FFT(a, len, 1); FFT(b, len, 1);
    for (int i = 0; i < len; i++) 
        P[i] = P[i] - a[i]*b[i]*Comp(2, 0);

    FFT(P, len, -1);

    for (int i = m-1; i < n; i++) 
        if (fabs(P[i].r) <= 1e-7) 
            printf("%d ", i-m+2);
}
```

## Summary

By reformulating string matching as convolution problems, FFT-based methods achieve $ O(n\log n) $ performance for both standard and wildcard matching cases.  This technique offers significant performance improvements over traditional approaches.

-----------



# Convolution Power via FFT

## 1. Introduction

In many combinatorial counting problems, probability computations, and signal processing tasks, we often require the multiple self-convolution (i.e., convolution power) of a sequence or polynomial. 

For example, given a sequence a, its k-fold convolution corresponds to the coefficients of $A(x)^k$, where A(x) is the generating polynomial of a.

Directly computing repeated convolutions has a high complexity: a single convolution using FFT already reduces complexity from $O(n^2)$ to $O(n \log n)$, and sequentially performing k convolutions would require $O(n \log n \log k)$ operations.

To further optimize, we leverage the **Convolution Theorem**, converting multiple convolutions in the time domain into a single exponentiation in the frequency domain. That is:
- First perform an FFT on the sequence,
- Raise each frequency component to the k-th power,
- Perform an inverse FFT to recover the result.

The overall complexity becomes $O(N \log N)$, where N is the transform length. This offers a significant improvement over traditional methods.

---

## 2. Basic Idea

Suppose the original sequence (or polynomial) has coefficients $a_0, a_1, \dots, a_{n-1}$​, forming
$$
A(x) = \sum_{i=0}^{n-1} a_i x^i
$$
The coefficients of C(x) = A(x)^k satisfy:
$$
c_m = \sum_{i_1+i_2+\cdots+i_k=m} a_{i_1} a_{i_2} \cdots a_{i_k}
$$
Direct computation of c_m is costly. By the **Convolution Theorem**, convolution in the time domain corresponds to multiplication in the frequency domain.

The concrete steps are:
- **Zero Padding**: Extend a to a length N where $N \geq k(n-1)+1$ and N is a power of two.
- **FFT**: Compute $\hat{a}_j$ by FFT.
- **Frequency Domain Power**: Raise each $\hat{a}_j$ to the k-th power.
- **Inverse FFT**: Perform inverse FFT to get c in the time domain.
- **Rounding**: Due to floating-point errors, take the real part and round to the nearest integer.

Thus, with a single FFT and a single IFFT, we compute the k-fold convolution efficiently.

---

## 3. Mathematical Derivation

Given a zero-padded sequence $(a_0, \dots, a_{N-1})$​, its forward DFT is:
$$
\hat{a}_j = \sum_{m=0}^{N-1} a_m \omega_N^{jm}, \quad \omega_N = e^{-2\pi i/N}, \quad j=0,\dots,N-1
$$
Inverse DFT is:
$$
a_m = \frac{1}{N} \sum_{j=0}^{N-1} \hat{a}_j \omega_N^{-jm}
$$
According to the **Convolution Theorem**, for convolution $(c = a * a * \cdots * a)$ (k times):
$$
\hat{c}_j = (\hat{a}_j)^k
$$
Thus,
$$
c_m = \frac{1}{N} \sum_{j=0}^{N-1} (\hat{a}_j)^k \omega_N^{-jm}
$$
This shows that by computing the FFT of a, raising each frequency component to the k-th power, and applying the inverse FFT, we obtain the coefficients of $A(x)^k$.

It is crucial to ensure that N is large enough to avoid wrap-around effects from circular convolution.

---

## 4. Technical Details

- **Length Selection**: Choose $N \geq k(n-1)+1$ and N should be a power of two.
- **Precision Handling**: Due to floating-point inaccuracies during FFT/IFFT, take only the real part and apply rounding.
- **Number-Theoretic Transform (NTT)**: If exact integer precision is required, especially for modular computations, NTT can be used instead of FFT.
- **Efficiency**: One forward FFT, one pointwise exponentiation, and one inverse FFT; total complexity $O(N \log N)$.
- **Exponentiation Optimization**: For large k, use fast exponentiation for complex numbers.

This method is especially useful when k is large and multiple sequential convolutions are impractical.

---

## 5. Manual FFT-Based Implementation

```python
import math

# Cooley-Tukey FFT implementation
def fft(a, invert):
    n = len(a)
    j = 0
    for i in range(1, n):
        bit = n >> 1
        while j & bit:
            j ^= bit
            bit >>= 1
        j ^= bit
        if i < j:
            a[i], a[j] = a[j], a[i]

    length = 2
    while length <= n:
        ang = 2 * math.pi / length * (-1 if invert else 1)
        wlen = complex(math.cos(ang), math.sin(ang))
        for i in range(0, n, length):
            w = complex(1)
            for j in range(i, i + length // 2):
                u = a[j]
                v = a[j + length // 2] * w
                a[j] = u + v
                a[j + length // 2] = u - v
                w *= wlen
        length <<= 1

    if invert:
        for i in range(n):
            a[i] /= n

# Convolution power using FFT
def conv_power(a, k):
    n = len(a)
    N = 1
    while N < k * (n - 1) + 1:
        N <<= 1

    fa = [complex(x, 0) for x in a] + [0] * (N - n)

    fft(fa, False)
    for i in range(N):
        fa[i] = fa[i] ** k
    fft(fa, True)

    result = [round(fa[i].real) for i in range(k * (n - 1) + 1)]
    return result

# Example usage
a = [1, 2, 1]  # Represents 1 + 2x + 1x^2
k = 3
res = conv_power(a, k)
print(res)  # Output: [1, 6, 15, 18, 12, 3]
```

---

## Summary

The FFT-based convolution power method transforms multiple convolutions into simple exponentiations in the frequency domain, achieving $O(N \log N)$​ complexity. This technique dramatically accelerates previously infeasible large-scale convolutions and is a vital tool in competitive programming and engineering applications.

----



# FFT-Based Image Denoising

## 1. Introduction

Noise is an inevitable artifact in real-world images, originating from factors like sensor imperfections, environmental interference, or compression errors. Denoising aims to recover the true image content by removing unwanted noise while preserving important details.

Among various denoising techniques, **Fast Fourier Transform (FFT)** offers a powerful frequency-domain approach. FFT transforms an image from the spatial domain (where pixels represent brightness) to the frequency domain (where each component represents a particular frequency's contribution). Through carefully designed frequency-domain filtering, noise—especially high-frequency noise—can be efficiently suppressed.

---

## 2. The principle that FFT Perform Image Denoising

### 2.1 Image and Frequency Domain Relationship

An image can be considered as a 2D signal. Each pixel's intensity represents the signal value at that position. When applying a 2D FFT, the image is decomposed into its frequency components:

- **Low frequencies**: Represent smooth variations — large structures like sky, walls, or backgrounds.
- **High frequencies**: Represent abrupt changes — edges, fine textures, and noise.

Thus, the frequency domain provides a way to distinguish between structured content (low frequency) and noise (often high frequency).

### 2.2 Interpretation of the FFT Spectrum

After applying a 2D FFT to an image:
- The center of the spectrum corresponds to low-frequency components.
- The outer regions correspond to high-frequency components.

In the spectrum visualization:
- **Bright center**: Strong low-frequency (overall shape and smooth areas).
- **Bright spots in outer areas**: High-frequency signals, usually fine details or noise.

Hence, analyzing the FFT spectrum reveals where noise and important image contents are distributed.

### 2.3 How FFT Enables Denoising

Most noise, particularly Gaussian noise and salt-and-pepper noise, appears in the high-frequency domain. Thus, noise suppression can be achieved by:

- **Low-pass Filtering**: Attenuate high-frequency components, keeping only low frequencies that represent major image structures.
- **Notch Filtering**: Target and remove specific frequencies associated with periodic noise.
- **Custom Frequency Masking**: Suppress manually identified noisy regions in the frequency domain.

After filtering, an **Inverse FFT (IFFT)** brings the modified frequency domain data back into the spatial domain, yielding a denoised image.

---

## 3. Technical Details

### 2D FFT Formula

The 2D DFT for an image  f(x,y)  of size  M \times N  is:

$$
F(u,v) = \sum_{x=0}^{M-1} \sum_{y=0}^{N-1} f(x,y) \cdot e^{-2\pi i (\frac{ux}{M} + \frac{vy}{N})}
$$
The inverse 2D DFT is:

$$
f(x,y) = \frac{1}{MN} \sum_{u=0}^{M-1} \sum_{v=0}^{N-1} F(u,v) \cdot e^{2\pi i (\frac{ux}{M} + \frac{vy}{N})}
$$
Here, (u,v)  represents frequency coordinates, and  (x,y) represents spatial coordinates.

### Common Filtering Techniques

- **Ideal Low-Pass Filter**: Sharp cutoff; removes all frequencies above a threshold.
- **Gaussian Low-Pass Filter**: Smoothly decays high frequencies.
- **Butterworth Low-Pass Filter**: Balances between ideal and Gaussian filters.

Gaussian and Butterworth filters are often preferred for avoiding artifacts caused by sharp cutoffs.

---

## 4. Practical Example

![Figure_1](/Users/ruiyuhan/Desktop/Figure_1.png)

- **Original Image**: Contains noticeable noise.
- **FFT Spectrum**: Bright center (image structure), noisy edges (high-frequency noise).
- **Denoised Image**: Smooth appearance, noise significantly reduced, major structures preserved.

---

## Summary

FFT provides a powerful way to analyze images in the frequency domain. Since noise often resides in high-frequency components, FFT-based filtering allows selective suppression of noise while preserving important image features.

---



# Pattern Detection and Cross-Correlation Using FFT

## 1. Introduction

In many signal processing and image analysis tasks, it is important to determine the similarity between two signals or locate a pattern within a larger signal. 

**Cross-correlation** is a mathematical tool that measures the similarity between two signals as a function of the lag applied to one of them. However, direct computation of cross-correlation has $O(n^2)$ complexity, which becomes infeasible for large data.

Using **FFT**, we can significantly speed up cross-correlation to $O(n \log n)$ by transforming the problem into the frequency domain.

---

## 2. Basic Principle

### 2.1 Cross-Correlation Definition

Given two discrete signals x[n] and y[n], the cross-correlation $R_{xy}[m]$ is defined as:

$$
R_{xy}[m] = \sum_{n} x[n] \cdot y[n+m]
$$
It measures how much x and a shifted version of y overlap.


### 2.2 Cross-Correlation via FFT

Using properties of the Fourier Transform:

$$
\mathcal{F}\{x \star y\}(f) = \overline{X(f)} \cdot Y(f)
$$
where:
- X(f) is the Fourier transform of x[n]
- Y(f) is the Fourier transform of y[n]
- $\overline{X(f)}$ denotes the complex conjugate of X(f)

Thus, the steps are:
1. Perform FFT on both signals.
2. Multiply the FFT of y with the complex conjugate of FFT of x.
3. Apply inverse FFT to get the cross-correlation sequence.

This reduces the complexity from $O(n^2)$ to $O(n \log n)$.

---

## 3. Implementation Example

```python
import numpy as np
import matplotlib.pyplot as plt

# Example signals
x = np.array([1, 2, 3, 4, 2, 1])
y = np.array([2, 1, 0, 1, 2, 3, 4, 2, 1, 0])

# Pad x to the length of y
x_padded = np.pad(x, (0, len(y) - len(x)))

# FFT of both signals
X = np.fft.fft(x_padded)
Y = np.fft.fft(y)

# Cross-correlation using FFT
corr = np.fft.ifft(np.conj(X) * Y).real

# Then plot the cross-correlation...
```

This code uses **FFT** to accelerate the computation of cross-correlation between two one-dimensional signals x and y.
 The goal is to find **the position where x best matches within y** by sliding and measuring similarity at each lag.

In this code:

- x=[1,2,3,4,2,1] is the **template signal** we want to match.
- y=[2,1,0,1,2,3,4,2,1,0] is the **target signal** containing x.
- ![Figure_2](/Users/ruiyuhan/Desktop/Figure_2.png)

## Summary

Cross-correlation via FFT is an efficient method for pattern detection, template matching, and similarity measurement between signals and images. It reduces computational time dramatically, especially important for large datasets. 

---



# Feature Extraction Using FFT

## 1. Introduction

In machine learning and signal processing tasks, extracting meaningful features from raw data is crucial for classification, clustering, and analysis.

FFT is a powerful tool for feature extraction because it converts data from the time or spatial domain into the frequency domain, where important patterns, periodicities, and structures can become more apparent.

---

## 2. Basic Principle

### 2.1 Principle that Feature Extraction

- In the time or spatial domain, patterns might be hidden and hard to distinguish.
- After applying FFT, significant components (e.g., dominant frequencies, energy distribution) become visible.
- Frequency-domain features often capture structural or periodic behaviors that are useful for tasks like speech recognition, fault detection, or image classification.

### 2.2 How to Extract Features Using FFT

The general process:
1. **Apply FFT** to the raw signal or image.
2. **Compute the magnitude spectrum** |F(f)| to remove phase information.
3. **Extract features** such as:
   - Energy in specific frequency bands
   - Peak frequencies
   - Overall spectral shape (e.g., mean, variance)
4. **Normalize** features if necessary.
5. **Use features** as input to machine learning models.

---

## 3. Implement Example

```python
import numpy as np
import matplotlib.pyplot as plt

# Example 1D signal (could be audio, sensor data, etc.)
signal = np.sin(2*np.pi*5*np.linspace(0, 1, 128)) + 0.5*np.random.randn(128)

# FFT and magnitude spectrum
fft_values = np.fft.fft(signal)
magnitude = np.abs(fft_values)

# Use first half (symmetric for real signals)
magnitude = magnitude[:len(magnitude)//2]

# Extract simple features
mean_freq = np.mean(magnitude)
max_freq = np.max(magnitude)
# Then Plot...
```

---

We use FFT to transform a signal from the time domain into the frequency domain, making important patterns and periodic structures more visible. After the transformation, simple features like the mean and maximum magnitude of the frequency components can be extracted easily, helping to summarize the signal's behavior for further analysis.

![Figure_3](/Users/ruiyuhan/Desktop/Figure_3.png)

## Summary

FFT-based feature extraction helps reveal underlying frequency characteristics that are often crucial for accurate classification and analysis. By capturing the energy and distribution across frequency bands, FFT transforms raw data into a more informative representation for machine learning models.
