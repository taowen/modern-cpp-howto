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

class PurchaseItems(object):
    def __init__(self):
        self.items = [] # list of (sku, quantity)
        self.coupon_id = None # the coupon to use

    def __eq__(self, other):
        return self.items == other.items and self.coupon_id == other.coupon_id

    def __ne__(self, other):
        return not (self == other)

    def __hash__(self):
        raise Exception('mutable do not support hash')

    def __copy__(self):
        copied = PurchaseItems()
        copied.items = list(self.items)
        copied.coupon_id = self.coupon_id
        return copied

    def __deepcopy__(self, memo):
        copied = PurchaseItems()
        copied.items = copy.deepcopy(self.items, memo)
        copied.coupon_id = self.coupon_id
        return copied

p1 = PurchaseItems()
p1.items.append(('S102', 1))
p1.items.append(('S159', 2))
p1.coupon_id = 10001
p2 = PurchaseItems()
p2.items.append(('S102', 1))
p2.items.append(('S159', 2))
p2.coupon_id = 10001
print(p1 == p2) # True
print(p1 != p2) # False
# some_map = {p1: None} # not hashable
copy.copy(p1)
copy.deepcopy(p1)