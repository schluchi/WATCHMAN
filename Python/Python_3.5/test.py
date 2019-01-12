#!/usr/bin/env python
############################################################################
# Author: Anthony
# Date:10/3/18
#
# Plot the recovered data_1_25
###########################################################################

# Libraries
import numpy as np
from scipy.interpolate import interp1d
import matplotlib.pyplot as plt
x = np.linspace(0,2.5,11)
y = x*x #[588,587,596,675,819,1009,1249,1473,1692,1821,1823]
p = np.polyfit(x,y,2)
#p = [ -107.659674, 111.142191, 337.156177, -114.275058]
f = np.poly1d(p)
print(x)
print(y)
print(p)
a = plt.subplot(1,1,1)
a.plot(x,y,x,f(x))
a.grid(True)
plt.show()