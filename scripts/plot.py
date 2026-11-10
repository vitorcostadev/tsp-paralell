from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


OUTPUT_DIR = Path(__file__).resolve().parent.parent / "output"


def plot_route(
    cities: pd.DataFrame,
    distances: pd.DataFrame,
    route: pd.DataFrame,
    title: str,
    output_path: Path,
) -> None:
    fig, ax = plt.subplots(figsize=(12, 9))

    for _, edge in distances.iterrows():
        city_a = cities.loc[cities["city"] == edge["city_a"]].iloc[0]
        city_b = cities.loc[cities["city"] == edge["city_b"]].iloc[0]

        ax.plot(
            [city_a["x"], city_b["x"]],
            [city_a["y"], city_b["y"]],
            color="gray",
            alpha=0.45,
            linewidth=1,
            zorder=1,
        )

        midpoint_x = (city_a["x"] + city_b["x"]) / 2
        midpoint_y = (city_a["y"] + city_b["y"]) / 2
        ax.annotate(
            f'{edge["distance"]:.1f}',
            (midpoint_x, midpoint_y),
            fontsize=7,
            color="gray",
            alpha=0.8,
        )

    route_cities = (
        route.sort_values("order")
        .merge(cities, on="city", how="left")
        .reset_index(drop=True)
    )
    closed_route = pd.concat(
        [route_cities, route_cities.iloc[[0]]],
        ignore_index=True,
    )

    ax.plot(
        closed_route["x"],
        closed_route["y"],
        color="red",
        linewidth=2.8,
        marker="o",
        markersize=5,
        label="Optimal route",
        zorder=2,
    )

    for start, end in zip(
        closed_route.iloc[:-1].itertuples(),
        closed_route.iloc[1:].itertuples(),
    ):
        ax.annotate(
            "",
            xy=(end.x, end.y),
            xytext=(start.x, start.y),
            arrowprops={
                "arrowstyle": "->",
                "color": "red",
                "linewidth": 1.5,
                "alpha": 0.9,
                "shrinkA": 10,
                "shrinkB": 10,
            },
            zorder=5,
        )

    ax.scatter(
        cities["x"],
        cities["y"],
        s=100,
        color="royalblue",
        edgecolors="black",
        zorder=3,
    )

    initial_city = cities.loc[cities["city"] == 1].iloc[0]
    ax.scatter(
        initial_city["x"],
        initial_city["y"],
        s=100,
        color="orange",
        edgecolors="black",
        label="Initial city",
        zorder=4,
    )

    for _, city in cities.iterrows():
        ax.annotate(
            f'City {int(city["city"])}',
            (city["x"], city["y"]),
            xytext=(7, 7),
            textcoords="offset points",
            fontsize=9,
        )

    ax.set_title(title)
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.grid(True, alpha=0.3)
    ax.legend()
    ax.axis("equal")

    fig.tight_layout()
    fig.savefig(output_path, dpi=200)
    plt.close(fig)


def main() -> None:
    cities = pd.read_csv(OUTPUT_DIR / "cities.csv")
    distances = pd.read_csv(OUTPUT_DIR / "distances.csv")

    plot_route(
        cities,
        distances,
        pd.read_csv(OUTPUT_DIR / "sequential_route.csv"),
        "Traveling Salesman Problem",
        OUTPUT_DIR / "sequential_route.png",
    )
    
    plot_route(
        cities,
        distances,
        pd.read_csv(OUTPUT_DIR / "parallel_route.csv"),
        "Traveling Salesman Problem",
        OUTPUT_DIR / "parallel_route.png",
    )


if __name__ == "__main__":
    main()
