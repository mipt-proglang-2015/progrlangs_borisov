__author__ = 'b.alexei'

import numpy as np
from datetime import datetime
import compareimg

def cut_img(img, size_of_frag):
    """
    Cut img to microfragments.
    """
    result = list()
    width = len(img[0])
    height = len(img)
    print ("Current image\n")
    print ("Width: ", width, " Height: ", height, "\n")
    img_clone = np.empty_like(img)
    img_clone[:] = img

    for y in range(0, height, size_of_frag):
        for x in range(0, width, size_of_frag):
            sub_img = []
            if x + size_of_frag <= width and y + size_of_frag <= height:
                sub_img = img[y:y+size_of_frag, x:x+size_of_frag]
            elif x + size_of_frag > width and y + size_of_frag > height:
                sub_img = img[y:y+(height - y), x:x+(width - x)]
            elif x + size_of_frag > width:
                sub_img = img[y:y+size_of_frag, x:x+(width - x)]
            elif y + size_of_frag > height:
                sub_img = img[y:y+(height - y), x:x+size_of_frag]
            result.append(sub_img)
    return result


def generate_random_image(h, w):
    imarray = np.random.rand(h, w, 3) * 255
    return imarray


def mse(_img1, _img2,):
    # image grey-scaling
    gray1 = _img1
    gray2 = _img2
    diff = 0.0
    for y in range(len(gray2)):
        for x in range(len(gray2[0])):
            for ch in range(0, 3):
                    diff += float((int(gray1[y][x][ch]) - int(gray2[y][x][ch])) * (int(gray1[y][x][ch]) - int(gray2[y][x][ch])))

    return float((float(diff)) / float(int(len(_img1) * int(len(_img1[0])))) )


if __name__ == "__main__":
    r_img = generate_random_image(600, 300)
    img_grid = cut_img(r_img, 100)
    print ("Amount of fragments: ", str(len(img_grid)))

    python_start = datetime.now()

    diff_py = list()
    for i in range(0, len(img_grid)):
        for j in range(0, len(img_grid)):
            diff_py.append(mse(img_grid[i], img_grid[j]))

    python_end = datetime.now()
    python_time = python_end - python_start
    print("Python calculations time: " + (str(python_time)))

    c_start = datetime.now()

    diff_c = list()
    for i in range(0, len(img_grid)):
        for j in range(0, len(img_grid)):
            compareimg.diff_frag(img_grid[i].tolist(), img_grid[j].tolist())

    c_end = datetime.now()
    c_time = c_end - c_start
    print("Python calculations time: " + (str(c_time)))
