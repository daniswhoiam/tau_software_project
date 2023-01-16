from setuptools import setup, Extension

module = Extension("mykmeanssp", sources=["kmeansmodule.c"])
setup(
    name="mykmeanssp",
    version="1.0.0",
    description="K means algorithm",
    ext_modules=[module],
)
