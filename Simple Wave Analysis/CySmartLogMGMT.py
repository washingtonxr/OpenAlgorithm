import os
import os.path
from functools import reduce

Datadir_path = "./data/"
Target_dir_path = "CysmartLogData"
Child_path = "Processed_data/"
Datafile_path = Datadir_path + Target_dir_path + "/"

def Save_log(argv, argv2):
    # print("Save_log = " + argv)
    processed_file = open(Datafile_path + Child_path + 'Fixed_' + argv2, mode='a+',
                          encoding='utf-8')
    processed_file.write(argv + '\n')
    processed_file.close()

def Check_file(argv, argv2):
    # print("Check file = " + argv)
    with open(argv, mode='r', encoding='utf-8') as f2read:
        # Read each line by a while process.
        line = f2read.readline()
        #print(line)
        while line:
            #print(line)
            # Acquire each line.
            line = f2read.readline()
            fix_line_loc = 0
            for fix_line in line:
                fix_line_loc = fix_line_loc + 1

                # Just define fix_line_loc and len(line) manual.
                if(fix_line == "[" and fix_line_loc > 100 and len(line) > 300):
                    #print(fix_line)
                    #print(fix_line_loc)
                    #print(line[fix_line_loc:len(line)-2])

                    # Spilt data for mess RAW data block.
                    data_line = line[fix_line_loc:len(line)-2]
                    # print(data_line)

                    # Get data's ID number.
                    data_ID = eval('0x' + line[len(line)-12:len(line)-10])
                    # print("Data ID = " + str(data_ID))

                    # Get number of data exist in FIFO.
                    data_depth = int('0x' + line[len(line)-9:len(line)-7], 16)
                    # print("FIFO depth = " + str(data_depth))

                    # data_part_index = 0
                    data_raw_axis = 0
                    data_output = ""
                    for data_part_index in range(int(data_depth/2)):
                        # print(data_part_index)
                        # print(data_line[6*data_part_index + 1] + data_line[6*data_part_index + 2])
                        data_part_temp = data_line[6*data_part_index + 4] + data_line[6*data_part_index + 5] + \
                                         data_line[6*data_part_index + 1] + data_line[6*data_part_index + 2]
                        data_part_temp2 = int('0x'+data_part_temp, 16)
                        if(data_part_temp2 > 16384*2):
                            data_part_temp2 = 0xffff - data_part_temp2 + 1
                            data_part_temp2 = -(data_part_temp2)
                        data_part_temp2 = data_part_temp2*9.8/16384
                        # print(data_part_temp2)
                        data_output = data_output + '\t' + str(data_part_temp2)
                        data_raw_axis = data_raw_axis + 1
                        if(data_raw_axis >= 3):
                            data_output = 'Info: ACC:' + str(data_output)
                            print(data_output)
                            # Save_log(data_output, argv2)
                            data_output = ''
                            data_raw_axis = 0

def Readfile(argv, argv2):
    # print("Read file = " + argv + argv2)
    # Check suffix of file.
    Prifix_dir = argv[len(argv) - 3:len(argv)]
    #print(Prifix_dir)

    # Filter suffix file type of 'txt'.
    if(Prifix_dir == "txt"):
        Check_file(argv, argv2)
    else:
        print("No valid file type in this folder")

def Data_mgmt():
    print("Data file management")
    for parent, dirnames, filenames in os.walk(Datafile_path):
        for filename in filenames:

            # print(filename)
            tmp_file_name = os.path.join(parent, filename)

            #print(tmp_file_name)
            Readfile(tmp_file_name, filename)

if __name__ == "__main__":
    Data_mgmt()
    print("The end")
