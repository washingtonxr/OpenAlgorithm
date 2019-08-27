import numpy as np
import time
filePrefix = "vlog"
fileConnector = "-"
fileID = "002"
fileDate = "[Jun.11th,2019]"
fileType = ".txt"
filename = filePrefix + fileConnector + fileID + fileDate + fileType
filename1 = filePrefix + "_mag" + fileConnector + fileID + fileDate + fileType
filename2 = filePrefix + "_acc" + fileConnector + fileID + fileDate + fileType
datapath = "./data/"
str_compass = "Info: MAG:"
str_accelerator = "Info: ACC:"
str_indentation = 10
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
            if(line[: str_indentation] == str_compass):
                file_com.write(line[str_indentation:])
                #print(line[str_indentation:])
            elif(line[: str_indentation] == str_accelerator):
                file_acc.write(line[str_indentation:])
                #print(line[str_indentation:])
            else:
                print("Error: Read line is invalid.")
            line = f2read.readline()
            #time.sleep(0.01)
        f2read.close()
        file_com.close()
        file_acc.close()
        print("All finished.")

if __name__ == "__main__":
    readfile()
