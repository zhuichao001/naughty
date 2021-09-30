#analyse result of mprocess_write.cpp


ok = True
f = open("fwrite.txt")
for line in f:
    if len(line)!=10:
        print(line)
        ok = False
if ok:
    print ("fwrite is safe for multi process" )
else:
    print ("fwrite is not safe for multi process" )


ok = True
f = open("write.txt")
for line in f:
    if len(line)!=10:
        print(line)
        ok = False
if ok:
    print ("write is safe for multi process" )
else:
    print ("write is not safe for multi process" )
