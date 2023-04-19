# Image Mirroring
Given a grayscale image, the code would load the image, mirror it along the horizontal axis and save it in mirrored_images folder.
This project uses cuda libraries to mirror a given grayscale .tiff image.

```src/```
This folder contains mirror_image.cpp containing the code that uses cuda NPP/FreeImage to load, mirror and save a grayscale image

```data/```
Includes original images and the mirrored ones at data/mirrored_images

## Running
you can simply run `bash run.sh` which will build,clean & run or call bin/src/mirror_image.exe -I {input_directory} -O {output_directory}

You can build using cmake
