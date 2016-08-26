import unittest


class Test(unittest.TestCase):
    def test_foreach_on_lazy_range(self):
        for i in xrange(6):
            print i ** 2

    def test_foreach_on_list(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in colors:
            print color
