#!/usr/bin/python
import os
import time
import struct

fileZone1Prefix = "flashZone1"
fileExtension = ".bin"
fileExtensionFilter = ".c"
dataPath = "./data/C2Bin4Image_db/"
fixedPath = dataPath + "fixed/"
originalDataPath = "D:/Download/B02newUI/Stage1"
targetStructure = "static GUI_CONST_STORAGE unsigned short"
# MSB in signal byte.
BYTE_ORDER = 1
# LSB in signal byte.
# BYTE_ORDER = 0


# Common structure class.
class GUI_ConstClass:
    def __init__(self):
        self.id = -1
        self.name = {}
        self.Sdir = {}
        self.Tdir = {}
        self.len = -1

    class Struct:
        def __init__(self, fileID, fileName, fileSourceDir, fileTargetDir, fileLen):
            self.id = fileID
            self.name = fileName
            self.Sdir = fileSourceDir
            self.Tdir = fileTargetDir
            self.len = fileLen

    def InsertData(self, fileID=-1, fileName="", fileSourceDir="", fileTargetDir="", fileLen=-1):
        return self.Struct(fileID, fileName, fileSourceDir, fileTargetDir, fileLen)


class GUI_CommonClass:
    GUI_CommonInfo = []
    GUI_SourceDir = ""


def STR2HEX(arg):
    arg_int = 0
    for index in range(len(arg)):
        arg_int = arg_int * pow(16, index) + int(arg[index], base=16)
    return arg_int


# argA source file structure.
# argB source RAW data.
def GUI_ConstMGMT(argA, argB):
    print("GUI_ConstMGMT:" + argA)
    print(argB.__len__())
    if argB.__len__() > 0:
        # Create a bin file and named as source file plus bin under fix folder.
        if argA[len(argA) - 2:] == fileExtensionFilter:
            with open(fixedPath + argA[:len(argA) - 2] + fileExtension, 'wb')as bFile:
                for cInx in range(argB.__len__()):
                    line = argB[cInx]
                    # print(line)
                    str_temp = line.split("0x")
                    for cEle in str_temp:
                        cEle = cEle.strip(" ")
                        cEle = cEle.strip(",")
                        # print(cEle)
                        # print(len(cEle))
                        # print("It is alpha or number : ", cEle.)
                        if len(cEle) > 0:
                            for cAtom in range(0, len(cEle), 2):
                                # Exit current filed.
                                if cEle[cAtom] == "," or cEle[cAtom] == "\n":
                                    break
                                # Combines high and low bits into binary fields. MSB now.
                                else:
                                    if BYTE_ORDER:
                                        cHexAtom = cEle[cAtom] + cEle[cAtom + 1]
                                    else:
                                        cHexAtom = cEle[cAtom + 1] + cEle[cAtom]
                                    cHexAtom = STR2HEX(cHexAtom)
                                    # print(str(cHexAtom))
                                    content = struct.pack('B', cHexAtom)
                                    bFile.write(content)
            bFile.close()
            return 0
        else:
            print("There is a error in source file")
            return -2
    else:
        return -1


def LoadData(DataStruct):
    print("Load data file in folder.")
    GUI_FileClass = GUI_ConstClass()
    # Case I.
    for fInx in range(DataStruct.GUI_CommonInfo.__len__()):
        print("File ID:", fInx)
        dataLock = 0
        rawData = []
        with open(DataStruct.GUI_CommonInfo[fInx].Sdir, mode='r', encoding='utf-8') as f2read:
            line = f2read.readline()
            while line:
                # print(line)
                # Only check one targetStructure for sure.
                if dataLock != 1:
                    for element_index in range(len(line)):
                        if line[element_index:element_index + len(targetStructure)] == targetStructure:
                            dataLock = 1
                else:
                    if line[:2] != "};":
                        # The end of file.
                        rawData.append(line)
                    else:
                        # Manager GUI_Const data.
                        DataStruct.GUI_CommonInfo[fInx].id = fInx
                        DataStruct.GUI_CommonInfo[fInx].len = len(rawData)
                        # Manager RAW data for a file.
                        if GUI_ConstMGMT(DataStruct.GUI_CommonInfo[fInx].name, rawData) < 0:
                            print("GUI_ConstMGMT failed.")
                        # Release a lock.
                        dataLock = 0
                        # Close this file.
                        f2read.close()
                        break
                # Acquire next line.
                line = f2read.readline()

    # list_test = []
    # with open(targetPath + 'hexBin.bin', 'wb')as bFile:
    #     # Leave for testing.
    #     for cInx in range(256):
    #         list_test.append(cInx)
    #
    #     for cInx in range(len(list_test)):
    #         if cInx != "\n":
    #             # print(type(cInx))
    #             content = struct.pack('B', list_test[cInx])
    #             bFile.write(content)
    #         # print(list_test[cInx])


def Check_dir(FileInfo):
    print("Check director.")
    # this folder is custom
    for parent, dirnames, filenames in os.walk(FileInfo.GUI_SourceDir):
        # # case 1:
        # for dirname in dirnames:
        #     print("parent folder is:" + parent)
        #     print("dirname is:" + dirname)
        # case 2
        for filename in filenames:
            if filename[len(filename) - len(fileExtensionFilter):] == fileExtensionFilter:
                # fileID, fileName, fileSourceDir, fileTargetDir, fileLen
                goodFile = GUI_ConstClass().InsertData()
                print("parent folder is:" + parent)
                # print("filename with full path:" + os.path.join(parent, filename))
                # Check file path.
                goodFile.Sdir = os.path.join(parent, filename)
                # Load file name.
                goodFile.name = filename
                # Append goodFile.
                FileInfo.GUI_CommonInfo.append(goodFile)

    if FileInfo.GUI_CommonInfo.__len__() > 0:
        return 0
    else:
        return -1


if __name__ == "__main__":
    guiInfoStruct = GUI_CommonClass()
    guiInfoStruct.GUI_SourceDir = originalDataPath

    print("Processing...")
    ret = Check_dir(guiInfoStruct)
    if ret != 0:
        print("Check_dir error.")
    LoadData(guiInfoStruct)
    print("Done.")
