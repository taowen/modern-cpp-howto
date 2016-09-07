# coding=utf-8
import unittest
import re


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

    def test_format(self):
        # positional
        self.assertEqual('hello world', '{} {}'.format('hello', 'world'))
        # named
        self.assertEqual('hello world', '{v1} {v2}'.format(v1='hello', v2='world'))
        # format
        self.assertEqual('3.14', '{:.2f}'.format(3.1415))

    def test_lower_upper(self):
        self.assertEqual('hello world', 'Hello World'.lower())
        self.assertEqual('HELLO WORLD', 'Hello World'.upper())

    def test_startswith(self):
        self.assertTrue('Hello World'.startswith('He'))

    def test_endswith(self):
        self.assertTrue("Hello World".endswith('ld'))

    def test_strip(self):
        self.assertEqual('hello world', ' hello world'.lstrip())
        self.assertEqual('hello world', 'hello world '.rstrip())
        self.assertEqual('hello world', ' hello world '.strip())

    def test_find(self):
        self.assertEqual(2, 'hello'.find('l'))
        self.assertEqual(-1, 'hello'.find('!'))
        self.assertEqual(3, 'hello'.rfind('l'))

    def test_replace(self):
        self.assertEqual('he__o', 'hello'.replace('l', '_'))
        self.assertEqual('he_lo', 'hello'.replace('l', '_', 1))

    def test_regex_search(self):
        match = re.search('e\wl', 'HELLO', re.IGNORECASE)
        self.assertEqual('ELL', match.group())

    def test_regex_replace(self):
        self.assertEqual('HeLLo', re.sub('[l|h]+', lambda match: match.group().upper(), 'hello'))

    def test_numeric_conversion(self):
        self.assertEqual(42, int('42'))
        self.assertEqual(4.2, float(4.2))
