import sys

maxNumLines = 0
files = []

for csvFile in sys.argv[1:]:
    fileLines = []
    numLines = 0
    
    for line in open(csvFile):
        fileLines.append(line.strip())
        numLines += 1

    if numLines > maxNumLines:
        maxNumLines = numLines

    files.append(fileLines)
    print(maxNumLines)

outFile = open("histogram.dat", "w+")

for lineIndex in range(0, maxNumLines):
    outLine = "" + str(lineIndex + 1) + "\t"
    fileIndex = 0

    for fileLines in files:
        if lineIndex < len(fileLines):
            outLine += fileLines[lineIndex]
        else:
            outLine += "0"    

        if fileIndex < (len(files) - 1):
            outLine += "\t"

        fileIndex += 1

    outFile.write(outLine + "\n")

outFile.close()
