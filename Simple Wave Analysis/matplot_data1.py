import numpy as np
import matplotlib.pyplot as plt

Datadir_path = "./data/"
Datafile_path = Datadir_path + "Pedometer_test1[Jul.15th,2019]/" + "Processed_data/"
File_name = "Pedometer_testingdata_acc_JLS001[Jul.15th,2019].txt"

def LoadData(file):
    data = list()
    data_axis = [list(), list(), list(), list()]
    with open(file, 'r') as fileReader:
        lines = fileReader.readlines()  # 读取全部内容
        inx = 0
        for line in lines:
            inx += 1
            line = line.strip()
            line = line.split("\t")     # 根据数据间的分隔符切割行数据
            line = [inx, line]
            data.append(line[:])
    data = np.array(data)

    inx = 0
    time_stamp = 1/42
    for index in data:
        inx += 1
        data_axis[0].append(time_stamp*inx)
        for column in range(3):
            data_axis[column+1].append(float(index[1][column]))

    plt.figure(1)
    plt.subplot(311)
    plt.plot(data_axis[0], data_axis[1])
    plt.xlabel('Time (s)')
    plt.ylabel('Gravity (m^2/s)')
    plt.subplot(312)
    plt.plot(data_axis[0], data_axis[2])
    plt.xlabel('Time (s)')
    plt.ylabel('Gravity (m^2/s)')
    plt.subplot(313)
    plt.plot(data_axis[0], data_axis[3])
    plt.xlabel('Time (s)')
    plt.ylabel('Gravity (m^2/s)')

    plt.figure(2)
    plt.plot(data_axis[0], data_axis[1], data_axis[0], data_axis[2], data_axis[0], data_axis[3])
    plt.xlabel('Time (s)')
    plt.ylabel('Gravity (m^2/s)')
    plt.show()

if __name__ == "__main__":
    LoadData(Datafile_path + File_name)
