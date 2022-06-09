from pybind11.setup_helpers import Pybind11Extension
from setuptools import setup
from setuptools_cpp import ExtensionBuilder

__version__ = "0.0.1"

ext_modules = [
    Pybind11Extension("board",
                      ["src/bindings.cpp"],),
    Pybind11Extension("myrandoms",
                      ["src/bindings.cpp"],),
]


setup(
    name="coremodule",
    version=__version__,
    install_requires=['pybind11', 'pytest', 'pytest-cpp', 'setuptools_cpp',
                      'pygame', 'tk', 'pygame-menu'],
    author="Lukasz Szarejko & Kuba Forczek",
    author_email="",
    url="",
    description="Game Larger Than Life implemented in C++ and Python",
    long_description="",
    ext_modules=ext_modules,
    extras_require={"test": "pytest"},
    cmdclass=dict(build_ext=ExtensionBuilder),
    zip_safe=False,
    python_requires=">=3.9",
)
