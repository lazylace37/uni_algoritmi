import subprocess
from functools import partial
from multiprocessing import Manager, Pool

import matplotlib.pyplot as plt


def run_experiment(n, res):
    # print(f"Running experiment for n={n}")
    result = subprocess.run(["./bench", str(n)], capture_output=True, text=True)

    if result.returncode != 0:
        print(f"Error for {n}: {result.stderr.strip()}")
        return

    output = result.stdout.strip()
    values = list(map(float, output.split("\t")))
    res[n] = values


def main():
    with Manager() as manager:
        res = manager.dict()

        sizes = [2**exp for exp in range(4, 21)]

        with Pool(processes=4) as pool:
            pool.map(partial(run_experiment, res=res), sizes)

        res = sorted(res.items())
        x = [n for n, _ in res]

        results = list(zip(*[values for _, values in res]))
        insertion_sort, merge_sort, heap_sort, quick_sort = results

        print("Insertion Sort:", insertion_sort)
        print("Merge Sort:", merge_sort)
        print("Heap Sort:", heap_sort)
        print("Quick Sort:", quick_sort)

        plt.figure(figsize=(10, 6))
        plt.plot(x, insertion_sort, marker="o", label="Insertion Sort")
        plt.plot(x, merge_sort, marker="s", label="Merge Sort")
        plt.plot(x, heap_sort, marker="^", label="Heap Sort")
        plt.plot(x, quick_sort, marker="*", label="Quick Sort")

        # plt.xscale("log")
        # plt.yscale("log")
        # plt.xlabel("Input Size (n) - log scale")
        # plt.ylabel("Execution Time (seconds) - log scale")
        plt.title("Sorting Algorithm Performance Comparison")
        plt.legend()
        plt.grid(True, which="both", linestyle="--")
        plt.show()


if __name__ == "__main__":
    main()
