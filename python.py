#!/bin/python3

print(chr(59))
print(chr(10))


def remove_non_ascii_1(text):
    return ''.join(i for i in text if ord(i)<128)