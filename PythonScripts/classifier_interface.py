import os
import sys


class MockClassifier(object):

	def classify(self, image):
		print('[PY|MOCK_CLASSIFIER] You passed {} <{}>'.format(image, type(image)))
		return (0, 0)


class ClassifierEmbeddingInterface(object):

	def __init__(self, environment_root=None):
		if not environment_root:
			environment_root = self.resolve_virtual_environment()

		if environment_root:
			self.activate_virtual_environment(environment_root)

		current_dir = os.path.dirname(os.path.realpath(__file__))
		sys.path.append(current_dir)

		try:
			from person_classifier import classifier_model
			self.classifier = classifier_model
		except ImportError:
			print('[PY|IMPORT_ERROR] Could not import classifier model')
			self.classifier = MockClassifier()

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

	def _debug_call(self, var):
		print('[PY|_DEBUG_CALL] You passed {} <{}>'.format(var, type(var)))

	def classify(self, image):
		_classify = getattr(self.classifier, 'classify', None)
		if _classify and callable(_classify):
			return _classify(image)
		else:
			raise AttributeError('Classifer must implement <classify> method.')


if __name__ == '__main__':
	_c_interface = ClassifierEmbeddingInterface()
	print(_c_interface.classifier)
