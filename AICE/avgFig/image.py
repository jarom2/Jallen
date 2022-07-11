from PIL import Image as im
import numpy as np


#avg_4 = np.load("avg_4.npy")
#avg_10 = np.load("avg_10.npy")
#avg_32 = np.load("avg_32.npy")
#avg_C = np.load("avg_C.npy")
#avg_sm2 = np.load("avg_sm2.npy")
#avg_sm3 = np.load("avg_sm3.npy")
#avg_sm4 = np.load("avg_sm4.npy")
#avg_all = np.load("avg_all.npy")
base = np.load("mnist_200.npy")

#img_4 = im.fromarray(avg_4*255)
#img_4 = img_4.convert("L")
#img_4.save("img_4.jpg")

#img_10 = im.fromarray(avg_10*255)
#img_10 = img_10.convert("L")
#img_10.save("img_10.jpg")

#img_32 = im.fromarray(avg_32*255)
#img_32 = img_32.convert("L")
#img_32.save("img_32.jpg")

#img_C = im.fromarray(avg_C*255)
#img_C = img_C.convert("L")
#img_C.save("img_C.jpg")

#img_sm2 = im.fromarray(avg_sm2*255)
#img_sm2 = img_sm2.convert("L")
#img_sm2.save("img_sm2.jpg")

#img_sm3 = im.fromarray(avg_sm3*255)
#img_sm3 = img_sm3.convert("L")
#img_sm3.save("img_sm3.jpg")

#img_sm4 = im.fromarray(avg_sm4*255)
#img_sm4 = img_sm4.convert("L")
#img_sm4.save("img_sm4.jpg")  

#img_all = im.fromarray(avg_all*255)
#img_all = img_all.convert("L")
#img_all.save("img_all.jpg")  

img_base = im.fromarray(base*255)
img_base = img_base.convert("L")
img_base.save("img_base.jpg")
