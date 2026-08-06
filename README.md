# PCAP Lab

Programs completed as part of the Parallel Computing and Architecture Programming (PCAP) laboratory.

## Repository Structure

```
PCAP/
├── W1/
├── W2/
└── README.md
```

- `W1/` – Week 1 programs and lab sheet
- `W2/` – Week 2 OpenMP programs and lab sheet

## Week 1

Contents:

- Six C programs (`w1q1.c` to `w1q6.c`)
- Lab instructions (`Lab1.pdf`)

Topics covered:

- Parallel array initialization
- Parallel reduction
- Parallel summation
- Maximum and minimum search
- Prefix sum
- Performance comparison of serial and parallel execution

---

## Week 2

Contents:

- Main Programs
  - `w2q1.c` – Matrix Addition using `parallel for`
  - `w2q2.c` – Matrix Transformation
  - `w2q3.c` – Binary 1's Complement Transformation
  - `w2q4.c` – Matrix–Vector Multiplication
  - `w2q5.c` – Random Array Generation and Squaring
  - `w2q6.c` – Vector Operations using `parallel sections`

- Additional Programs
  - `w2a1.c` – Compute i^x using Thread ID
  - `w2a2.c` – One-Dimensional Array Addition
  - `w2a3.c` – Reverse Elements of an Array
  - `w2a4.c` – Prime Numbers in an Interval

- Lab instructions
  - `Lab 2 NE.pdf`

## Compilation

Compile any program using:

```bash
gcc -fopenmp filename.c -o output
```

Example:

```bash
gcc -fopenmp w2q1.c -o w2q1
```

Run using:

```bash
OMP_NUM_THREADS=4 ./w2q1
```

## Notes

- Programs are written in C using OpenMP.
- Execution time is measured using `omp_get_wtime()`.
- Serial and parallel execution are compared wherever applicable.
- Compiled executable files are excluded using `.gitignore`.
