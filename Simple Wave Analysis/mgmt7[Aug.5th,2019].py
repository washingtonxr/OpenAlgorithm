import os
import os.path

Datadir_path = "./data/"
Target_dir_path = "Payment_model"   #"Pedometer_test1[Aug.5th,2019]"
Target_file = "Tang_L[Jul.31th,2019].txt"   #"data[Jul.29th,2019].txt"
Datafile_path = Datadir_path + Target_dir_path + "/"
File_prefix = "Pedometer_test"
#str_compass = "Info: MAG:"
Str_accelerator_prefix = "Info:ACC: "
Append_prefix = "acc_"
Child_path = "Processed_data/"
Innerfile_prefix = 'Pedometer_testingdata_'

def Check_dir():
    # this folder is custom
    for parent, dirnames, filenames in os.walk(Datafile_path):
        # case 1:
        for dirname in dirnames:
            print("parent folder is:" + parent)
            print("dirname is:" + dirname)
        # case 2
        for filename in filenames:
            print("parent folder is:" + parent)
            print("filename with full path:" + os.path.join(parent, filename))

def Check_file(argv):
    print('Check_file:'+ argv)
    with open(argv, mode='r', encoding='utf-8') as f2read:
        line = f2read.readline()
        # file_acc = open(Datafile_path + Child_path + argv[len(Datafile_path):(len(Datafile_path) +
        #            len(Innerfile_prefix))] + Append_prefix + argv[(len(Datafile_path) + len(Innerfile_prefix)):] +
        #            '.txt', mode='a+', encoding='utf-8')
        processed_file = open(Datafile_path + Child_path + 'Fixed_' + argv[len(Datafile_path):len(argv)], mode='a+', encoding='utf-8')
        #print(line)
        while line:
            #print(line[:len(Str_accelerator_prefix)])
            #print(line[len(Str_accelerator_prefix):len(line)])
            #processed_file.write(line[len(Str_accelerator_prefix):len(line)])
            if (line[:len(Str_accelerator_prefix)] == Str_accelerator_prefix):
                #print(line[len(Str_accelerator_prefix):])
                processed_file.write(line[len(Str_accelerator_prefix):])
            line = f2read.readline()
        processed_file.close()

def Readfile(argv):
    Prifix_dir = argv[(len(argv) - 3):len(argv)]
    print(Prifix_dir)
    if(Prifix_dir == "txt"):
        Check_file(argv)
    else:
        print('No valid file type:' + Prifix_dir)

def Data_mgmt():
    # this folder is custom
    for parent, dirnames, filenames in os.walk(Datafile_path):
        for dirname in dirnames:
            #print(dirname)
            if (dirname != File_prefix):
                for filename in filenames:
                    #print("parent folder is:" + parent)
                    #print("filename with full path:" + os.path.join(parent, filename))
                    tmp_file_name = os.path.join(parent, filename)
                    #print(tmp_file_name)
                    Readfile(tmp_file_name)

if __name__ == "__main__":
    #Check_dir()
    Data_mgmt()
    print('Then end')
