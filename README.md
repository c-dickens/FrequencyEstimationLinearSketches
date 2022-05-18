# LinearSketches

This repository contains c++ code for _Linear Sketches for Frequency Estimation._
So far, only the _CountMinSketch_ has been implemented, although the next
step is to also include the _CountSketch_.

An overview of the CountMin sketch can be found [here](http://dimacs.rutgers.edu/~graham/pubs/papers/cmencyc.pdf).

## Problem Setting:
Let $D$ be a data stream of integers and let $f$ be the underlying 
frequency vector with every $f_i$ counting the number of occurences of $i$
in data $D$.
For example, if $D$ = {1, 1, 2, 3}$, then $f = (2, 1, 1)$.

The algorithm works by initialising $n_h$ hash functions $h_{\cdot}$ and 
$n_b$ buckets.
A table $S_{n_h \times n_b}$ is set to zero and is updated when items $x$ 
are observed:
for each hash function $i \in {1, \dots, n_h}$, we increment $S[i, h_i(x)]$.

To estimate the frequency of item $x$, we obtain the hashes $h_i(x)$ and
output $\hat{f} \min_i S[i, h_i(x)]$.
The guarantee is that:
$f_i \le \hat{f} \le f_i + \epsilon \| f \|_1$.

