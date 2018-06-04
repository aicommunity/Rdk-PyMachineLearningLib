#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import os


class VirtualEnvInitializer(object):

	@staticmethod
	def resolve_virtual_environment(override=None):
		"""
		Fetch the virtual environment path in the
       	process' environment or use an override.
		"""
		path = os.getenv('VIRTUAL_ENV')
		if override:
			path = os.path.join(os.getcwd(), override)
		return path

	@staticmethod
	def activate_virtual_environment(environment_root):
		"""Configures the virtual environment starting at ``environment_root``."""
		activate_script = os.path.join(
			environment_root, 'bin', 'activate_this.py')
		globals = {
			'__file__': activate_script
		}
		exec(compile(open(activate_script, "rb").read(), activate_script, 'exec'), globals) 


class IntegrationInterface(object):

	def __init__(self):
		"""
		При внедрении этого модуля в С++ программу сначала загружается __main__ - модуль с
		модулями питона, которые установлены глобально.

		Чтобы иметь возможность подключать виртуальные среды с требуемым набором пакетов, в 
		этой функции импортируются требуемые пакеты через global

		Пример:
			global tf
			import tensorflow as tf
		"""
		global misc, load_model

		from keras.models import load_model
		from scipy import misc

	def predict_with_keras(self):
		dir_path = os.path.dirname(os.path.realpath(__file__))
		input_file = dir_path + '/205.jpg'
		model = load_model(dir_path + '/peoples.h5')
		image = misc.imread(input_file, flatten=True)

		img = image.reshape(1, 1, 96, 96)

		pred = model.predict(img)
		print('[PY] Prediction = {} ({})'.format(pred, type(pred)))
