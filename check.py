from time import perf_counter

import cv2


if __name__ == '__main__':
    # load the model
    net = cv2.dnn.readNetFromModelOptimizer('person-detection-0200.xml', 'person-detection-0200.bin')
    net.setPreferableBackend(cv2.dnn.DNN_BACKEND_INFERENCE_ENGINE)

    # prepare input image
    image = cv2.imread('person.jpg')
    blob = cv2.dnn.blobFromImage(image=image, size=(256, 256))

    # do inference
    net.setInput(blob)
    tic = perf_counter()
    outputs = net.forward()
    toc = perf_counter()
    print(f'inference time = {toc - tic:.4f} ms')

    assert outputs.shape == (1, 1, 200, 7)
