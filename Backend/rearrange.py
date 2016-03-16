import glob
import numpy as np

def rearr(file):
	f = open(file,"r")
	print(str(file))
	noOfGraphs = int(f.readline())
	# print noOfGraphs

	x = []
	points = []

	counter = 0
	for line in f:
		if (line == "<>\n"):
			break
		points = line.split(" ")
		x.append(points[0])
		counter = counter + 1

	# print x

	f.close()

	eachGraph = []
	counter = 0
	points = []
	y = []

	f = open(file,"r")
	f.readline()

	for line in f:
		# print line
		if(line == "<>\n" or line =="<>"):
			counter = counter + 1
			eachGraph.append(y)
			y = []
		else:
			points = line.split(" ")
			points[1] = points[1].strip("\n")
			y.append(points[1])

	print eachGraph
	eachGraph = np.asarray(eachGraph)
	print eachGraph.shape[0]

	temp = ""

	output = open(file,"w")
	# output.write(noOfGraphs)
	# output.write("\n")
	for i in range(len(x)):
		for j in range(eachGraph.shape[0]):
			temp = temp + " " + str(eachGraph[j][i])
		temp = str(x[i] + temp)
		print (temp)
		output.write(temp)
		output.write("\n")
		temp = ""
	output.close()

def main():
	for file in glob.glob("*_plot.txt"):
		print file
		rearr(file)

if __name__ == '__main__':
	main()