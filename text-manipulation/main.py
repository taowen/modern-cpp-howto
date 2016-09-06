# coding=utf-8
import unittest


class TextManipulation(unittest.TestCase):
    def test_substr(self):
        self.assertEqual('o w', 'hello world'[4:7])

    def test_multiline(self):
        self.assertEqual('abc\ndef', """abc
def""")

    def test_chinese(self):
        self.assertEqual(u'文', u'中文'[1])
        self.assertEqual(2, len(u'中文'))
        self.assertEqual(6, len('中文'))

    def test_utf8(self):
        self.assertEqual('中文', u'中文'.encode('utf8'))

    def test_split(self):
        self.assertListEqual(['hello', 'world'], 'hello world'.split(' '))

    def test_build_string(self):
        # small
        self.assertEqual('hello world', 'hello' + ' world')
        # large
        parts = []
        parts.append('h')
        parts.append('e')
        parts.append('ll')
        parts.append('o')
        self.assertEqual('hello', ''.join(parts))
