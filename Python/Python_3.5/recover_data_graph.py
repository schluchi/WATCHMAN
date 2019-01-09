#!/usr/bin/env python
############################################################################
# Author: Anthony
# Date:10/3/18
#
# Plot the recovered data
###########################################################################

# Libraries
import sys

f = open("recovered_data.bin", "rb")

start_code = list()
header_vped = list()
header_window = list()
end_code = list()
data = list()
for k in range(0,11):
    window = list()
    for i in range(0,512):
        start_code.append(f.read(2))
        header_vped.append(int.from_bytes(f.read(1), byteorder='little', signed=False)*0.25)
        header_window.append(int.from_bytes(f.read(2), byteorder='little', signed=False))
        channel = list()
        for j in range(0,16):
            sample = list()
            for m in range(0,32):
                sample.append(int.from_bytes(f.read(2), byteorder='little', signed=False))
            channel.append(sample)
        window.append(channel)
        end_code.append(f.read(2))
    data.append(window)

f.close()

general_flag=True
flag=True
for tmp in start_code:
    if(str(tmp.hex()) != "55aa"):
        print("problem start code="+str(tmp.hex()))
        general_flag=False
        flag=False
if(flag):
    print("start code are ok!")

flag=True
for i in range(0,11):
    vped = i*0.25
    for j in range(0,512):
        if(header_vped[i*512 + j] != vped):
            print("problem header vped="+str(header_vped[i*512 + j])+" | vped="+str(vped))
            general_flag=False
            flag=False
if(flag):
    print("header vped are ok!")

flag=True
for i in range(0,11):
    for j in range(0,512):
        if(header_window[i*512 + j] != j):
            print("problem header window="+str(header_window[i*512 + j])+" | window="+str(j))
            general_flag=False
            flag=False
if(flag):
    print("header window are ok!")

flag=True
for tmp in end_code:
    if(str(tmp.hex()) != "33cc"):
        print("problem end code="+str(tmp.hex()))
        general_flag=False
        flag=False
if(flag):
    print("end code are ok!")

if(general_flag == False):
    sys.exit()

"""start_code = list()
vped = list()
window = list()
end_code = list()
data = list()

for k in range(0,5632):
    start_code.append(f.read(2))
    vped.append(int.from_bytes(f.read(1), byteorder='little', signed=False)*0.25)
    window.append(int.from_bytes(f.read(2), byteorder='little', signed=False))
    channel = list()
    for i in range(0,16):
        channel.append(list())
        for j in range(0,32):
            channel[i].append(int.from_bytes(f.read(2), byteorder='little', signed=False))
    data.append(channel)
    end_code.append(f.read(2))

for k in range(0,5632):
    print("start code: "+str(start_code[k].hex())+" | vped: "+str(vped[k])+" | window: "+str(window[k])+" | end code: "+str(end_code[k].hex()))"""