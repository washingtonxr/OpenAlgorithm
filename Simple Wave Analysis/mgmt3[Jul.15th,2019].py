import numpy as np
import time
filePrefix = "vlog"
fileConnector = "-"
fileID = "001"
fileDate = "[Jun.27th,2019]"
fileType = ".txt"
filename = filePrefix + fileConnector + fileID + fileDate + fileType
filename1 = filePrefix + "_mag" + fileConnector + fileID + fileDate + fileType
filename2 = filePrefix + "_acc" + fileConnector + fileID + fileDate + fileType
datapath = "./data/Pedometer_test1[Jul.15th,2019]/"
str_compass = "Info: MAG:"
str_accelerator = "Info: ACC:"
str_indentation = 10
str_prefix = 0 #15
pos = []
Efield = []

def readfile():
    with open(datapath + filename, mode='r', encoding='utf-8') as f2read:
        line = f2read.readline()
        #print(line)
        #print(type(line))
        file_com = open(datapath + filename1, mode='a+', encoding='utf-8')
        file_acc = open(datapath + filename2, mode='a+', encoding='utf-8')
        #while x in ls:
        while line:
            if(line[str_prefix: str_prefix + str_indentation] == str_compass):
                file_com.write(line[str_prefix + str_indentation:])
                #print(line[str_prefix + str_indentation:])
            elif(line[str_prefix: str_prefix + str_indentation] == str_accelerator):
                file_acc.write(line[str_prefix + str_indentation:])
                #print(line[str_prefix + str_indentation:])
            else:
                print("Error: Read line is invalid :" + line)
            line = f2read.readline()
            #time.sleep(0.01)
        f2read.close()
        file_com.close()
        file_acc.close()
        print("All finished.")

if __name__ == "__main__":
    readfile()
