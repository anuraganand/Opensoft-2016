import glob
import os
from fpdf import FPDF, HTMLMixin

class MyFPDF(FPDF, HTMLMixin):
    pass

def call_write(file):
	with open(file) as infile:
		content = infile.readlines()
	return content

def main():
	html = '<H1 align="center">TEAM 04</H1>'
	for file in glob.glob("./graph*/test*/graph*/*_html.txt"):
	    html = html + ''.join(call_write(file))
	print html
	pdf = MyFPDF()
	pdf.add_page()
	pdf.write_html(html)

	pdf.output('output.pdf','F')

if __name__ == '__main__':
	main()