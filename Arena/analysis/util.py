import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import glob


pd.set_option('display.max_rows', None)


def load_data():
    
    files = glob.glob(f"data/*.csv")
    csv_files = []

    print("Loaded game results: ")
    for f in files:
        print(f"  {f}")
        session_name = f.split('\\')[-1][:-4] # Name is between directory-slash and .csv
        csv_file = pd.read_csv(f)
        csv_file['session'] = session_name
        col = csv_file.pop('session')
        csv_file.insert(0, col.name, col)
        csv_files.append(csv_file)

    combined_data = pd.concat(csv_files);
    return combined_data.reset_index(drop=True)



def win_ratio_chart(data):
    def calc_ratio(row, ratio_type):
        matches = 0
        for winner in row.winner:
            matches += 1 if winner == ratio_type else 0
        ratio = matches / len(row.winner)
        return ratio
        
    def calc_draw_ratio(row):
        return calc_ratio(row, 0)

    def calc_win_ratio(row):
        return calc_ratio(row, 1)   

    version_data = data.groupby('tag').agg(list)

    version_data['white_win_ratio'] = data[data["white"] == 1].groupby('tag').agg(list).apply(calc_win_ratio, axis=1)
    version_data['white_draw_ratio'] = data[data["white"] == 1].groupby('tag').agg(list).apply(calc_draw_ratio, axis=1)
    version_data['black_win_ratio'] = data[data["white"] == 2].groupby('tag').agg(list).apply(calc_win_ratio, axis=1)
    version_data['black_draw_ratio'] = data[data["white"] == 2].groupby('tag').agg(list).apply(calc_draw_ratio, axis=1)

    version_data = version_data[['white_win_ratio', 'white_draw_ratio', 'black_win_ratio', 'black_draw_ratio']].transpose()

    #win_ratios = version_data.pivot('version', )

    ax = version_data.plot(kind='bar', figsize=(18,8) )
    ax.set_xticklabels(["Win ratio as white", "Draw ratio as white", "Win ratio as black", "Draw ratio as black"])
    plt.xticks(rotation = 0)
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)