# Competitive_prog

This repository contains implementations of various concurrency primitives and data structures, primarily in C and Java, organized into laboratories (`lab3`, `lab5`). These implementations are designed for competitive programming and learning concurrency concepts.

## Repository Structure

The repository is organized into lab directories, each containing specific exercises or projects related to concurrency.

### `lab3`

This directory contains C implementations of:

*   **Blocking Queues**: Implementations using semaphores (`sem_blocking_queue.c`) and condition variables (`cond_blocking_queue.c`).
*   **Bounded Buffers**: A basic implementation of a bounded buffer (`bounded_buffer.c`).
*   **Utilities**: Helper files (`utils.c`, `utils.h`) and a main program to test blocking queues (`main_blocking_queue.c`).
*   **Test Files**: Several `test-XX.txt` files for validating implementations.

### `lab5`

This directory contains C and Java implementations, focusing on more advanced concurrency concepts, including:

*   **Thread Pool**: Implementation of a thread pool (`thread_pool.c`, `thread_pool.h`).
*   **Executor**: A task execution system (`executor.c`, `executor.h`, `ExecutorMain.java`, `ExecutorRunnable.java`).
*   **Future**: Implementation of the Future concept for managing asynchronous results (`future.c`, `future.h`).
*   **Scenarios and Tasks**: Files for defining scenarios and tasks (`scenario.c`, `scenario.h`, `task.c`, `task.h`).
*   **Blocking Queues and Bounded Buffers**: Implementations similar to `lab3` but potentially adapted or extended for `lab5`.
*   **Test Files**: Several `test-XX.txt` files for validating implementations.

## Compilation and Execution

Each lab directory contains a `Makefile` to facilitate the compilation of C programs. For Java programs, standard compilation and execution via `javac` and `java` is expected.

### Example for `lab3` (C)

To compile:

```bash
cd Competitive_prog/lab3
make
```

To execute (e.g., the main blocking queue program):

```bash
./main_blocking_queue
```

### Example for `lab5` (Java)

To compile:

```bash
cd Competitive_prog/lab5
javac *.java
```

To execute (e.g., the main executor program):

```bash
java ExecutorMain
```

## Contribution

Contributions are welcome. Please follow established coding practices and submit pull requests for any improvements or bug fixes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details (if present).

