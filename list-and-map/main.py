import unittest
import itertools


class Test(unittest.TestCase):
    def test_foreach_on_lazy_range(self):
        for i in xrange(6):
            print(i ** 2)

    def test_foreach_on_list(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in colors:
            print(color)

    def test_foreach_reversed(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in reversed(colors):
            print(color)

    def test_foreach_with_index(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for i, color in enumerate(colors):
            print(i, color)

    def test_zip(self):
        names = ['raymond', 'rachel', 'matthew']
        colors = ['red', 'green', 'blue', 'yellow']
        for name, color in itertools.izip(names, colors):
            print(name, color)

    def test_sort(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in sorted(colors):
            print(color)

    def test_sort_reverse(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in sorted(colors, reverse=True):
            print(color)

    def test_custom_sort(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in sorted(colors, key=lambda e: len(e)):
            print(color)

    def test_any_of(self):
        colors = ['red', 'green', 'blue', 'yellow']
        print(any(color == 'green' for color in colors))

    def test_list_comprehension(self):
        colors = ['red', 'green', 'blue', 'yellow']
        print([len(color) for color in colors])

    def test_slicing(self):
        colors = ['red', 'green', 'blue', 'yellow']
        print(colors[1:2])
        print(colors[:2])
        print(colors[1:])
        print(colors[:-1])
