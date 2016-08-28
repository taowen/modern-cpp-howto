import unittest


class MyStruct:
    def __init__(self):
        self.field = None


class Test(unittest.TestCase):
    def test_pass_single_value(self):
        local_variable = 1
        print(id(local_variable))
        modify_single_value(local_variable)
        print(local_variable)  # 1

    def test_pass_list_value(self):
        local_variable = [1, 1]
        modify_list_value(local_variable)
        print(local_variable)  # [1, 2]

    def test_pass_struct_value(self):
        local_variable = MyStruct()
        local_variable.field = 1
        modify_struct_value(local_variable)
        print(local_variable.field)  # 2


def modify_single_value(single):
    print(id(single))
    single = 2


def modify_list_value(list):
    list[1] = 2


def modify_struct_value(struct):
    struct.field = 2
