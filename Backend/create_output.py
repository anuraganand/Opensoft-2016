import glob
import os
from fpdf import FPDF, HTMLMixin
 
title = ''
EiNames = []
width_perc = ''
html = ''
points = []
secondWord = ""

class MyFPDF(FPDF, HTMLMixin):
    pass

def table_html(points, legends, heading):
	headi = heading[0]
	ni = len(points[0]) - 1
	print ni
	width_perc = str(100/(int(ni)+1)) + '%'
	print width_perc
	op_tags = '<tr><td>'
	md_tags = '</td><td>'
	cl_tags = '</td></tr>'
	yield '<H1 align="center">%s</H1>' % (headi)
	yield '<table border="1" align="center" width="100%">'
	yield '<thead><tr>'
	yield '<th width="%s">%s</th>' % (width_perc,'X')
	for i in xrange(int(ni)):
		yield '<th width="%s">%s</th>' % (width_perc,legends[i])
	yield '</tr></thead>'
	yield '<tbody>'
	for p in points:
		yield op_tags
		for i in xrange(int(ni)):
			yield str(p[i]) + md_tags
		yield str(p[int(ni)]) + cl_tags
	yield '</tbody>'
	yield '</table>'

def call_write(file, legends, heading):
	delimiter = 0
	points = []
	ni = 0
	with open(file) as infile:
		for line in infile:
			points.append(line.split())
	return ''.join(list(table_html(points, legends, heading)))

def main():
	html = ''
	eachLegendLine = []
	legends = []
	legend = open("file_legend_map.txt")
	for line in legend:
		eachLegendLine = line.split(" ")
		secondWord = eachLegendLine[1]
		legends.append(secondWord)
	with open("heading.txt") as head:
		heading = head.readlines()
	print heading
	legends = [l.strip("\n") for l in legends]
	print legends
	for file in glob.glob("*_plot.txt"):
	    html = html + call_write(file, legends, heading)
	with open(str(os.getcwd().split("graph_extractor",1)[1].replace("/","_")) + '_html.txt', 'a') as infile:
		infile.write(html);
	print html

if __name__ == '__main__':
	main()