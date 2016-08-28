# coding=utf-8
import copy

# 网络 socket 显然应该属于 transient object

class MySocket(object):
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

sock1 = MySocket()
sock2 = MySocket()
# print(sock1 == sock2) 会抛异常
# print(sock1 != sock2) 会抛异常
# print(sock1 <> sock2) 会抛异常
# some_map = {sock1: None} 会抛异常
# copy.copy(sock1) 会抛异常
# copy.deepcopy(sock1) 会抛异常
