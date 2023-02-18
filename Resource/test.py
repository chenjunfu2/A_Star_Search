from PIL import Image
 
f = open("o.txt", 'w+')
im = Image.open('12.png')
rgb_im = im.convert('RGB')
for j in range(40):
	for i in range(41):
		if(rgb_im.getpixel((i, j))!=(0,127,0)):
			print('0',file=f,end=',')
		else:
			print('1',file=f,end=',')
	print(file=f)
