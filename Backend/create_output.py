html = """
<H1 align="center">html2fpdf</H1> 
<table border="1" align="center" width="50%">
<thead><tr><th width="30%">Header 1</th><th width="70%">header 2</th></tr></thead>
<tbody>
<tr><td>cell 1</td><td>cell 2</td></tr>
<tr><td>cell 2</td><td>cell 3</td></tr>
</tbody>
</table>
"""
 
from fpdf import FPDF, HTMLMixin
 
class MyFPDF(FPDF, HTMLMixin):
    pass

def main():
	pdf = MyFPDF()
	pdf.add_page()
	pdf.write_html(html)
	pdf.add_page()
	pdf.write_html(html)

	pdf.output('html.pdf','F')

if __name__ == '__main__':
	main()