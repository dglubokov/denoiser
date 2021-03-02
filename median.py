from skimage import restoration
import numpy as np
import utils

from scipy.signal import convolve2d
from skimage import data, img_as_float, img_as_ubyte
from skimage.util import random_noise

image_name = "blood"

img = utils.to_array_image(image_name)
img = img_as_float(img)

print(restoration.estimate_sigma(img, multichannel=True))

# original = img_as_float(data.chelsea()[100:250, 50:300])

# sigma = 0.155
# noisy = random_noise(original, var=sigma**2)
# denoised = restoration.denoise_tv_chambolle(img, weight=0.1, multichannel=True)
# print(restoration.estimate_sigma(denoised, multichannel=True))
# denoised = img_as_ubyte(denoised)
# utils.save_array_image(denoised, "test" + "_denoised")
