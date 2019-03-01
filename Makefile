all:
	g++ src/main.cpp src/ShaderProgram.cpp src/Texture2D.cpp -o run -framework OpenGl -I/usr/local/include -lglfw -lGLEW 