Fractal Scaling Project - Barnsley Fern
This project visualizes the Barnsley Fern fractal using SDL2 and multi-threading to scale the computation for faster rendering. It allows for zooming and panning, enabling users to explore different parts of the fractal.

Features
Barnsley Fern Fractal: A recursive fractal based on simple affine transformations.

Multi-threading: Utilizes multiple threads to divide the workload and improve performance.

Zooming and Panning: Mouse-controlled zoom and drag functionality for interactive exploration.

SDL2 Rendering: Uses SDL2 for window creation and rendering.

Requirements
SDL2 development libraries

C compiler (e.g., GCC)

pthread library

Installation
On Linux (Ubuntu example)
Install SDL2 and pthread libraries:

bash
Copy
sudo apt-get install libsdl2-dev
sudo apt-get install pthreads
Clone the repository:

bash
Copy
git clone https://github.com/yourusername/fractal-scaling.git
cd fractal-scaling
Compile the code:

bash
Copy
gcc -o barnsley_fern barnsley_fern.c -lSDL2 -lpthread
Run the application:

bash
Copy
./barnsley_fern
Usage
Controls
Zoom In/Out: Use the mouse scroll wheel to zoom in or out.

Pan: Click and drag the mouse to move the fractal view around.

Reset View: Press the R key to reset the fractal's scale and offset.

Iterations: Upon running, the program will ask for the number of iterations. The default is 50,000 iterations if no input is given.

Example Input
java
Copy
Enter number of iterations (default 50000): 100000
Mouse Actions
Left Mouse Button: Start dragging the view.

Mouse Wheel: Zoom in or out based on the direction of the scroll.

Key Actions
R Key: Reset the view to the default scale and offset.

Code Overview
The main program uses SDL2 for rendering and pthreads for multi-threaded fractal generation. The key components include:

barnsleyFernThread: Generates part of the fractal in a separate thread.

barnsleyFern: Initializes and manages threads to compute the fractal, renders it on an SDL surface, and presents the result.

main: Handles user input, events, and the SDL window loop.

ThreadData Structure
The ThreadData structure holds the data for each rendering thread, including:

scale: Scaling factor for the fractal.

offsetX, offsetY: Offsets for panning the view.

renderer: SDL2 renderer for drawing.

iter_per_thread: Number of iterations per thread.

seed: Random seed for thread.

render_mutex: Mutex to synchronize rendering across threads.

surface: SDL2 surface where the fractal is drawn.

FractalView Structure
The FractalView structure holds the scale and offsets for the fractal view.

Example Screenshot

License
This project is licensed under the MIT License - see the LICENSE file for details.

Acknowledgements
SDL2: Used for rendering and window management.

pthread: Used for multithreading to improve performance.
