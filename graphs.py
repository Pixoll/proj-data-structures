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
        dataset = file_name.split("_")[0]

        csv["length"] = csv["length"].map(lambda x: x / 1e6)

        if file_name.endswith("bytes.csv"):
            csv = csv.set_index("length").map(lambda x: x / 1e6)

            csv.plot(title=f"Bytes used ({dataset})")
            plt.grid(axis="y")
            plt.xlabel("original string size (MB)")
            plt.ylabel("new size (MB)")
            plt.savefig(f"{GRAPHS_DIR}{dataset}_bytes.png", dpi=300)
            plt.close()

            print(f"saved {dataset} bytes graph")
        else:
            _, *subset = file_name.replace(".csv", "").split("_")
            subset = "_".join(subset)
            average_times: DataFrame = csv.groupby(["length"]).mean().map(lambda x: x / 1e6)

            average_times.plot(title=f"Average times {subset.replace("_", " ")} ({dataset})")
            plt.grid(axis="y")
            plt.ylabel("milliseconds")
            plt.xlabel("original string size (MB)")
            plt.savefig(f"{GRAPHS_DIR}{dataset}_{subset}.png", dpi=300)
            plt.close()

            print(f"saved {dataset} {subset} graphs")


if __name__ == "__main__":
    main()
