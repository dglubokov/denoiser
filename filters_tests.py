import numpy as np
from skimage import restoration, color
from scipy.signal import convolve2d
from PIL import Image

import utils

orig_img_name = "blood"
gauss_img_name = orig_img_name + "_gauss"
poisson_img_name = orig_img_name + "_poisson"

gauss_im = utils.to_array_image(gauss_img_name)
poisson_img = utils.to_array_image(poisson_img_name)


# non-local means
denoised_gauss = restoration.denoise_nl_means(gauss_im, multichannel=True)
utils.save_array_image(denoised_gauss, gauss_img_name + "_nl_means")

denoised_poisson = restoration.denoise_nl_means(poisson_img, multichannel=True)
utils.save_array_image(denoised_gauss, poisson_img_name + "_nl_means")


# wiener
gray_gauss = color.rgb2gray(gauss_im)
# psf = np.ones((5, 5)) / 25
# gray_gauss = convolve2d(gray_gauss, psf, 'same')
# gray_gauss += 0.1 * gray_gauss.std() * np.random.standard_normal(gray_gauss.shape)
# denoised_gauss = restoration.wiener(gray_gauss, psf, 1100)
# print(color.gray2rgb(gauss_im))
utils.save_array_image(color.gray2rgb(gray_gauss), gauss_img_name + "_wiener")

# denoised_poisson = restoration.denoise_nl_means(poisson_img, multichannel=True)
# utils.save_array_image(denoised_gauss, poisson_img_name + "_wiener")


# median
# Bilateral
# BM3D
# wavelet
# denoise_tv_chambolle
# denoise_tv_bregman
