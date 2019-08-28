import os
import os.path
import numpy as np
import matplotlib.pyplot as plt
import math

Datadir_path = "./data/"
Target_dir_path = "CysmartLogData"
Child_path = "Processed_data/"
Datafile_path = Datadir_path + Target_dir_path + "/"
Target_file = "123456.txt"

data = list()
#               index   x       y       z       com
data_axis = [list(), list(), list(), list(), list()]

def Show_plot():
    # print("Show_plot")
    print("Info: Show raw data.")
    plt.figure(1)
    plt.grid(True)
    # plt.subplot(311)
    plt.plot(data_axis[0], data_axis[1], 'b', label="x")
    plt.plot(data_axis[0], data_axis[2], 'r', label="y")
    plt.plot(data_axis[0], data_axis[3], label="z")
    plt.plot(data_axis[0], data_axis[4], label="com")
    plt.xlabel('time (s)')
    plt.ylabel('gravity (m^2/s)')
    plt.legend()
    plt.show()

def Check_file(argv, argv2):
    # print("Check file = " + argv)
    global data
    global data_axis

    with open(argv, mode='r', encoding='utf-8') as f2read:
        # Read each line by a while process.
        line = f2read.readline()
        index = 0
        while line:
            line = line[11:len(line) - 1]   # Take off header.
            line = line.split("\t")         # Split data from single line.
            index = index + 1               # Add line number.
            line_part = [index, line]
            data.append(line_part[:])

            # print(line)
            # Acquire each line.
            line = f2read.readline()
        data = np.array(data)
        # print(data)
        single_ts = 0
        for single_data in data:
            # print(single_data)
            single_ts = single_ts + 1
            data_axis[0].append(single_ts)
            data_com = 0
            for axis in range(3):
                # print(str(single_data[0]) + ":" + single_data[1][axis])
                data_axis[axis + 1].append(float(single_data[1][axis]))
                data_com = data_com + math.pow(float(single_data[1][axis]), 2)
            data_com = math.sqrt(data_com)
            data_axis[4].append(data_com)
        Show_plot()

def Data_mgmt():
    print("Info: Data file management.")
    for parent, dirnames, filenames in os.walk(Datafile_path):
        # print("Parent dir : %s >>>" % parent)
        # for dirname in dirnames:
        #     print("Directory: %s " % dirname)
        for filename in filenames:
            if(parent[len(parent)-(len(Child_path)-1):] == Child_path[:len(Child_path)-1]):
                if(filename[len(filename)-len(Target_file):] == Target_file):
                    print(filename)
                    tmp_file_name = os.path.join(parent, filename)
                    print(tmp_file_name)
                    Check_file(tmp_file_name, filename)

if __name__ == "__main__":
    Data_mgmt()
    print("Info: The end.")
