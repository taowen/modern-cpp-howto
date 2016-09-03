# coding=utf-8
import copy

class File(object):
    def __eq__(self, other):
        raise Exception('transient object does not support ==')

    def __ne__(self, other):
        raise Exception('transient object does not support !=')

    def __hash__(self):
        raise Exception('transient object does not support hash')

    def __copy__(self):
        raise Exception('transient object does not support shallow copy')

    def __deepcopy__(self, memo):
        raise Exception('transient object does not support deep copy')

file1 = File()
file2 = File()
# print(file1 == file2) 会抛异常
# print(file1 != file2) 会抛异常
# some_map = {file1: None} 会抛异常
# copy.copy(file1) 会抛异常
# copy.deepcopy(file1) 会抛异常


class Order(object):
    def __init__(self, order_id, *order_lines):
        self.order_id = order_id
        self.order_lines = list(order_lines)

    def __eq__(self, other):
        return self.order_id == other.order_id

    def __ne__(self, other):
        return not (self == other)

    def __hash__(self):
        return hash(self.order_id)

    def __copy__(self):
        raise Exception('entity object does not support shallow copy')

    def __deepcopy__(self, memo):
        raise Exception('entity object does not support deep copy')

order1 = Order(101, 'one fish', 'one apple')
order2 = Order(101, 'one Fish')
print(order1 == order2) # True
print(order1 != order2) # False
some_map = {order1: None} # works
# copy.copy(order1) 会抛异常
# copy.deepcopy(order1) 会抛异常