# PongOpenGL
juego basico del pong usando openGl y c++ .<br>

![GitHub Logo](/imagenes/window.png)

### compilar en diferentes sistemas
el projecto esta configurado para usar xcode y c++ pero se pude copiar el archivo PongOpenGl/main.cpp y usar cualquier IDE para compilarlo
solo se debe cambiar los include comentados en main.cpp segun sea el sistema Winodws, ubuntu o mac.<br>
Nota:es necesario tener instalado openGl. en tu sistema
<br>

### compile Ubuntu
1.-   uncommit  include Ubuntu and comment Windows and Macos
      <br>
      #include <GL/glut.h>  //Ubuntu 
      <br>
2.-    g++ pong.cpp -o pong -lGL -lGLU -lglut
      <br>
3.-   ./pong
      <br>


### compile MacOs
clone project and run

### compile Windows
