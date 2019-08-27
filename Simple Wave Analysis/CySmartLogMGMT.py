import os
import os.path
from functools import reduce

Datadir_path = "./data/"
Target_dir_path = "CysmartLogData"
Child_path = "Processed_data/"
Datafile_path = Datadir_path + Target_dir_path + "/"
data_package_eldID = 0
processed_file = ""

def Save_log(argv, argv2, mode = 0):
    # print("Save_log = " + argv)
    global processed_file
    if(mode == 0):  # Write data into file.
        # print("Info: Write data to file.")
        processed_file.write(argv + '\n')
    elif(mode == 1):    # Check duplicate.
        # print("Info: Check duplicate and open file.")
        target_file = Datafile_path + Child_path + 'Fixed_' + argv2
        print(target_file)
        if os.path.exists(target_file):
            # Remove method.
            os.remove(target_file)
            # os.unlink(target_file)
        else:
            print("Warning: No such file:%s" % target_file)
        # File open.
        processed_file = open(target_file, mode='a+', encoding='utf-8')
    elif(mode == 2):    # File close.
        # print("Info: Close file.")
        processed_file.close()

def Check_file(argv, argv2):
    # print("Check file = " + argv)
    global data_package_eldID
    # Set a log file.
    Save_log(0, argv2, 1)
    with open(argv, mode='r', encoding='utf-8') as f2read:
        # Read each line by a while process.
        line = f2read.readline()
        #print(line)
        while line:
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
                    # print(str(data_package_eldID) + "vs" + str(data_ID))
                    if(data_ID != data_package_eldID):
                        data_package_eldID = data_ID
                        # print(str(data_package_eldID) + "vs" + str(data_ID))
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
                                # print(data_output)
                                Save_log(data_output, argv2, 0)
                                data_output = ''
                                data_raw_axis = 0
                    else:
                        print("Warning: Data ID (" + str(data_ID) + ") duplicate.")
            #print(line)
            # Acquire each line.
            line = f2read.readline()
    # Close log file.
    Save_log(0, 0, 2)

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
        for dirname in dirnames:
            # print("Directory: " + dirname + ">>>")
            for filename in filenames:
                # print(filename)
                tmp_file_name = os.path.join(parent, filename)
                # Check file duplicate or not?
                # Check_logduplicate(tmp_file_name)
                # print(tmp_file_name)
                Readfile(tmp_file_name, filename)

if __name__ == "__main__":
    Data_mgmt()
    print("The end")
