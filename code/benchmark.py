import subprocess
from functools import partial
from multiprocessing import Manager, Pool

import matplotlib.pyplot as plt

N_RUNS = 10


def execute_bench(n: int, m: int):
    result = subprocess.run(
        ["./bench", str(n), str(m), str(N_RUNS)], capture_output=True, text=True
    )

    if result.returncode != 0:
        print(f"Error for {n}: {result.stderr.strip()}")
        return

    output = result.stdout.strip()
    output = output.split("\n")[1]  # La prima riga è l'header
    return list(map(float, output.split("\t")))


def run_experiment(n: int, m: int, res: dict):
    values = execute_bench(n, m)
    res[n] = values


def run_experiment_l(m: int, n: int, res: dict):
    values = execute_bench(n, m)
    res[m] = values


def main():
    with Manager() as manager:
        ## Grafico in funzione di n
        # n = list(range(100, 100_000, 1000))
        n = []
        for i in range(0, 100):
            n_i = 100 * 1.072267222**i
            n.append(int(n_i))

        res_map = manager.dict()
        with Pool(processes=4) as pool:
            pool.map(partial(run_experiment, m=100_000, res=res_map), n)

        times_map = sorted(res_map.items())
        x = [n for n, _ in times_map]
        results = list(zip(*[values for _, values in times_map]))
        insertion_sort, merge_sort, heap_sort, quick_sort = results

        plt.figure(figsize=(10, 6))
        plt.plot(x, insertion_sort, marker="o", label="Insertion Sort")
        plt.plot(x, merge_sort, marker="s", label="Merge Sort")
        plt.plot(x, heap_sort, marker="^", label="Heap Sort")
        plt.plot(x, quick_sort, marker="*", label="Quick Sort")
        plt.title("Sorting Algorithm Performance Comparison")
        plt.xlabel("Input Size (n)")
        plt.ylabel("Execution Time (seconds)")
        plt.legend()
        plt.grid(True, which="both", linestyle="--")
        plt.show()

        ## Grafico in funzione di m
        # m = list(range(10, 1_000_000 + 1, 10_000))
        m = []
        for i in range(0, 100):
            m_i = 10 * 1.123324033**i
            m.append(int(m_i))

        res_map_1 = manager.dict()
        with Pool(processes=4) as pool:
            pool.map(partial(run_experiment_l, n=10_000, res=res_map_1), m)

        times_map = sorted(res_map_1.items())
        x = [n for n, _ in times_map]
        results = list(zip(*[values for _, values in times_map]))
        insertion_sort, merge_sort, heap_sort, quick_sort = results

        plt.figure(figsize=(10, 6))
        plt.plot(x, insertion_sort, marker="o", label="Insertion Sort")
        plt.plot(x, merge_sort, marker="s", label="Merge Sort")
        plt.plot(x, heap_sort, marker="^", label="Heap Sort")
        plt.plot(x, quick_sort, marker="*", label="Quick Sort")
        plt.title("Sorting Algorithm Performance Comparison")
        plt.xlabel("Range (m)")
        plt.ylabel("Execution Time (seconds)")
        plt.legend()
        plt.grid(True, which="both", linestyle="--")
        plt.show()


if __name__ == "__main__":
    main()
