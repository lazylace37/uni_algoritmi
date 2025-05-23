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
        "text.usetex": True,
        "pgf.rcfonts": False,
    }
)

N_PROC = 1  # multiprocessing.cpu_count() // 2
N_RUNS = 4

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

    plt.figure(figsize=(8, 4))
    plt.plot(x, quick_sort, marker="s", ms=2, label="Quick Sort")
    plt.fill_between(
        x,
        [t - s for t, s in zip(quick_sort, quick_sort_stddev)],
        [t + s for t, s in zip(quick_sort, quick_sort_stddev)],
        alpha=0.2,
    )
    plt.plot(x, counting_sort, marker="o", ms=2, label="Counting Sort")
    plt.fill_between(
        x,
        [t - s for t, s in zip(counting_sort, counting_sort_stddev)],
        [t + s for t, s in zip(counting_sort, counting_sort_stddev)],
        alpha=0.2,
    )
    plt.plot(x, quick_sort_3_way, marker="x", ms=2, label="Quick Sort 3-Way")
    plt.fill_between(
        x,
        [t - s for t, s in zip(quick_sort_3_way, quick_sort_3_way_stddev)],
        [t + s for t, s in zip(quick_sort_3_way, quick_sort_3_way_stddev)],
        alpha=0.2,
    )
    plt.plot(x, heap_sort, marker="^", ms=2, label="Heap Sort")
    plt.fill_between(
        x,
        [t - s for t, s in zip(heap_sort, heap_sort_stddev)],
        [t + s for t, s in zip(heap_sort, heap_sort_stddev)],
        alpha=0.2,
    )
    return plt


def main():
    ## Grafico in funzione di n
    with Manager() as manager:
        m = 100_000

        A = 100
        B = (20_000_000 / A) ** (1 / 99)
        n = [int(A * B**i) for i in range(0, 100)]

        res_map = manager.dict()
        with Pool(processes=N_PROC) as pool:
            pool.map(partial(run_experiment, m=m, res=res_map), n)
        times = sorted(res_map.items())

        plt = setup_chart(times)
        plt.xlabel("Input Size (n)")
        plt.ylabel("Execution Time (seconds)")
        plt.legend()
        plt.grid(True, which="both", linestyle="--")
        plt.subplots_adjust(left=0.1, right=0.9, top=0.9, bottom=0.1)
        plt.savefig(f"output/benchmark_n_m={m}.pgf")

        plt.xscale("log")
        plt.yscale("log")
        plt.savefig(f"output/benchmark_n_m={m}_log.pgf")

    ## Grafico in funzione di m
    with Manager() as manager:
        n = 10_000

        A = 10
        B = (1_000_000 / A) ** (1 / 99)
        m = [int(A * B**i) for i in range(0, 100)]
        # m = [int(10 * 1.123324033**i) for i in range(0, 150)]

        res_map_1 = manager.dict()
        with Pool(processes=N_PROC) as pool:
            pool.map(partial(run_experiment_l, n=n, res=res_map_1), m)
        times = sorted(res_map_1.items())

        plt = setup_chart(times)
        plt.xlabel("Range (m)")
        plt.ylabel("Execution Time (seconds)")
        plt.legend()
        plt.grid(True, which="both", linestyle="--")
        plt.subplots_adjust(left=0.1, right=0.9, top=0.9, bottom=0.1)
        plt.savefig(f"output/benchmark_m_n={n}.pgf")

        plt.xscale("log")
        plt.yscale("log")
        plt.savefig(f"output/benchmark_m_n={n}_log.pgf")


if __name__ == "__main__":
    main()
