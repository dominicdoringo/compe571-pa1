# COMPE 571 – Programming Assignment 1

**Student:** Dominic Hickman  
**Course:** COMPE 571 – Embedded Operating Systems  
**Assignment:** Programming Assignment 1  

---

## Files and Cases

- **Case 1: Baseline**
  - `baseline_seq.c`  
  - Sequential implementation of the workload with a simple loop.

- **Case 2: Multithreading**
  - `Multithreading.c`  
  - Uses the pthread library to divide the workload among multiple threads.

- **Case 3: Multitasking**
  - **Option 1 (fork):** `forkMultitasking.c`  
    - Uses `fork()`, `pipe()`, and `waitpid()` to create child processes and collect partial sums.  
  - **Option 2 (popen):**  
    - `popopenMultitasking.c` – main multitasking program.  
    - `RangeWorker.c` – helper executable that sums a subrange, called by the main program with `popen()`.  

---

## Compilation

All files can be compiled with `gcc` in a UNIX-like environment (Ubuntu/WSL, macOS, etc.).

```bash
# Case 1
gcc -O2 -Wall -Wextra -o baseline_seq baseline_seq.c

# Case 2
gcc -O2 -Wall -Wextra -pthread -o Multithreading Multithreading.c

# Case 3 Option 1
gcc -O2 -Wall -Wextra -o forkMultitasking forkMultitasking.c

# Case 3 Option 2
gcc -O2 -Wall -Wextra -o RangeWorker RangeWorker.c
gcc -O2 -Wall -Wextra -o popopenMultitasking popopenMultitasking.c
