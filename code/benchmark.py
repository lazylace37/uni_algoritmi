import multiprocessing
import subprocess
from functools import partial
from multiprocessing import Manager, Pool

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

N_PROC = multiprocessing.cpu_count
N_RUNS = 20


def execute_bench(n: int, m: int):
    result = subprocess.run(
        ["./bench", str(n), str(m), str(N_RUNS)], capture_output=True, text=True
    )

    if result.returncode != 0:
        print(f"Error for {n}: {result.stderr.strip()}")
        return

    output = result.stdout.strip().split("\n")
    exec_times = [float(x) for x in output[1].split("\t")]
    exec_stddev = [float(x) for x in output[2].split("\t")]
    return exec_times, exec_stddev


def run_experiment(n: int, m: int, res: dict):
    res[n] = execute_bench(n, m)


def run_experiment_l(m: int, n: int, res: dict):
    res[m] = execute_bench(n, m)


def setup_chart(times_map):
    x = [n for n, _ in times_map]
    mean_times = [run[0] for _, run in times_map]
    std_devs = [run[1] for _, run in times_map]
    quick_sort, counting_sort, heap_sort = list(zip(*mean_times))
    quick_sort_stddev, counting_sort_stddev, heap_sort_stddev = list(zip(*std_devs))

    plt.figure(figsize=(10, 6))
    plt.plot(x, quick_sort, marker="s", label="Quick Sort")
    plt.fill_between(
        x,
        [t - s for t, s in zip(quick_sort, quick_sort_stddev)],
        [t + s for t, s in zip(quick_sort, quick_sort_stddev)],
        alpha=0.2,
    )
    plt.plot(x, counting_sort, marker="o", label="Counting Sort")
    plt.fill_between(
        x,
        [t - s for t, s in zip(counting_sort, counting_sort_stddev)],
        [t + s for t, s in zip(counting_sort, counting_sort_stddev)],
        alpha=0.2,
    )
    plt.plot(x, heap_sort, marker="^", label="Heap Sort")
    plt.fill_between(
        x,
        [t - s for t, s in zip(heap_sort, heap_sort_stddev)],
        [t + s for t, s in zip(heap_sort, heap_sort_stddev)],
        alpha=0.2,
    )
    return plt


def main():
    with Manager() as manager:
        ## Grafico in funzione di n
        n = [100 * 1.072267222**i for i in range(0, 100)]

        res_map = manager.dict()
        with Pool(processes=4) as pool:
            pool.map(partial(run_experiment, m=100_000, res=res_map), n)
        times_map = sorted(res_map.items())

        plt = setup_chart(times_map)
        plt.title("Sorting Algorithm Performance Comparison")
        plt.xlabel("Input Size (n)")
        plt.ylabel("Execution Time (seconds)")
        plt.legend()
        plt.grid(True, which="both", linestyle="--")
        plt.savefig("benchmark_n.png")

        ## Grafico in funzione di m
        m = [10 * 1.123324033**i for i in range(0, 100)]

        res_map_1 = manager.dict()
        with Pool(processes=4) as pool:
            pool.map(partial(run_experiment_l, n=10_000, res=res_map_1), m)
        times_map = sorted(res_map_1.items())

        plt = setup_chart(times_map)
        plt.title("Sorting Algorithm Performance Comparison")
        plt.xlabel("Range (m)")
        plt.ylabel("Execution Time (seconds)")
        plt.legend()
        plt.grid(True, which="both", linestyle="--")
        # plt.show()
        plt.savefig("benchmark_m.png")


if __name__ == "__main__":
    main()
