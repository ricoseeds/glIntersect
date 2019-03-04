
###### Implementation using Opengl 3.3 and C++
Author : Argha Chakraborty

###### Internet-enabled Skills Training Platform for neurosurgical training
https://ieeexplore.ieee.org/document/7098726

The medical and technological sciences have grown rapidly over the last decade. Delivering these advances to the medical students and practitioners has been a challenge in developing nations like India. The quality of training of residents during post-graduation greatly affects the health care system. Interaction between the residents and the training faculty is pertinent for good quality of training. Specialties like neurosurgery deal with critical patients with high morbidity. Neuroradiology based on Computed Tomography (CT) and Magnetic Resonance Imaging (MRI) scans play vital role in diagnosis and treatment. In this paper we propose a framework for annotation based interactive quizzes for neuroradiology and its integration with an e-learning platform to assist the trainee to improve diagnostic skills and pre-surgical planning capabilities.

###### Compile and run in MAC
```
g++ src/main.cpp src/ShaderProgram.cpp src/Texture2D.cpp  -framework OpenGl -I/usr/local/include -lglfw -lGLEW && a.out
```

###### Compile and run in Linux
```
g++ src/main.cpp src/ShaderProgram.cpp src/Texture2D.cpp -std=c++0x -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -lGLEW && ./a.out
```
