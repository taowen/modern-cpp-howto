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

    def test_reversed(self):
        colors = ['red', 'green', 'blue', 'yellow']
        self.assertListEqual(['yellow', 'blue', 'green', 'red'], list(reversed(colors)))

    def test_zip(self):
        names = ['raymond', 'rachel', 'matthew']
        colors = ['red', 'green', 'blue', 'yellow']
        self.assertListEqual([
            ('raymond', 'red'),
            ('rachel', 'green'),
            ('matthew', 'blue')
        ], zip(names, colors))

    def test_foreach_with_index(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for i, color in enumerate(colors):
            print(i, color)

    def test_foreach_map(self):
        d = {'matthew': 'blue', 'rachel': 'green', 'raymond': 'red'}
        for k, v in d.iteritems():
            print(k, v)

    def test_sort(self):
        colors = ['red', 'green', 'blue', 'yellow']
        self.assertListEqual(['blue', 'green', 'red', 'yellow'], sorted(colors))

    def test_sort_reverse(self):
        colors = ['red', 'green', 'blue', 'yellow']
        self.assertListEqual(['yellow', 'red', 'green', 'blue'], sorted(colors, reverse=True))

    def test_sort_by_lambda(self):
        colors = ['red', 'green', 'blue', 'yellow']
        self.assertListEqual(['red', 'blue', 'green', 'yellow'], sorted(colors, key=lambda e: len(e)))

    def test_list_comprehension(self):
        colors = ['red', 'green', 'blue', 'yellow']
        self.assertListEqual([3, 5, 4, 6], [len(color) for color in colors])

    def test_any_of(self):
        colors = ['red', 'green', 'blue', 'yellow']
        self.assertTrue(any(color == 'green' for color in colors))

    def test_slicing(self):
        colors = ['red', 'green', 'blue', 'yellow']
        self.assertListEqual(['green'], colors[1:2])
        self.assertListEqual(['red', 'green'], colors[:2])
        self.assertListEqual(['green', 'blue', 'yellow'], colors[1:])
        self.assertListEqual(['red', 'green', 'blue', 'yellow'], colors[:-1])

    def test_foreach_map_keys(self):
        d = {'matthew': 'blue', 'rachel': 'green', 'raymond': 'red'}
        for k in d.keys():
            if k.startswith('r'):
                del d[k]
        print(d.keys())

    def test_construct_map_by_paris(self):
        colors = ['red', 'green', 'blue', 'yellow']
        self.assertDictEqual({
            'red': 3,
            'green': 5,
            'blue': 4,
            'yellow': 6
        }, dict((color, len(color)) for color in colors))
