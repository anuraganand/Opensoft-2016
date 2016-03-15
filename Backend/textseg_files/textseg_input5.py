import cv2
import numpy as np

# Either x_origin & y_origin should be evaluated by another file
# or else each image must be scaled to a particular size of m*n to give the same x_origin & y_origin. Initialise that x_origin & y_origin here.

x_origin = 60
y_origin = 250
x_top = x_origin
y_top = 20

# Load the image

img = cv2.imread('input5.png')
# extendr = cv2.row(img) / 20
# extendc = cv2.col(img) / 20
extendr = 0
extendc = 0

# convert to grayscale
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

# smooth the image to avoid noises
gray = cv2.medianBlur(gray,5)

# Apply adaptive threshold
thresh = cv2.adaptiveThreshold(gray,255,1,1,11,2)
thresh_color = cv2.cvtColor(thresh,cv2.COLOR_GRAY2BGR)

# apply some dilation and erosion to join the gaps
# Edit required for enhancement in case of decimals

thresh = cv2.dilate(thresh,None,iterations = 3)
thresh = cv2.erode(thresh,None,iterations = 2)

# Find the contours
contours,hierarchy = cv2.findContours(thresh,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
# cv2.circle(img,img(x_origin,y_origin),img(x_origin+5,y_origin),(0,255,0),2)
          
# For each contour, find the bounding rectangle and draw it
for cnt in contours:
    x,y,w,h = cv2.boundingRect(cnt)
    if x < x_origin :
    	if (x+w) < x_origin :
    	  x = x - extendr
          y = y - extendc
          w = w + 2 * extendr
          h = h + 2 * extendc
    	  cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),1)
          cv2.rectangle(thresh_color,(x,y),(x+w,y+h),(0,255,0),1)
          print x
          print y
          print w
          print h 
    if y > y_origin :
    	if (y+h) > y_origin :
          x = x - extendr
          y = y - extendc
          w = w + 2 * extendr
          h = h + 2 * extendc
          cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),1)
          cv2.rectangle(thresh_color,(x,y),(x+w,y+h),(0,255,0),1)
          print x
          print y
          print w
          print h 
   

# Finally show the image
cv2.imshow('img',img)
cv2.imwrite("output5.png",img);

cv2.imshow('res',thresh_color)
cv2.waitKey(0)
cv2.destroyAllWindows()
