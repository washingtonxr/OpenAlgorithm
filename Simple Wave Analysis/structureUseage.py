#!/usr/bin/python

import sys


class MyClass():
    def __init__(self, name=""):
        self.name = name
        self.data_dic = {}
        self.index = -1

    class Struct():
        def __init__(self, contents, name, message, status, num=-1):
            self.contents = contents
            self.name = name
            self.message = message
            self.status = status
            self.line_num = num

    # return struct item.
    def make_struct(self, contents="", name="", message="", status="", num=-1):
        return self.Struct(contents, name, message, status, num)


if __name__ == "__main__":
    test_struct = []

    test_struct1 = MyClass().make_struct("hello world", "s2", "你好", "success")
    test_struct2 = MyClass().make_struct("123", "test1", "你好", "success")
    test_struct3 = MyClass().make_struct("456", "test1", "你好", "success")
    test_struct.append(test_struct1)
    test_struct.append(test_struct2)
    test_struct.append(test_struct3)
    for x in range(10):
        test = MyClass().make_struct()
        test.line_num = x
        test_struct.append(test)
    print("done")
