#import cv2
#import os
from keras.models import load_model
#from scipy import misc


__all__ = ('classifier_model',)


class PersonClassifer(object):

	def __init__(self):
		'''try:
			self.model = load_model('peoples.h5')
		except OSError:
			os.chdir(os.path.dirname(os.path.abspath(__file__)))
			self.model = load_model('peoples.h5')

	def classify(self, img):
		img = cv2.resize(img, (96, 96))
		#cv2.imshow('image', img)
		#cv2.waitKey(0)
		img_cv = img.reshape(1, 1, 96, 96)
		pred = self.model.predict(img_cv)
		print('[PY CL] Prediction is {}'.format(pred))
		return pred'''


classifier_model = PersonClassifer()
