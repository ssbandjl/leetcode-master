import os, sys

def write_file(filename, data):
        fd = file(filename, "a+")
        fd.write(data)
        fd.close()

write_file("/dev/sdb", "itworld123")