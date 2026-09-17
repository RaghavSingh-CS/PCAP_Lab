# PCAP Lab

Programs completed as part of the **Parallel Computing and Architecture Programming (PCAP)** laboratory.

## Repository Structure

```text
PCAP/
├── W1/
├── W2/
├── W3/
├── W4/
├── W5/
├── W6/
├── W7/
├── W8/
└── README.md
```

* `W1/` – Week 1 programs and lab sheet
* `W2/` – Week 2 OpenMP programs and lab sheet
* `W3/` – Week 3 OpenMP programs on data sharing, loop scheduling, and synchronization
* `W4/` – Week 4 MPI programs on point-to-point communication
* `W5/` – Week 5 MPI programs on collective communication
* `W6/` – Week 6 MPI programs
* `W7/` – Week 7 CUDA introduction
* `W8/` – Week 8 CUDA programs and synchronization

---

## Week 1

### Contents

* Six C programs (`w1q1.c` to `w1q6.c`)
* Lab instructions (`Lab1.pdf`)

### Topics Covered

* Parallel array initialization
* Parallel reduction
* Parallel summation
* Maximum and minimum search
* Prefix sum
* Performance comparison of serial and parallel execution

---

## Week 2

### Main Programs

* `w2q1.c` – Matrix Addition using `parallel for`
* `w2q2.c` – Matrix Transformation
* `w2q3.c` – Binary 1's Complement Transformation
* `w2q4.c` – Matrix–Vector Multiplication
* `w2q5.c` – Random Array Generation and Squaring
* `w2q6.c` – Vector Operations using `parallel sections`

### Additional Programs

* `w2a1.c` – Compute `i^x` using Thread ID
* `w2a2.c` – One-Dimensional Array Addition
* `w2a3.c` – Reverse Elements of an Array
* `w2a4.c` – Prime Numbers in an Interval

### Lab Instructions

* `Lab 2 NE.pdf`

---

## Week 3

### Main Programs

* `w3q1.c` – Data-Sharing Clauses Demonstration (`shared`, `private`, `firstprivate`, `lastprivate`)
* `w3q2.c` – OpenMP Synchronization Constructs (`critical`, `atomic`, `reduction`, `master`)
* `w3q3.c` – Prime Number Counting and Loop Scheduling Policies (`static`, `dynamic`, `guided`)
* `w3q4.c` – Parallel Merge Sort Performance Analysis

### Additional Programs

* `w3a1.c` – Multithreaded Tokenizer
* `w3a2.c` – Parallel Histogram Generation
* `w3a3.c` – Factorial Comparison of Synchronization Constructs
* `w3a4.c` – Monte Carlo Estimation of Pi

### Lab Instructions

* `Lab3_3_NE.pdf`

### Topics Covered

* OpenMP data-sharing attributes:
  * `shared`
  * `private`
  * `firstprivate`
  * `lastprivate`
  * `default`
* Synchronization constructs:
  * `critical`
  * `atomic`
  * `barrier`
  * `reduction`
* Race conditions
* Loop scheduling strategies:
  * `static`
  * `dynamic`
  * `guided`
* Workload distribution and load balancing
* Performance metrics:
  * Execution Time
  * Speedup
  * Efficiency

---

## Week 4

### Main Programs

* `w4q1.c` – Compute `pow(x, rank)` for each MPI process
* `w4q2.c` – Toggle a string character based on process rank
* `w4q3.c` – Factorial for even ranks and Fibonacci for odd ranks
* `w4q4.c` – Toggle a word using synchronous send (`MPI_Ssend`)
* `w4q5.c` – Master-to-slave communication using standard send (`MPI_Send`)
* `w4q6.c` – Circular/ring communication with value increment
* `w4q7.c` – Array distribution using buffered send (`MPI_Bsend`)

### Additional Programs

* `W4_Additional/w4a1.c` – Reverse digits of array elements using multiple processes
* `W4_Additional/w4a2.c` – Check array elements for prime numbers using multiple processes

### Lab Instructions

* `Lab 4_NE.pdf`

### Topics Covered

* MPI process initialization and termination
* Process rank and communicator size
* Point-to-point communication
* Standard send and receive
* Synchronous send
* Buffered send
* Message tags and communicators
* Ring communication

---

## Week 5

### Main Programs

* `w5q1.c` – Factorial computation using `MPI_Scatter` and `MPI_Gather`
* `w5q2.c` – Average calculation using MPI collective communication
* `w5q3.c` – Non-vowel counting in a distributed string
* `w5q4.c` – String interleaving using `MPI_Scatter` and `MPI_Gather`
* `w5q5.c` – Parallel selection sort followed by merging

### Additional Programs

* `W5_Additional/w5a1.c` – Distributed power computation
* `W5_Additional/w5a2.c` – Even/odd array transformation and counting

### Lab Instructions

* `Lab 5-NE.pdf`

### Topics Covered

* Collective communication
* Broadcasting data using `MPI_Bcast`
* Distributing data using `MPI_Scatter`
* Collecting data using `MPI_Gather`
* Reduction operations using `MPI_Reduce`
* Distributed computation
* Parallel processing of arrays and strings

---

## Week 6

### Main Programs

* `w6q1.c` to `w6q4.c` – MPI laboratory exercises

### Additional Programs

* `w6a1.c` and `w6a2.c` – Additional MPI exercises

### Lab Instructions

* `Lab 6-NE.pdf`

---

## Week 7

### Main Programs

* `w7q1.cu` to `w7q4.cu` – CUDA introduction exercises

### Additional Programs

* `W7_Additional/w7a1.cu`
* `W7_Additional/w7a2.cu`

### Lab Instructions

* `Lab 7 CUDA Introduction NE.pdf`

### Topics Covered

* CUDA programming model
* Host and device execution
* CUDA kernels
* Thread and block indexing
* Device memory allocation and data transfer
* GPU execution

---

## Week 8

### Main Programs

* `w8q1.cu` – One-dimensional convolution
* `w8q2.cu` – Parallel odd-even transposition sort
* `w8q3.cu` – Parallel rank sort
* `w8q4.cu` – Word occurrence counting using atomic operations
* `w8q5.cu` – Parallel vowel replacement

### Additional Programs

* `W8_Additional/w8a1.cu` – Parallel string expansion
* `W8_Additional/w8a2.cu` – Reverse each word in parallel
* `W8_Additional/w8a3.cu` – Parallel string repetition

### Lab Instructions

* `Lab 8 CUDA Programs and Synchronization NE.pdf`

### Topics Covered

* One-dimensional convolution
* Parallel sorting algorithms
* CUDA atomic operations
* Kernel execution timing
* Parallel string processing
* Thread synchronization

---

## Compilation

### OpenMP Programs

Compile an OpenMP C program using:

```bash
gcc -fopenmp filename.c -o output
```

For example:

```bash
gcc -fopenmp w3q1.c -o w3q1
```

Run the program with the desired number of OpenMP threads:

```bash
OMP_NUM_THREADS=4 ./w3q1
```

### MPI Programs

Compile an MPI C program using:

```bash
mpicc filename.c -o output
```

For example:

```bash
mpicc w5q1.c -o w5q1
```

Run the program with the desired number of MPI processes:

```bash
mpirun -np 4 ./w5q1
```

---

### CUDA Programs

Compile a CUDA program using:

```bash
nvcc filename.cu -o output
```

For example:

```bash
nvcc w8q1.cu -o w8q1
```

Run using:

```bash
./w8q1
```

CUDA programs require a compatible NVIDIA GPU and CUDA runtime.

---

## Notes

* Programs are written in **C** and **CUDA C** using **OpenMP**, **MPI**, and **CUDA**.
* OpenMP is used for shared-memory parallel programming.
* MPI is used for distributed-memory parallel programming.
* CUDA is used for GPU-based parallel programming.
* Execution time is measured using `omp_get_wtime()` where applicable.
* Serial and parallel execution are compared wherever applicable.
* Compiled executable files are excluded from version control.
