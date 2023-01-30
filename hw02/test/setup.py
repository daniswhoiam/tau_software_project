from setuptools import setup, Extension

module = Extension("mysummodule", sources=["test.c"])
setup(
    name="mysummodule",
    version="1.0.0",
    description="sum algorithm",
    ext_modules=[module],
)
