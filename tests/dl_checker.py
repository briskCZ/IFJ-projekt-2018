#duplicate label checker
import sys
import os
path = os.path.abspath(sys.argv[1])
labels = []
lc = 0
label = ""
ins = ""
with open(path, 'r') as input:
    for line in input:
        lc = lc + 1
        try:
            ins, label = line.split(" ")
        except:
            pass
        if ins == "LABEL":
            print('pridavam: ' + ins + " " + label)
            if label not in labels:
                labels.append(label)
            else:
                print("Duplicate label: " + str(label), " line: " + str(lc))
