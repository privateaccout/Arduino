#-.- coding:utf-8
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import serial
import numpy as np
from Tkinter import *
import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import threading

root=Tk()
root.resizable(width=0, height=0)
root.title("By Kid")

ser = serial.Serial("/dev/ttyACM1",9600,timeout = 1)
matplotlib.use('TkAgg')

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
xs = []
ys = []
k = 0
Is_Run = 1
y_low_int = 0
y_high_int = 0
Is_Auto = 1
def test():
    global ys
    while Is_Run:
        try:
            line = ser.readline()
            try:
                b = float(line)
                ys.append(b)
                ys = ys[-50:]
            except :
                print line
                print "ValueError"
                continue
        except :
            print "no data receive"
            #root.after(100,test)
            continue
        #time.sleep(0.001)  
def ani():
    global xs
    global ys
    global k
    while Is_Run:
        k += 1
        if k > 60000 :
            k = 0
        xs.append(k)
        if len(xs) :
            xs = xs[-50:]
        try:
            ax.clear()
            if (y_low_int < y_high_int) and not Is_Auto  :
                plt.ylim([y_low_int,y_high_int])
            ax.plot(xs,ys)
            canvas.show()
        except :
            #print line
            print "Show Error"
            #continue
        #time.sleep(0.001)  
    
def Set_ylim():
    global y_low_int
    global y_high_int
    global Is_Auto
    try :
        y_low_int=int(y_low.get())
        y_high_int=int(y_high.get())
        Is_Auto = 0
    except :
        print "Input Error"

def Set_Auto():
    global Is_Auto
    Is_Auto = 1


def Quit():
    global Is_Run
    Is_Run = 0
    print "close!"
    root.quit()


def start():
    global Start_B
    Start_B.config(state = 'disable')
    recvth = threading.Thread(target=test,args={})
    drawth = threading.Thread(target=ani,args={})
    recvth.start()
    #time.sleep(2000)
    drawth.start()
    
    

Start_B = Button(root,text='Begin!!',command=start)
Start_B.grid(row=1,column=0)

#List_COM = Listbox(root,height=1)
#List_COM.grid(row=2,column=0)

Label(root,text='y_low').grid(row=1,column=1)
y_low=Entry(root)
y_low.grid(row=1,column=2)
y_low.insert(0,'0')

Label(root,text='y_high').grid(row=2,column=1)
y_high=Entry(root)
y_high.grid(row=2,column=2)
y_high.insert(0,'120')

Button(root,text='Set_Y',command=Set_ylim).grid(row=1,column=3)
Button(root,text='Auto ',command=Set_Auto).grid(row=2,column=3)


canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().grid(row=0, columnspan=4)





root.protocol("WM_DELETE_WINDOW", Quit)
root.mainloop()

