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

    def test_utf8(self):
        self.assertEqual('中文', u'中文'.encode('utf8'))
