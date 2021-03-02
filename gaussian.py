import numpy as np
from scipy import ndimage
from PIL import Image
import utils


image_name = "blood_poisson"

img = utils.to_array_image(image_name)
denoised = ndimage.gaussian_filter(img, sigma=(2, 2, 0))
utils.save_array_image(denoised, image_name + "_denoised")
