import numpy as np
import pandas as pd
import statistics
import matplotlib.pyplot as plt
import os
import glob
from matplotlib.ticker import ScalarFormatter


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


def remove_sessions(data: pd.DataFrame, sessions):
    for session in sessions:
        data = data.drop(data[data.session == session].index)
    return data


def bar_chart(data_frame, xlabel = "", ylabel = "", ymax=None, logscale=False):
    
    bar = data_frame.plot(kind='bar', figsize=(12,6))
    plt.xticks(rotation = 0)
    plt.xlabel(xlabel,fontsize=12)
    plt.ylabel(ylabel,fontsize=12)
    if logscale:
        plt.yscale('log')
        for axis in [bar.yaxis]:
            axis.set_major_formatter(ScalarFormatter())
    if ymax is not None:
            plt.ylim([0, ymax])
    bar.set_axisbelow(True)
    bar.yaxis.grid(color='lightgray', linestyle='-')
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)
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


def effective_branching_chart(data):

    def calculate_effective_branching(group):
        series = group['branching'] - group['cutoffs']
        series.index = group['case_name']
        return series
        # def calc_effect_branching_group(case):
        #     case = case.sort_values('depth')
        #     previous = None
        #     count = 0
        #     branching = 0
        #     for nodes in case['nodes']:
        #         if previous is None:
        #             previous = nodes
        #             continue
        #         count = count + 1
        #         branching = branching + nodes / previous
        #         previous = nodes
        #     return branching / count             
        
        # branchings = group.groupby('case_name').apply(calc_effect_branching_group)       
        # return branchings

    branchings_data = data.groupby('session').apply(calculate_effective_branching)
    bar_chart(branchings_data.transpose())
    display(branchings_data)


def cutoff_chart(data, ylabel=None):

    def calculate_cutoff(group):        
        series = group['cutoffs'] / group['branching']
        series.index = group['case_name']
        return series

    cutoff_data = data.groupby('session').apply(calculate_cutoff)
    bar_chart(cutoff_data.transpose(), ymax=1.05, ylabel=ylabel)
    display(cutoff_data)


def visited_nodes_at_depth(data_frame):
    for case in range(7):
        caseData = data_frame.loc[data_frame["case"] == case]
        graph_chart(caseData.pivot('depth', 'session', 'nodes'), title=f"Visited nodes for search at depth for case {case}", xlabel="Depth", ylabel="Nodes", logscale=True)
        display(caseData.pivot('session','depth','nodes'))

