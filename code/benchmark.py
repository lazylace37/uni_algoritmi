import multiprocessing
import subprocess
from functools import partial
from multiprocessing import Manager, Pool
from multiprocessing.managers import DictProxy
from typing import List, Tuple

import matplotlib
import matplotlib.pyplot as plt

matplotlib.use("pgf")
matplotlib.rcParams.update(
    {
        "pgf.texsystem": "pdflatex",
        "font.family": "serif",
        "text.usetex": True,
        "pgf.rcfonts": False,
    }
)

N_PROC = multiprocessing.cpu_count() // 2
N_RUNS = 20

BenchmarkRun = Tuple[List[float], List[float]]
Benchmark = Tuple[int, BenchmarkRun]


def execute_bench(n: int, m: int) -> BenchmarkRun:
    result = subprocess.run(
        ["./bench", str(n), str(m), str(N_RUNS)], capture_output=True, text=True
    )

    if result.returncode != 0:
        print(f"Error for {n}: {result.stderr.strip()}")
        return [], []

    output = result.stdout.strip().split("\n")
    exec_times = [float(x) for x in output[1].split("\t")]
    exec_stddev = [float(x) for x in output[2].split("\t")]
    return exec_times, exec_stddev


def run_experiment(n: int, m: int, res: "DictProxy[int, BenchmarkRun]"):
    res[n] = execute_bench(n, m)


def run_experiment_l(m: int, n: int, res: "DictProxy[int, BenchmarkRun]"):
    res[m] = execute_bench(n, m)


def setup_chart_data(times: List[Benchmark]):
    xs: List[int] = []
    quick_sort_means: List[float] = []
    counting_sort_means: List[float] = []
    quick_sort_3_way_means: List[float] = []
    heap_sort_means: List[float] = []

    quick_sort_stddevs: List[float] = []
    counting_sort_stddevs: List[float] = []
    quick_sort_3_way_stddevs: List[float] = []
    heap_sort_stddevs: List[float] = []

    for n, (means, stddevs) in times:
        xs.append(n)

        quick_sort_means.append(means[0])
        counting_sort_means.append(means[1])
        quick_sort_3_way_means.append(means[2])
        heap_sort_means.append(means[3])

        quick_sort_stddevs.append(stddevs[0])
        counting_sort_stddevs.append(stddevs[1])
        quick_sort_3_way_stddevs.append(stddevs[2])
        heap_sort_stddevs.append(stddevs[3])

    return (
        xs,
        quick_sort_means,
        counting_sort_means,
        quick_sort_3_way_means,
        heap_sort_means,
        quick_sort_stddevs,
        counting_sort_stddevs,
        quick_sort_3_way_stddevs,
        heap_sort_stddevs,
    )


def setup_chart(times: list[tuple[int, tuple[list[float], list[float]]]]):
    (
        x,
        quick_sort,
        counting_sort,
        quick_sort_3_way,
        heap_sort,
        quick_sort_stddev,
        counting_sort_stddev,
        quick_sort_3_way_stddev,
        heap_sort_stddev,
    ) = setup_chart_data(times)

    plt.figure(figsize=(12, 8))
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
    plt.plot(x, quick_sort_3_way, marker="x", label="Quick Sort 3-Way")
    plt.fill_between(
        x,
        [t - s for t, s in zip(quick_sort_3_way, quick_sort_3_way_stddev)],
        [t + s for t, s in zip(quick_sort_3_way, quick_sort_3_way_stddev)],
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
        m = 100_000
        # for m in range(100_000, 1_000_000, 100_000):
        if True:
            n = [int(100 * 1.072267222**i) for i in range(0, 100)]

            res_map = manager.dict()
            with Pool(processes=N_PROC) as pool:
                pool.map(partial(run_experiment, m=m, res=res_map), n)
            times = sorted(res_map.items())

            plt = setup_chart(times)
            plt.suptitle("Sorting Algorithm Performance Comparison")
            plt.title(f"m = {m}")
            plt.xlabel("Input Size (n)")
            plt.ylabel("Execution Time (seconds)")
            plt.legend()
            plt.grid(True, which="both", linestyle="--")
            plt.savefig(f"output/benchmark_n_m={m}.pgf")

            plt.xscale("log")
            plt.yscale("log")
            plt.savefig(f"output/benchmark_n_m={m}_log.pgf")

        ## Grafico in funzione di m
        # for n in range(10_000, 100_000, 10_000):
        n = 10_000
        if True:
            m = [int(10 * 1.123324033**i) for i in range(0, 100)]

            res_map_1 = manager.dict()
            with Pool(processes=N_PROC) as pool:
                pool.map(partial(run_experiment_l, n=n, res=res_map_1), m)
            times = sorted(res_map_1.items())

            plt = setup_chart(times)
            plt.suptitle("Sorting Algorithm Performance Comparison")
            plt.title(f"n = {n}")
            plt.xlabel("Range (m)")
            plt.ylabel("Execution Time (seconds)")
            plt.legend()
            plt.grid(True, which="both", linestyle="--")
            plt.savefig(f"output/benchmark_m_n={n}.pgf")

            plt.xscale("log")
            plt.yscale("log")
            plt.savefig(f"output/benchmark_m_n={n}_log.pgf")


if __name__ == "__main__":
    main()
