#!/usr/bin/env python
############################################################################
# Author: Anthony
# Date:10/3/18
#
# Plot the recovered data_1_25
###########################################################################

# Libraries
import sys
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import interp1d

def data_of_file(file, data_list):
    print("file: "+file)
    f = open(file, "rb")
    start_code = list()
    header_vped = list()
    header_window = list()
    end_code = list()
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
        data_list.append(window)
    f.close()
    general_flag=True
    flag=True
    for tmp in start_code:
        if(str(tmp.hex()) != "55aa"):
            #print("  - problem start code="+str(tmp.hex()))
            general_flag=False
            flag=False
    if(flag):
        print("  - start code are ok!")
    else:
        print("  - start code failed!")
    flag=True
    for i in range(0,11):
        vped = i*0.25
        for j in range(0,512):
            if(header_vped[i*512 + j] != vped):
                #print("  - problem header vped="+str(header_vped[i*512 + j])+" | vped="+str(vped))
                general_flag=False
                flag=False
    if(flag):
        print("  - header vped are ok!")
    else:
        print("  - header vped failed!")
    flag=True
    for i in range(0,11):
        for j in range(0,512):
            if(header_window[i*512 + j] != j):
                print("  - problem header window="+str(header_window[i*512 + j])+" | window="+str(j))
                general_flag=False
                flag=False
    if(flag):
        print("  - header window are ok!")
    else:
        print("  - header window failed!")
    flag=True
    for tmp in end_code:
        if(str(tmp.hex()) != "33cc"):
            print("  - problem end code="+str(tmp.hex()))
            general_flag=False
            flag=False
    if(flag):
        print("  - end code are ok!")
    else:
        print("  - end code failed!")
    if(general_flag == False):
        sys.exit()

"""data_1_25 = list()
data_of_file("recovered_data_1_25.bin", data_1_25)
data_2 = list()
data_of_file("recovered_data_2.bin", data_2)
data_no_pedestal = list()
data_of_file("recovered_data_no_pedestal.bin", data_no_pedestal)

y_1_25 = list()
for i in range(0,11):
    y_1_25.append(data_1_25[i][0][0][0])
y_2 = list()
for i in range(0,11):
    y_2.append(data_2[i][0][0][0])
y_no_pedestal = list()
for i in range(0,11):
    y_no_pedestal.append(data_no_pedestal[i][0][0][0])
y_ideal = np.linspace(0,2047,11)
x = np.linspace(0,2.5,11)
f = interp1d(x,y_no_pedestal, kind='cubic')
plt.plot(x,y_1_25,'ro',x,y_2,'b*',x,y_no_pedestal,'gv',x,y_ideal,'k',x,f(x),'c--')
plt.grid(True)
plt.show()"""

window = 0
channel = 0
sample = 0
data_file = list()
y = list()
x = np.linspace(0,2.5,11)
ax_1_25 = plt.subplot(1,2,1)
ax_1_25.set_title("vped@1.25V, wdo="+str(window)+" | ch="+str(channel)+" | sample="+str(sample))
y_ideal = np.linspace(0,2047,11)
ax_1_25.plot(x, y_ideal, ':', label='data ideal')
for i in range(0,10):
    data_file.append(list())
    file_name = "recovered_data_1_25_n"+str(i)+".bin"
    data_of_file(file_name, data_file[i])
    y.append(list())
    for j in range(0,11):
        y[i].append(data_file[i][j][window][channel][sample])
    ax_1_25.plot(x, y[i], label=('data '+str(i)))
ax_1_25.grid(True)
ax_1_25.legend()

data_file = list()
y = list()
x = np.linspace(0,2.5,11)
ax_2 = plt.subplot(1,2,2)
ax_2.set_title("vped@2V, wdo="+str(window)+" | ch="+str(channel)+" | sample="+str(sample))
y_ideal = np.linspace(0,2047,11)
ax_2.plot(x, y_ideal, ':', label='data ideal')
for i in range(0,10):
    data_file.append(list())
    file_name = "recovered_data_2_n"+str(i)+".bin"
    data_of_file(file_name, data_file[i])
    y.append(list())
    for j in range(0,11):
        y[i].append(data_file[i][j][window][channel][sample])
    ax_2.plot(x, y[i], label=('data '+str(i)))
ax_2.grid(True)
ax_2.legend()

plt.show()
