# MPI Prime Numbers

The c++ program finds prime numbers between 1 and and 10000x[argument] using MPI. Sure finding prime numbers in parallel using O(n&#178;) is not nearly the most optimal way, but is a good example for usage of MPI.

# Usage

No makefile so you will have to build by hand

```sh
mpic++ main.cpp -o exec
mpiexec -np <number of processes> ./exec <input>
```
