# Automatic-Fan-Control-based-on-Humidity-Values
ML Control of Bathroom exhaust fan taking humidity values and outputting predicted Fan State (1/0, ON or OFF)

Upload the Jupyter file via Google Collab or click on the following link: https://drive.google.com/file/d/1ozxZJ222mv-oaq5fUqMSDMBaXcmbjPAI/view?usp=sharing

This file imports the trainig data and will export a CPP file that can be used as a header for an Arduino Sketch.

An Arduino ino file is included that uses a simple engineered feature extraction model, which does not use TensorFlow Light

The scitree file displays the projects descision tree code

The link to the Edge Impulse: https://studio.edgeimpulse.com/public/43415/latest




Credits

This project uses sample code from the following projects:

[1]	Github,  “TensorFlow Lite for Microcontrollers”. Accessed on: July 20, 2020. [Online]. Available: https://github.com/tensorflow/tflite-micro/tree/main/tensorflow/lite/micro/examples/hello_world

[2]	Github,  “Train a Simple TensorFlow Lite for Microcontrollers model”. Accessed on: July 25, 2020. [Online]. Available: https://github.com/tensorflow/tflitemicro/blob/main/tensorflow/lite/micro/examples/hello_world/train/train_hello_world_model.ipynb

[3]	Sandeep Mistry & Dominic Pajak,  “How-to Get Started with Machine Learning on Arduino”. Accessed on: July 26, 2020. [Online]. Available: https://blog.tensorflow.org/2019/11/how-to-get-started-with-machine.html

[4]	Shawn Hymel,  “Intro to TinyML Part 2: Deploying a TensorFlow Lite Model to Arduino”. Accessed on: July 26, 2020. [Online]. Available: https://www.digikey.com/en/maker/projects/intro-to-tinyml-part-2-deploying-a-tensorflow-lite-model-to-arduino/59bf2d67256f4b40900a3fa670c14330


USeful Links

A video showing an example for TensorFlow Lite implementation
https://www.youtube.com/watch?v=dU01M61RW8s&t=42s


Link to download Netron, a TensorFlow Model Viewer
https://github.com/lutzroeder/netron
