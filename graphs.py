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

        if file_name == "bytes.csv":
            csv = csv.set_index("length")

            csv.plot(title=f"Bytes used")
            plt.grid(axis="y")
            plt.xlabel("original string length")
            plt.ylabel("bytes")
            plt.savefig(f"{GRAPHS_DIR}bytes.png", dpi=300)
            plt.close()

            print(f"saved bytes graph")
        else:
            dataset = file_name.replace(".csv", "")
            average_times: DataFrame = csv.groupby(["length"]).mean().map(lambda x: x / 1000)

            average_times.plot(title=f"Average times {dataset.replace("_", " ")}", lw=1)
            plt.grid(axis="y")
            plt.ylabel("microseconds")
            plt.xlabel("original string length")
            plt.savefig(f"{GRAPHS_DIR}{dataset}.png", dpi=300)
            plt.close()

            print(f"saved {dataset} graphs")


if __name__ == "__main__":
    main()
