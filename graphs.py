import matplotlib.pyplot as plt
from os import listdir, mkdir, path
from pandas import read_csv, DataFrame
from shutil import rmtree

DATA_DIR = "data/"
GRAPHS_DIR = "graphs/"


def main() -> None:
    if not path.exists(DATA_DIR):
        print("run C++ program first")
        exit(1)

    if path.exists(GRAPHS_DIR):
        rmtree(GRAPHS_DIR)

    mkdir(GRAPHS_DIR)

    for file_name in listdir(DATA_DIR):
        csv = read_csv(DATA_DIR + file_name, delimiter=",")
        dataset, *subset = file_name.replace(".csv", "").split("_")

        if dataset == "bits":
            csv = csv.set_index("length").map(lambda x: x / 8)

            csv.plot(title=f"Bytes used ({" ".join(subset)})")
            plt.grid(axis="y")
            plt.ylabel("bytes")
            plt.savefig(f"{GRAPHS_DIR}bytes_{"_".join(subset)}.png", dpi=300)
            plt.close()

            print(f"saved bytes {" ".join(subset)} graph")
        else:
            average_times: DataFrame = csv.groupby(["length"]).mean()
            a, b = ("encode", "decode") if dataset == "huffman" else ("compress", "decompress")

            average_times[a].plot(title=f"Average {a} time for {dataset} ({" ".join(subset)})")
            plt.grid(axis="y")
            plt.ylabel("nanoseconds")
            plt.savefig(f"{GRAPHS_DIR}times_{dataset}_{"_".join(subset)}.png", dpi=300)
            plt.close()

            average_times[b].plot(title=f"Average {b} time for {dataset} ({" ".join(subset)})")
            plt.grid(axis="y")
            plt.ylabel("nanoseconds")
            plt.savefig(f"{GRAPHS_DIR}times_{dataset}_{"_".join(subset)}.png", dpi=300)
            plt.close()

            print(f"saved {dataset} times for {" ".join(subset)} graphs")


if __name__ == "__main__":
    main()
