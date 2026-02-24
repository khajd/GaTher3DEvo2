#!/usr/bin/python

# Hello world python program

import numpy as np
import tkinter as tk
import xml.etree.ElementTree as ET
import sys
import csv
from PIL import ImageTk, Image

import pandas as pd
from pandas import DataFrame

import datetime
#import pandas_datareader.data

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def explode(data):
    size = np.array(data.shape)*2
    data_e = np.zeros(size - 1, dtype=data.dtype)
    data_e[::2, ::2, ::2] = data
    return data_e



class App:
    def __init__(self):
        
        if len(sys.argv)>1:
           path=sys.argv[1]#print(sys.argv[1])
        else:
           path=input("Podaj ścieżkę pliku XML\n")
        
        tree = ET.parse(path)
        root = tree.getroot()
        for g in root.iter('maxgeneration'):
           self.generation=int(g.text)+1;
        for s in root.iter('size'):
           sizeXML=int(s.text)
        for d in root.iter('dimension'):
           dimension=int(d.text)
        maxGeneration = self.generation
        self.size=[1,1,1]
        self.csv=[]
        
        for i in range(0,dimension):
           self.size[i]=sizeXML;
        
        self.fenotypes=0
        
        for fen in root.iter('row'):
           self.fenotypes=self.fenotypes+1
           
        for f in root.iter('save-path'):
           tempPath=f.text;
           filebin = open((f.text)+".bin","rb")
           
        for hd in root.iter('resources'):
           if(hd.text=='step' or hd.text=='linear' or hd.text=='sine' or hd.text=='localH' or hd.text=='globalH'):
               self.fenotypes=2;   
           
        tempPath=tempPath+'.csv'
        with open(tempPath) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            for it in range(self.fenotypes+1):
                self.csv.append([])
            for row in csv_reader:
                #self.csv.append(row)
                for it2 in range(self.fenotypes+1):
                    self.csv[it2].append(float(row[it2]))
     
        
        


        self.panel= None;

 
        self.data = np.fromfile(filebin,'<f4')
        

        self.data.shape = (self.generation,self.size[2],self.size[1],self.size[0],self.fenotypes)
        self.root = tk.Tk()
        self._job = None
        self.w=[];
       
	 
	

           
        self.label= tk.LabelFrame(self.root, text="Ustaw kanały alfa fenotypów do podglądu 3D")
        self.label.pack(side="bottom")
        
        self.button = tk.Button(self.label, text="Pokaż generację w 3D", command=self.openIn3D)
        self.button.pack(side="right")
        
        
        for fen in range(self.fenotypes):
            var = tk.DoubleVar(value=10)  # initial value
            self.w.append(tk.Spinbox(self.label, textvariable=var, from_=0, to=255, increment=1));#, format='%01.2f'
            if(fen<3):
                self.w[fen].pack(side="bottom")
        
        self.labelEmpty= tk.Label(self.root, text="")
        self.labelEmpty.pack(side="bottom")
        
        
        self.group = tk.LabelFrame(self.root, text="Zapisz podgląd 2D do pliku")
        self.group.pack(side="bottom")
        
        self.button = tk.Button(self.group, text="Zapisz GEN", command=self.saveImagesGen)
        self.button.pack(side="right")
        self.button = tk.Button(self.group, text="Zapisz", command=self.save)
        self.button.pack(side="right")
        self.button = tk.Button(self.group, text="Zapisz GIF", command=self.saveImagesToGIF)
        self.button.pack(side="right")        

        self.pathEntry = tk.Entry(self.group)
        self.pathEntry.insert(0,'image.png')
        self.pathEntry.pack(side="right")
        
        self.genValue=tk.IntVar();
        
        self.labelGeneration = tk.LabelFrame(self.root, text="Generacja")
        self.labelGeneration.pack(side="bottom")
        self.slider = tk.Scale(self.labelGeneration, from_=0, to=self.generation-1, 
                               orient="horizontal", 
                               command=self.updateValue, variable=self.genValue, showvalue=0, length=300)
        
        self.meanBool = tk.IntVar()
        self.checkButton=tk.Checkbutton(self.labelGeneration, text="Pokaż uśrednionie wszystkich generacji", variable=self.meanBool, command=self.meanUpdate)
        self.checkButton.pack(side="bottom")
        
        self.slider.pack(side = "left")
        
        self.labelGenVal=tk.Label(self.labelGeneration, textvariable=self.genValue)
        self.labelGenVal.pack(side="left")
        
        
        
        
        self.slider2 = tk.Scale(self.root, from_=1, to=self.size[2],  
                               command=self.updateValue)
        
        if(self.size[2]!=1):
            self.slider2.pack(side = "right")
        
        
        self.button = tk.Button(self.root, text="Pokaż przebiegi fenotypów", command=self.openIn2D)
        self.button.pack(side="top")
        
    
       
        
        onegeneration = self.data[0,0,:,:,:]
   
        
        #print(onegeneration)
        img = ImageTk.PhotoImage(Image.fromarray(np.uint8(onegeneration* 255)))# , 'L'))
        #tkimage = ImageTk.PhotoImage(img.rotate(90))
        self.panel = tk.Label(self.root, image = img)
        self.panel.pack(side = "top", fill = "both", expand = "yes")
        
        self.root.title(path)
        self.root.mainloop()
        
        
    def saveImagesToGIF(self):
        frames = []
        for i in range(0,self.generation):
            generation = self.data[i,self.slider2.get()-1,:,:,:]
            cur = self.slider.get()
            cur2 = self.slider2.get()
            
            zer=np.zeros((generation.shape[0],generation.shape[1],3))
            generation2=np.append(generation,zer,axis=2)
            generation3=generation2[:,:,0:3]
            img2=Image.fromarray(np.uint8(generation3* 255))
            img3=img2.rotate(90)
            resized = img3.resize((350, 350),Image.FIXED)
            frames.append(resized)
            #resized.save(self.pathEntry.get())
        
        frames[0].save('gif.gif', format='GIF', append_images=frames[1:], save_all=True, duration=0.01, fps=60, loop=0)
        #img.write('some_name.gif', format='gif')

    def saveImagesGen(self):
        frames = []
 #       for i in range(0,self.generation):

        generation = self.data[0,self.size[2]//2,:,:,:]
        zer=np.zeros((generation.shape[0],generation.shape[1],3))
        generation2=np.append(generation,zer,axis=2)
        generation3=generation2[:,:,0:3]
        img2=Image.fromarray(np.uint8(generation3* 255))
        img3=img2.rotate(90)
        resized = img3.resize((350, 350),Image.FIXED)
        frames.append(resized)
            #resized.save(self.pathEntry.get())
        
        frames[0].save('gen0.PNG', format='PNG', append_images=frames[1:], save_all=True, duration=0.01, fps=60, loop=0)

        frames = []
 #       for i in range(0,self.generation):

        generation = self.data[self.generation//2,self.size[2]//2,:,:,:]
        zer=np.zeros((generation.shape[0],generation.shape[1],3))
        generation2=np.append(generation,zer,axis=2)
        generation3=generation2[:,:,0:3]
        img2=Image.fromarray(np.uint8(generation3* 255))
        img3=img2.rotate(90)
        resized = img3.resize((350, 350),Image.FIXED)
        frames.append(resized)
            #resized.save(self.pathEntry.get())
        
        frames[0].save('genmid.PNG', format='PNG', append_images=frames[1:], save_all=True, duration=0.01, fps=60, loop=0)
        #img.write('some_name.gif', format='gif')
             
        frames = []
 #       for i in range(0,self.generation):

        generation = self.data[self.generation-1,self.size[2]//2,:,:,:]
        zer=np.zeros((generation.shape[0],generation.shape[1],3))
        generation2=np.append(generation,zer,axis=2)
        generation3=generation2[:,:,0:3]
        img2=Image.fromarray(np.uint8(generation3* 255))
        img3=img2.rotate(90)
        resized = img3.resize((350, 350),Image.FIXED)
        frames.append(resized)
            #resized.save(self.pathEntry.get())
        
        frames[0].save('genlast.PNG', format='PNG', append_images=frames[1:], save_all=True, duration=0.01, fps=60, loop=0)
        #img.write('some_name.gif', format='gif')
        
        fig = plt.figure()
        col=[];
        col.append('r')
        col.append('g')
        col.append('b')
        col.append('c')
        col.append('m')
        col.append('y')
        col.append('k') 
        for i in range(self.fenotypes):
            if i == 0:
                plt.plot(self.csv[0],self.csv[i+1], col[i], label=('Fenotyp INV'))
            if i == 1:
                plt.plot(self.csv[0],self.csv[i+1], col[i], label=('Fenotyp AG'))
            if i == 2:
                plt.plot(self.csv[0],self.csv[i+1], col[i], label=('Fenotyp GLY'))
            if i == 3:
                plt.plot(self.csv[0],self.csv[i+1], col[i], label=('Fenotyp N'))
        plt.legend()
        axes = plt.gca()
        axes.set_ylim([0.0,1.0])
        
        plt.xlabel("Generation")
        plt.ylabel("Frequency")
        plt.savefig('timecourse.png')
        #plt.show()
             


    def updateImage(self, generation):
        
        
        
        
        zer=np.zeros((generation.shape[0],generation.shape[1],3))
        
        
        
       
        generation2=np.append(generation,zer,axis=2)
        
        
        generation3=generation2[:,:,0:3]
        
        
   
        img2=Image.fromarray(np.uint8(generation3* 255))
        img3=img2.rotate(90)
        resized = img3.resize((350, 350),Image.FIXED)
        img = ImageTk.PhotoImage(resized) # Keep a reference, prevent GC
        
        #img = ImageTk.PhotoImage(Image.fromarray(np.uint8(generation2* 255)))
        self.panel.configure(image=img)
        self.panel.image = img

    def updateValue(self, event):
        if self._job:
            self.root.after_cancel(self._job)
        self._job = self.root.after(50, self._do_something)
    def meanUpdate(self):
        if self._job:
            self.root.after_cancel(self._job)
        self._job = self.root.after(50, self._do_something)

    def _do_something(self):
        self._job = None
        if self.meanBool.get():
            self.showMean()

        else:
            onegeneration = self.data[self.slider.get(),self.slider2.get()-1,:,:,:]
            self.updateImage(onegeneration)

        
    def save(self):
        
        generation = self.data[self.slider.get(),self.slider2.get()-1,:,:,:]
              #[gen,layer,:,:,:]
        zer=np.zeros((generation.shape[0],generation.shape[1],3))
        
       
        generation2=np.append(generation,zer,axis=2)
        
        generation3=generation2[:,:,0:3]
        
       
        
   
        img2=Image.fromarray(np.uint8(generation3* 255))
        img3=img2.rotate(90)
        resized = img3.resize((350, 350),Image.FIXED)
        resized.save(self.pathEntry.get())
        
        #img = ImageTk.PhotoImage(resized) # Keep a reference, prevent GC
        #img = ImageTk.PhotoImage(Image.fromarray(np.uint8(generation2* 255)))
        #self.panel.configure(image=img)
        #self.panel.image = img
        
        

    def showMean(self):
        
        layer=self.slider2.get();
        temp=self.data[0,layer-1,:,:,:]
        genSum=np.zeros((temp.shape[0],temp.shape[1],3));
        for g in range(self.generation):
     
            generation = self.data[g,layer-1,:,:,:]
              #[gen,layer,:,:,:]
            zer=np.zeros((generation.shape[0],generation.shape[1],3))
        
       
            generation2=np.append(generation,zer,axis=2)
        
            generation3=generation2[:,:,0:3]
            genSum=genSum+generation3
        
       
        genSum=genSum/(self.generation)
       
        
   
        img2=Image.fromarray(np.uint8(genSum* 255))
        img3=img2.rotate(90)
        resized = img3.resize((350, 350),Image.FIXED)
        #resized.save(self.tempPath+"Mean.png")
        img = ImageTk.PhotoImage(resized) # Keep a reference, prevent GC
        
        #img = ImageTk.PhotoImage(Image.fromarray(np.uint8(generation2* 255)))
        self.panel.configure(image=img)
        self.panel.image = img
  
        


    def openIn2D(self):
        fig = plt.figure()
        col=[];
        col.append('r')
        col.append('g')
        col.append('b')
        col.append('y')
        col.append('m')
        col.append('y')
        col.append('c') 
        for i in range(self.fenotypes):
            if i < 7:
                plt.plot(self.csv[0],self.csv[i+1], col[i], label=('Phenotype '+str(i+1)))
            else:
                plt.plot(self.csv[0],self.csv[i+1], label=('Phenotype '+str(i+1)))
        plt.legend()
        axes = plt.gca()
        axes.set_ylim([0.0,1.0])
        
        plt.xlabel("Generation")
        plt.ylabel("Frequency")
        #plt.savefig(self.tempPath+'.png')
        plt.show()

    def openIn3D(self):
        voxels = np.ones((self.size[2]//2,self.size[1], self.size[0]))
        colors = np.empty(voxels.shape, dtype=object)
        fentemp=self.fenotypes
        if(fentemp>3):
            fentemp=3
        for i in range(self.size[2]//2):
           for j in range(self.size[1]):
              for k in range(self.size[0]):
                 c=np.zeros(3);
                 alpha=0.0;
                 for fen in range(fentemp):
                     #if fen==3:
                         #c = [255,204,255]
                         #colors[i,j,k]="#2052042555" 
                     #else:
                        c[fen]=255*self.data[self.slider.get(),i,j,k,fen]
                        alpha=alpha+self.data[self.slider.get(),i,j,k,fen]*int(self.w[fentemp-1-fen].get())
                    
                    
                        colors[i,j,k]="#%02X%02X%02X%02X" % (int(c[0]), int(c[1]), int(c[2]),int(alpha))
                        
                        if (c[0]==0 and c[1]==0 and c[2]==0):
                             colors[i,j,k]="#%02X%02X%02X%02X" % (int(255), int(204), int(255),int(1))
                                               

        filled = np.ones(voxels.shape)
        filled_2 = explode(filled)
        fcolors_2 = explode(colors)
        # Shrink the gaps
        x, y, z = np.indices(np.array(filled_2.shape) + 1).astype(float) // 2
        x[0::2, :, :] += 0.01
        y[:, 0::2, :] += 0.01
        z[:, :, 0::2] += 0.01
        x[1::2, :, :] += 0.99
        y[:, 1::2, :] += 0.99
        z[:, :, 1::2] += 0.99
        fig = plt.figure()
        ax= fig.add_subplot(projection = '3d')
        #ax = fig.gca(projection='3d')
        ax.voxels(x,y,z,filled_2, facecolors=fcolors_2)#, edgecolors='#00000030')
        plt.axis('on')
        ax.set_xlim([0.0,self.size[0]])
        ax.set_ylim([0.0,self.size[0]])
        ax.set_zlim([0.0,self.size[0]])
        ticks = np.arange(0,101,50)
        ax.set_xticks(ticks)
        ax.set_yticks(ticks)
        ax.set_zticks(ticks)
        #plt.xticks(np.arange(0, 101, 50))
        #plt.yticks(np.arange(0, 101, 50))
        #set_zticks(np.arange(0, 101, 50))
        ax.view_init(30,45)
        plt.show()
        
    def openIn3D_old(self):
        X=[];
        Y=[];
        Z=[];
        fenmin=[];
        fenmax=[];
        fenmin.append(0.5);#To muszę wziąć z boxów
        fenmax.append(1.0);
        fenmin.append(0.0);
        fenmax.append(0.0);
        
        for i in range(self.size[2]):
           for j in range(self.size[1]):
              for k in range(self.size[0]):
                 append=1
                 for fen in range(self.fenotypes):
                    if(self.data[self.slider.get(),i,j,k,fen]<fenmin[fen] or self.data[self.slider.get(),i,j,k,fen]>fenmax[fen]):
                       append=0
                 if(append):
                    X.append(i)
                    Y.append(j)
                    Z.append(k)
        threedee = plt.figure().gca(projection='3d') 
        threedee.scatter(X, Y, Z)
        threedee.set_xlabel('X')
        threedee.set_xlim([0,self.size[2]])
        threedee.set_ylabel('Y')
        threedee.set_ylim([0,self.size[1]])
        threedee.set_zlabel('Z')
        threedee.set_zlim([0,self.size[0]])
        plt.show()

app=App()
