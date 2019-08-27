import numpy as np
import time
filename = "vlog-002[May.30th,2019].txt"
filename1 = "vlog_mag-002[May.30th,2019].txt"
filename2 = "vlog_acc-002[May.30th,2019].txt"
datapath = "./data/"
str_compass = "Compass"
pos = []
Efield = []

def readfile():
    with open(datapath + filename, mode='r', encoding='utf-8') as f2read:
        line = f2read.readline()
        print(line)
        print(type(line))
        file_com = open(datapath + filename1, mode='a+', encoding='utf-8')
        file_acc = open(datapath + filename2, mode='a+', encoding='utf-8')
        while line:
            if(line[: 7] == str_compass):
                file_com.write(line[8: ])
            else:
                file_acc.write(line)
            line = f2read.readline()
            #time.sleep(0.01)
        f2read.close()
        file_com.close()
        file_acc.close()
        print("All finished.")

if __name__ == "__main__":
    readfile()
