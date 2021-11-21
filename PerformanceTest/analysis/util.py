import numpy as np
import pandas as pd
import statistics
import matplotlib.pyplot as plt
import os
import glob


pd.set_option('display.max_rows', None)


def load_data(dir):
    
    files = glob.glob(f"{dir}/*.csv")
    csv_files = []

    print("Loaded test cases: ")
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



def bar_chart(data_frame, xlabel = "", ylabel = "", ymax=None):
    
    bar = data_frame.plot(kind='bar', figsize=(16,8))
    plt.xticks(rotation = 0)
    plt.xlabel(xlabel,fontsize=12)
    plt.ylabel(ylabel,fontsize=12)
    if ymax is not None:
        plt.ylim([0, ymax])
    plt.xticks(fontsize=11)
    plt.yticks(fontsize=11)
    bar.spines['top'].set_visible(False)
    bar.spines['right'].set_visible(False)
    bar.spines['bottom'].set_linewidth(0.5)
    bar.spines['left'].set_visible(True)
    plt.show()


def graph_chart(data_frame, title="", xlabel="", ylabel="", logscale=False):
    graph = data_frame.plot(figsize=(12,5))
    plt.xlabel(xlabel,fontsize=12)
    plt.ylabel(ylabel,fontsize=12)
    if logscale:
        plt.yscale('log')
    plt.xticks(fontsize=11)
    plt.yticks(fontsize=11)
    plt.title(title, fontsize=18)
    plt.show()


def visited_nodes_at_depth(data_frame):
    for case in range(7):
        caseData = data_frame.loc[data_frame["case"] == case]
        graph_chart(caseData.pivot('depth', 'session', 'nodes'), title=f"Visited nodes for search at depth for case {case}", xlabel="Depth", ylabel="Nodes", logscale=True)
        display(caseData.pivot('session','depth','nodes'))

