**** README File ****
Final Project <2D to 3D Face Reconstruction> Name: Sunoh Yoo
OS: MacOSX
Libraries: C++ (OpenGL, OpenCV, Dlib)
- In this final project, I used morphing target animation to implement a system to reconstruct 2D webcam frame images to 3D facial mesh
- Project directory contains the project containing source and header files
- Video directory contains a gif video file (animation.gif) and jpg frame images.
<How to Run>
- You should have dlib directory inside the project directory
In MacOS, under the Project directory, execute Cmake to build the project. To run the project, shape_predictor_68_face_landmarks.dat (Point data for the landmark detection) is required.
(You should face the webcam when starting the program because it gets the first frame as a base facial landmark points.)
<Main Modules>
1. jello.h, jello.cpp
: Those contain main function, contains gl initialization so that it initialize color with lighting information, camera position, etc. It also contains functions to create mesh objects and draw meshes, and it also contains functions to get frames from webcam and display those as a texture mapping.
2. Face.h, Face.cpp
: The Face class contains vertices and their manipulating functions. When creating an object with an obj file, it translates the mesh file and stores vertices. It calculates variations its vertices with a base mesh, and it also stores the variation vertices in it.
<Acomplished>
- I created Face class to translate obj files and store vertices.
- I get webcam frame images and show those images with OpenGL texture mapping. - I implemented linear interpolation for the blend shapes with target meshes.
- The program can move mouth and eyes (slightly) according to landmark point data from webcam images.
