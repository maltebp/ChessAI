import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import glob
import statsmodels.stats.proportion


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


def remove_tags(data: pd.DataFrame, tags):
    for tag in tags:
        data = data.drop(data[data.tag == tag].index)
    return data


def num_samples_table(data: pd.DataFrame):
    samples_df = data.groupby('tag').size().reset_index(name='samples')
    display(samples_df)


def win_ratio_chart(data):

    def collect_win_data(group):
        series = pd.Series()

        white_samples = group[group['white'] == 1]
        black_samples = group[group['white'] == 2]
        
        series['samples'] = len(group.index)
        series['wins'] = len(group[group['winner'] == 1])
        series['white_samples'] = len(white_samples.index)
        series['white_wins'] = len(white_samples[white_samples['winner'] == 1])
        series['white_draws'] = len(white_samples[white_samples['winner'] == 0])
        series['black_samples'] = len(white_samples.index)
        series['black_wins'] = len(black_samples[black_samples['winner'] == 1])
        series['black_draws'] = len(black_samples[black_samples['winner'] == 0])

        series['win_ratio'] = series['wins'] / series['samples']
        series['white_win_ratio'] = series['white_wins'] / series['white_samples']
        series['white_draw_ratio'] = series['white_draws'] / series['white_samples']
        series['black_win_ratio'] = series['black_wins'] / series['black_samples']
        series['black_draw_ratio'] = series['black_draws'] / series['white_samples']

        return series

    tag_data = data.groupby('tag').apply(collect_win_data)

    def calc_conf(row):
        def _conf(samples, wins):
            [lower, upper] = statsmodels.stats.proportion.proportion_confint(wins, samples, alpha=0.05, method='wilson')
            return (upper - lower) / 2.0
        
        intervals = [
            _conf(row['samples'], row['wins']),
            _conf(row['white_samples'], row['white_wins']),
            _conf(row['black_samples'], row['black_wins'])
        ]

        series = pd.Series(intervals, index=['win_ratio', 'white_win_ratio', 'black_win_ratio'])
        return series
    
    bar_data = tag_data[['win_ratio', 'white_win_ratio', 'black_win_ratio']]
    bar_data.columns = ['Overall', 'As white', 'As black']

    conf = tag_data.apply(calc_conf, axis='columns')
    conf.columns = ['Overall', 'As white', 'As black']

    ax = bar_data.transpose().plot.bar(yerr=conf.transpose(), capsize=4, figsize=(14,8) )
    ax.set_ylim([0, 1])
    ax.set_axisbelow(True)
    ax.yaxis.grid(color='lightgray', linestyle='-')
    plt.xticks(rotation=0, fontsize=14)
    plt.yticks(np.arange(0.0, 1.09, 0.1), fontsize=14)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['bottom'].set_linewidth(0.5)
    ax.spines['left'].set_visible(True)
    plt.show()

    draw_ratios = tag_data[['white_draw_ratio', 'black_draw_ratio']]
    draw_ratios.columns = ['As white', 'As black']

    return [bar_data, conf, draw_ratios]
    display(bar_data)
    display(conf)