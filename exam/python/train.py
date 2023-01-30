import pandas as pd

city_names = pd.Series(["San Francisco", "San Jose", "Sacramento"])
population = pd.Series([852469, 1015785, 485199])
city_df = pd.DataFrame({"City name": city_names, "Population": population})

# Naming rows
world_cup = pd.DataFrame(
    {
        "Home": ["Israel", "Israel", "Moldova"],
        "Away": ["Denmark", "Scotland", "Israel"],
        "Result": ["0-2", "1-1", "1-4"],
    },
    index=["match 1", "match 2", "match 3"],
)
# print(world_cup["Result"]["match 1"])

# Import from csv
houses = pd.read_csv("https://download.mlcc.google.com/mledu-datasets/california_housing_train.csv")
# print(houses.shape)
# print(houses.head(8))
# print(houses.describe())
covid = pd.read_csv("covid.csv.zip")
# print(covid.sample(5))
# print(covid[covid["country"]=="Israel"].head(5))

# Accessing data
# print(covid.country) // covid['country']