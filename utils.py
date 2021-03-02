import numpy as np
from PIL import Image


def save_array_image(array, image_name):
    pil_img = Image.fromarray(np.uint8(array)).convert('RGB')
    pil_img.save('./imgs/' + image_name + '.jpg')


def add_noise(image, image_name):
    row, col, ch = image.shape

    # Gauss
    mean = 0
    var = 0.5
    sigma = var**0.5
    gauss = np.random.normal(mean, sigma, (row, col, ch))
    gauss = gauss.reshape(row, col, ch)
    noisy = image + gauss * 30
    save_array_image(noisy, image_name + '_gauss')

    # Poisson
    noisy = np.random.poisson(image)
    save_array_image(noisy, image_name + '_poisson')


def to_array_image(image_name):
    full_image_name = './imgs/' + image_name + '.jpg'
    img = Image.open(full_image_name)
    img = np.asarray(img)
    return img


# image_name = input(".jpg image name: ")
# img = to_array_image(image_name)
# add_noise(img, image_name)
