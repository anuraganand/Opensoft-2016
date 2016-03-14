import wx
import numpy
import warnings
from numpy import array,mean,uint8
from PIL import Image
import os
import copy
import glob

def plotpoints(file, filename):
	filename = filename.split("_")[0]
	im = numpy.asarray(Image.open(file))
	size=wx.Image(file).GetSize()
	points = []
	axes=[]

	with open(filename + '.txt') as infile:
		content = infile.readlines()

	content = [x.strip('\n') for x in content]
	xmin=int(content[0])
	ymin=int(content[1])
	xmax=int(content[2])
	ymax=int(content[3])
	ymin = size[1]-ymin
	ymax = size[1]-ymax

	pt=[]
	with warnings.catch_warnings():
		warnings.simplefilter("ignore", category=RuntimeWarning)
		for i in range(im.shape[1]):
		    v=mean((im[:,i]>0).nonzero())
		    if v>0:	pt=pt+[[i,v]]
	pt1=[]
	points=pt
	for x in points:
		if x[0] > xmin and x[0] < xmax and x[1] > ymax and x[1] < ymin:
			pt1 = pt1 + [x]

	cal=(0,2.0e+07,1000,3.0e+07)
	cal=array(cal).astype(float)
	xfac=(cal[2]-cal[0])/(xmax-xmin)
	yfac=(cal[3]-cal[1])/(ymin-ymax)
	pt1=array(pt1).astype(float)
	pt1[:,0]=(pt1[:,0]-xmin)*xfac+cal[0]
	pt1[:,1]=(size[1]-pt1[:,1]-ymax)*yfac+cal[1]
	with open(filename + '_plot.txt', 'a') as outfile:
		outfile.write('\n')
		for point in pt1:
			outfile.write('{0:.2f} {1:.2f}\n'.format(point[0],point[1]))

for file in glob.glob("*.png"):
    print(file)
    filename = str(file)
    filename = os.path.splitext(filename)[0]
    plotpoints(file, filename)