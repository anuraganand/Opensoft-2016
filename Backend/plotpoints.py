import numpy
import warnings
from numpy import array,mean,uint8
from PIL import Image
import os
import copy
import glob
import cv2

noOfInterval = 15

def find_nearest(array,value):
	idx = 0
	idx1 = 0
	array = array - value
	array1 = [i for i in array if i >= 0]
	array2 = [i for i in array if i < 0]
	if array1:
		idx = array1.index(min(array1))
	else:
		idx = -1
	if array2:
		idx1 = array2.index(max(array2))
	else:
		idx = -1
	return idx, idx1

def plotpoints(file, filename):
	# print filename
	filename = filename.split("_")[0]
	im = numpy.asarray(Image.open(file))
	im1 = cv2.imread(file)
	width, height, channels = im1.shape
	size = (height, width)
	points = []
	axes=[]

	infile = glob.glob1(os.getcwd(),"graph_[0-9]*.txt")[0]
	print "infile ", infile

	content = open(infile).readlines()

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

	with open('axisvalsx.txt') as axisvals:
		calx = axisvals.readlines()

	with open('axisvalsy.txt') as axisvals1:
		caly = axisvals1.readlines()
	calx = [x.strip("\n") for x in calx]
	caly = [y.strip("\n") for y in caly]
	# print calx
	# print caly
	cal = []
	cal.append(calx[0])
	cal.append(caly[0])
	cal.append(calx[1])
	cal.append(caly[1])
	cal = [float(x.strip('\n')) for x in cal]

	xfac=float((cal[2]-cal[0])/(xmax-xmin))
	yfac=float((cal[3]-cal[1])/(ymin-ymax))
	pt1=array(pt1).astype(float)
	pt1[:,0]=(pt1[:,0]-xmin)*xfac+cal[0]
	pt1[:,1]=(size[1]-pt1[:,1]-ymax)*yfac+cal[1]

	""" TAKING OUT ONLY 15 POINTS CORRESPONDING TO THE X-AXIS """
	minX = cal[0]
	maxX = cal[2]
	difference = maxX - minX
	interval = difference/noOfInterval
	counter = 0
	newPt = []
	pt1 = numpy.asarray(pt1)
	
	for i in xrange(0,16):
		val = minX + (interval * i)
		# print val

		closest, closest1 = (find_nearest(pt1[:,0], val))
		if closest == -1 and closest1 == -1:
			avg = "-"
		elif closest == -1 and closest1 != -1:
			if pt1[closest1][0] - val > (0.05 * interval):
				avg = "-"
			else:
				avg = pt1[closest1][1]
		elif closest != -1 and closest1 == -1:
			if pt1[closest][0] - val > (0.05 * interval):
				avg = "-"
			else:
				avg = pt1[closest][1]
		else:
			if pt1[closest1][0] - val > (0.05 * interval) and pt1[closest][0] - val > (0.05 * interval):
				avg = "-"
			elif pt1[closest][0] - val > (0.05 * interval):
				avg = pt1[closest1][1]
			elif pt1[closest1][0] - val > (0.05 * interval):
				avg = pt1[closest][1]
			else:
				avg = (pt1[closest][1] + pt1[closest1][1])/2.0
		newPt.append([val, avg])
	for singlePoint in newPt:
		if (singlePoint[1] == '-'):
			singlePoint[0] = "%.5f" % (round(singlePoint[0], 5))
		else:
			singlePoint[0] = "%.5f" % (round(singlePoint[0], 5))
			singlePoint[1] = "%.5f" % (round(singlePoint[1], 5))

	with open(str(os.getcwd().split("graph_extractor",1)[1].replace("/","_")) + '_plot.txt', 'a') as outfile:
		for point in newPt:
			point = str(point)
			point = point.strip('[')
			point = point.strip(']')
			point = point.strip("'")
			point = point.replace("', '"," ")
			outfile.write(point)
			outfile.write("\n")
		outfile.write('<>\n')

pngCounter = len(glob.glob1(os.getcwd(),"*color*.png"))
print glob.glob1(os.getcwd(),"*color*.png")

for file in glob.glob("*color*.png"):
	# print os.getcwd()
	# print os.getcwd().split("graph_extractor",1)[1]
	filename = str(file)
	filename = os.path.splitext(filename)[0]
	filename = filename.split("_")[0]
	with open(str(os.getcwd().split("graph_extractor",1)[1].replace("/","_")) + '_plot.txt', 'a') as outfile:
		outfile.write(str(pngCounter) + '\n')
	break
for file in glob.glob("*color*.png"):
    print(file)
    filename = str(file)
    filename = os.path.splitext(filename)[0]
    plotpoints(file, filename)