import sys
from collections import defaultdict

minLineNum = sys.maxint
maxLineNum = -sys.maxint - 1
values = defaultdict(dict)
fileIndex = 0

for csvFile in sys.argv[1:]:
    fileLines = []
    
    for line in open(csvFile):
        lineArray = line.strip().split(",")
        lineNum = int(lineArray[0])

        values[lineNum][fileIndex] = float(lineArray[1])

        if lineNum > maxLineNum:
           maxLineNum = lineNum
        if lineNum < minLineNum:
           minLineNum = lineNum


        #fileLines.append(line.strip())

    #files.append(fileLines)

    fileIndex = fileIndex + 1
    print(minLineNum)
    print(maxLineNum)

#print(values)

outFile = open("histogram.dat", "w+")

for lineIndex in range(minLineNum, maxLineNum + 1):
    outLine = "" + str(lineIndex) + "\t"

    for fileIndex2 in range(0, fileIndex):
        value = values[lineIndex].get(fileIndex2)

        if value == None:
            outLine += "0"    
        else:
            outLine += str(value)

        if fileIndex2 < (fileIndex - 1):
            outLine += "\t"

    outFile.write(outLine + "\n")

outFile.close()
