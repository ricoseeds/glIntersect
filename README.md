
###### Implementation using Opengl 3.3 and C++
Author : Argha Chakraborty

###### Internet-enabled Skills Training Platform for neurosurgical training
https://ieeexplore.ieee.org/document/7098726

The medical and technological sciences have grown rapidly over the last decade. Delivering these advances to the medical students and practitioners has been a challenge in developing nations like India. The quality of training of residents during post-graduation greatly affects the health care system. Interaction between the residents and the training faculty is pertinent for good quality of training. Specialties like neurosurgery deal with critical patients with high morbidity. Neuroradiology based on Computed Tomography (CT) and Magnetic Resonance Imaging (MRI) scans play vital role in diagnosis and treatment. In this paper we propose a framework for annotation based interactive quizzes for neuroradiology and its integration with an e-learning platform to assist the trainee to improve diagnostic skills and pre-surgical planning capabilities.

###### Compile and run in MAC
```
g++ src/main.cpp src/ShaderProgram.cpp src/Texture2D.cpp -o run -std=c++11 -framework OpenGl -I/usr/local/include -lglfw -lGLEW -I/usr/local/opt/freetype/include/freetype2  -L/usr/local/lib -lfreetype -g
```

###### Compile and run in Linux
```
g++ src/main.cpp src/ShaderProgram.cpp src/Texture2D.cpp -std=c++0x -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -lGLEW -I/usr/include/freetype2 -L/usr/local/lib -lfreetype 
```
###### USE
1. draw truth region and press 'D' when done.
2. draw your poly and press 'C' to see the score
3. Optional : To check the correct region press 'P'
4. Optional : To hide the truth region press 'K'
5. Optional : To hide your marked region press 'L'


###### DEMO
![upload](https://user-images.githubusercontent.com/17334660/54269481-75367600-45a3-11e9-9207-b4053bb041fb.gif)
