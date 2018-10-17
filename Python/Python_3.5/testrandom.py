import tkinter as Tkinter
from threading import Timer

def timer_int():
    print(int("0x10",0)+1)

root = Tkinter.Tk()
timer=Timer(0.5, timer_int)
timer.start()
x=322
print(x/256)
print(x%256)

Tkinter.mainloop()

