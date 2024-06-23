import matplotlib.pyplot as plot
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
        # noinspection PyUnusedLocal
        subset = ""

        csv["length"] = csv["length"].map(lambda x: x / 1e6)

        if file_name.endswith("bytes.csv"):
            subset = "bytes"
            csv = csv.set_index("length").map(lambda x: x / 1e6)

            csv.plot(title=f"Bytes used ({dataset})")
            plot.ylabel("new size (MB)")
        else:
            subset = "_".join(file_name.replace(".csv", "").split("_")[1:])
            average_times: DataFrame = csv.groupby(["length"]).mean().map(lambda x: x / 1e6)

            average_times.plot(title=f"Average times {subset.replace("_", " ")} ({dataset})")
            plot.ylabel("milliseconds")

        plot.grid(axis="y")
        plot.xlabel("original size (MB)")
        plot.savefig(f"{GRAPHS_DIR}{dataset}_{subset}.png", dpi=300)
        plot.close()
        print(f"saved {dataset} {subset} graphs")


if __name__ == "__main__":
    main()
