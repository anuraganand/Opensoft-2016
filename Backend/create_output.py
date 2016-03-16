import glob
from fpdf import FPDF, HTMLMixin
 
title = ''
EiNames = []
width_perc = ''
html = ''
points = []

class MyFPDF(FPDF, HTMLMixin):
    pass

def table_html(points):
	ni = len(points[0])
	width_perc = str(100/(int(ni))) + '%'
	print width_perc
	op_tags = '<tr><td>'
	md_tags = '</td><td>'
	cl_tags = '</td></tr>'
	yield '<H1 align="center">html2fpdf</H1>'
	yield '<table border="1" align="center" width="100%">'
	yield '<thead><tr>'
	# yield '<th width="%s">%s</th><th width="%s">%s</th>' % (,,,)
	for i in xrange(int(ni)):
		yield '<th width="%s">%s</th>' % (width_perc,str(ni))
	yield '</tr></thead>'
	yield '<tbody>'
	# print ni
	for p in points:
		# yield op_tags + str(p[0]) + md_tags + str(p[1]) + cl_tags
		yield op_tags
		for i in xrange(int(ni)-1):
			yield str(p[i]) + md_tags
		yield str(p[int(ni)-1]) + cl_tags
	yield '</tbody>'
	yield '</table>'

def call_write(file):
	delimiter = 0
	points = []
	ni = 0
	with open(file) as infile:
		for line in infile:
			points.append(line.split())
	# ni = points.shape[1]
	return ''.join(list(table_html(points)))

def main():
	html = ''
	for file in glob.glob("*_plot.txt"):
	    # print(file)
	    html = html + call_write(file)
	# print html
	print html
	pdf = MyFPDF()
	pdf.add_page()
	pdf.write_html(html)

	pdf.output('html.pdf','F')

if __name__ == '__main__':
	main()