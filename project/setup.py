from setuptools import setup, Extension

module = Extension("myspkmeanssp", sources=["spkmeansmodule.c", "spkmeans.c"])
setup(
    name="myspkmeanssp",
    version="1.0.0",
    description="Extended K means algorithm",
    ext_modules=[module],
)
