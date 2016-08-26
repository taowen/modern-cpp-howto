import unittest


def fib(n):
    if n <= 1:
        return n
    else:
        return fib(n - 1) + fib(n - 2)


class FibTest(unittest.TestCase):
    def test_fib_0(self):
        self.assertEqual(0, fib(0))

    def test_fib_1(self):
        self.assertEqual(1, fib(1))

    def test_fib_5(self):
        self.assertEqual(5, fib(5))
