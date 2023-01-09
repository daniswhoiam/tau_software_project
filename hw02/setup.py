from distutils.core import setup, Extension


def main():
    setup(
        name="mykmeanssp",
        version="1.0.0",
        description="K means algorithm",
        ext_modules=[Extension("mykmeanssp", ["kmeansmodule.c"])],
    )


if __name__ == "__main__":
    main()
