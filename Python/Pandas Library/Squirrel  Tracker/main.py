import pandas

color_counts = {
    'Red': 0,
    'Gray': 0,
    'Black': 0
}

data = pandas.read_csv("2018_Central_Park_Squirrel_Census_-_Squirrel_Data_20240407.csv")
fur_column = data['Primary Fur Color'].tolist()

for squirrel_color in fur_column:
    if squirrel_color == 'Gray':
        color_counts['Gray'] += 1
    elif squirrel_color == 'Cinnamon':
        color_counts['Red'] += 1
    elif squirrel_color == 'Black':
        color_counts['Black'] += 1



color_dict = {
    'colors': list(color_counts.keys()),
    'values': list(color_counts.values())
}

data_frame = pandas.DataFrame(color_dict)

print(data_frame)

data_frame.to_csv("squirrel_colors.csv")
