import cv2
import numpy as np
import sys
# Either x_origin & y_origin should be evaluated by another file
# or else each image must be scaled to a particular size of m*n to give the same x_origin & y_origin. Initialise that x_origin & y_origin here.


x_origin = 53+30
y_origin = 473-20
y_top = 43
right_mostx=5

input_points = open(sys.argv[2],"r")
print sys.argv[2]
ct=0
for line in input_points:
  if ct==0:
    x_origin= int(line)
  elif ct==3:
    y_origin = int(line)
  elif ct==1:
    y_top = int(line)
  else:
    right_mostx = int(line)
  ct=ct+1
input_points.close()

left_mosty = x_origin
right_mosty = 0
top_mosty = y_origin
x_top = x_origin
bottom_mosty = 0
left_mostx = x_origin
right_mostx = 0
top_mostx = y_origin
bottom_mostx = 0
left_mostx_top = x_origin
right_mostx_top = 0
top_mostx_top = y_origin
bottom_mostx_top = 0
# Load the image
img = cv2.imread(sys.argv[1])
print sys.argv[1]

# extendr = cv2.row(img) / 20
# extendc = cv2.col(img) / 20
extendr = 0
extendc = 0

# convert to grayscale
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
# cv2.imwrite("output4_bin.png",gray);


# smooth the image to avoid noises
gray2 = cv2.medianBlur(gray,5)

# Apply adaptive threshold
thresh = cv2.adaptiveThreshold(gray2,255,1,1,11,2)
thresh_color = cv2.cvtColor(thresh,cv2.COLOR_GRAY2BGR)

# apply some dilation and erosion to join the gaps
# Edit required for enhancement in case of decimals

thresh = cv2.dilate(thresh,None,iterations = 3)
thresh = cv2.erode(thresh,None,iterations = 2)

# Find the contours
contours,hierarchy = cv2.findContours(thresh,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
# cv2.circle(img,img(x_origin,y_origin),img(x_origin+5,y_origin),(0,255,0),2)
          
# For each contour, find the bounding rectangle and draw it
# fx = open("output4_x.txt",'w')
# fy = open("output4_y.txt",'w')

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
          if left_mosty > x :
            left_mosty = x
          if top_mosty > y :  
            top_mosty = y
          if bottom_mosty < (y+h) :
            bottom_mosty = (y+h)
          if right_mosty < (x+w) : 
            right_mosty = x+w 
          # print x
          # print y
          # print w
          # print h 
          # fy.write(str(x)+" "+str(y)+" "+str(x+w)+" "+str(y+h)+"\n")

    if y > y_origin :
    	if (y+h) > y_origin :
          x = x - extendr
          y = y - extendc
          w = w + 2 * extendr
          h = h + 2 * extendc
          cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),1)
          cv2.rectangle(thresh_color,(x,y),(x+w,y+h),(0,255,0),1)
          if left_mostx > x :
            left_mostx = x
          if top_mostx > y :  
            top_mostx = y
          if bottom_mostx < (y+h) :
            bottom_mostx = (y+h)
          if right_mostx < (x+w) : 
            right_mostx = x+w 
          # print x
          # print y
          # print w
          # print h
          # fx.write(str(x)+" "+str(y)+" "+str(x+w)+" "+str(y+h)+"\n")

    if y < y_top :
      if (y+h) < y_top :
          x = x - extendr
          y = y - extendc
          w = w + 2 * extendr
          h = h + 2 * extendc
          cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),1)
          cv2.rectangle(thresh_color,(x,y),(x+w,y+h),(0,255,0),1)
          if left_mostx_top > x :
            left_mostx_top = x
          if top_mostx_top > y :  
            top_mostx_top = y
          if bottom_mostx_top < (y+h) :
            bottom_mostx_top = (y+h)
          if right_mostx_top < (x+w) : 
            right_mostx_top = x+w 
          # print x
          # print y
          # print w
          # print h
          # fx.write(str(x)+" "+str(y)+" "+str(x+w)+" "+str(y+h)+"\n")
# fx.close()
# fy.close()
# Finally show the image
crop_pad = 5
# if top_most > left_most :
#   crop_pad = left_most
# if left_most > top_most :
#   crop_pad = top_most  
# left_most = left_most - crop_pad
right_mosty = right_mosty + crop_pad
right_mostx = right_mostx + crop_pad

# top_most = top_most - crop_pad
# bottom_most = bottom_most + crop_pad
# cv2.imshow('img',img)
# cv2.namedWindow('img',cv2.WINDOW_NORMAL)
# cv2.imwrite("output4.png",img);
if bottom_mostx_top==0:
  left_mostx_top=0
  right_mostx_top=1
  top_mostx_top=0
  bottom_mostx_top=1
crop_yaxis = gray[top_mosty:bottom_mosty , left_mosty : right_mosty]
print top_mostx,bottom_mostx , left_mostx , right_mostx
crop_xaxis = gray[top_mostx:bottom_mostx , left_mostx : right_mostx]
crop_xaxis_top = gray[top_mostx_top:bottom_mostx_top , left_mostx_top : right_mostx_top]
# cv2.imshow('res',thresh_color)
# cv2.imshow('cropped y-axis',crop_yaxis)
# cv2.imshow('cropped x-axis',crop_xaxis)
cv2.imwrite("croppedtop.png",crop_xaxis_top);
cv2.imwrite("croppedy.png",crop_yaxis);
cv2.imwrite("croppedx.png",crop_xaxis);
# cv2.waitKey(0)
cv2.destroyAllWindows()
print "Hello"
