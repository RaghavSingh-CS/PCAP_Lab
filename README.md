# PCAP Lab

Programs completed as part of the **Parallel Computing and Architecture Programming (PCAP)** laboratory.

## Repository Structure

```text
PCAP/
├── W1/
├── W2/
├── W3/
└── README.md
```

* `W1/` – Week 1 programs and lab sheet
* `W2/` – Week 2 OpenMP programs and lab sheet
* `W3/` – Week 3 OpenMP programs on data sharing, loop scheduling, and synchronization

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

## Compilation

Compile any OpenMP C program using:

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

On Windows, the equivalent environment-variable syntax depends on the shell being used.

## Notes

* Programs are written in **C** using **OpenMP**.
* Execution time is measured using `omp_get_wtime()`.
* Serial and parallel execution are compared wherever applicable.
* Compiled executable files are excluded using `.gitignore`.


## Week 4

Contents:

* Main Programs

  * `w4q1.c` – Compute `pow(x, rank)` for each MPI process
  * `w4q2.c` – Toggle a string character based on process rank
  * `w4q3.c` – Factorial for even ranks and Fibonacci for odd ranks
  * `w4q4.c` – Toggle a word using synchronous send (`MPI_Ssend`)
  * `w4q5.c` – Master-to-slave communication using standard send (`MPI_Send`)
  * `w4q6.c` – Circular/ring communication with value increment
  * `w4q7.c` – Array distribution using buffered send (`MPI_Bsend`)

* Additional Programs

  * `W4_Additional/w4a1.c` – Reverse digits of array elements using multiple processes
  * `W4_Additional/w4a2.c` – Check array elements for prime numbers using multiple processes

* Lab instructions (`Lab 4_NE.pdf`)

Topics covered:

* MPI process initialization and termination
* Process rank and communicator size
* Point-to-point communication
* Standard send and receive
* Synchronous send
* Buffered send
* Message tags and communicators
* Ring communication

