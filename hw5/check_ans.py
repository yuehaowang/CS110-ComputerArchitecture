import sys

if len(sys.argv) < 2:
    print('missing file path')
    exit(0)

file_path = sys.argv[1]

with open(file_path, 'r') as f:
    txt = f.read()
    print(txt.count('*'))