#import cv2
import os
from keras.models import load_model
#from scipy import misc
import numpy as np
import PIL
from PIL import Image
import logging
logging.basicConfig(filename='classifier_log.txt', filemode='w', format = '%(asctime)s - %(name)s - %(levelname)s - %(message)s', level = logging.DEBUG)


__all__ = ('classifier_model',)


class PersonClassifer(object):

    def __init__(self):
        try:
            self.model = load_model('peoples.h5')
        except OSError:
            os.chdir(os.path.dirname(os.path.abspath(__file__)))
            self.model = load_model('peoples.h5')

    def classify(self, img):
        logging.debug(type(img))
        im = Image.fromarray(img)
        img_pil = im.resize((96, 96), PIL.Image.ANTIALIAS)
        img = np.array(img_pil)
        img_rs = img.reshape(1, 1, 96, 96)
        pred = self.model.predict(img_rs)
        pred_list = pred.tolist()[0]
        logging.debug('[PY CL] Prediction is {}'.format(pred))
        logging.debug('{}, {}'.format(pred_list, type(pred_list)))
        return pred_list.index(max(pred_list))


classifier_model = PersonClassifer()
