import glob
from fpdf import FPDF, HTMLMixin
 
title = ''
EiNames = []
width_perc = ''
html = ''
points = []
secondWord = ""

class MyFPDF(FPDF, HTMLMixin):
    pass

def table_html(points, legends):
	ni = len(points[0]) - 1
	print ni
	width_perc = str(100/(int(ni)+1)) + '%'
	print width_perc
	op_tags = '<tr><td>'
	md_tags = '</td><td>'
	cl_tags = '</td></tr>'
	yield '<H1 align="center">html2fpdf</H1>'
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

def call_write(file, legends):
	delimiter = 0
	points = []
	ni = 0
	with open(file) as infile:
		for line in infile:
			points.append(line.split())
	return ''.join(list(table_html(points, legends)))

def main():
	html = ''
	eachLegendLine = []
	legends = []
	legend = open("file_legend_map.txt")
	for line in legend:
		eachLegendLine = line.split(" ")
		secondWord = eachLegendLine[1]
		legends.append(secondWord)
	
	legends = [l.strip("\n") for l in legends]
	for file in glob.glob("*_plot.txt"):
	    html = html + call_write(file, legends)
	print html
	pdf = MyFPDF()
	pdf.add_page()
	pdf.write_html(html)

	pdf.output('html.pdf','F')

if __name__ == '__main__':
	main()