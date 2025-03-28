# Fractal Scaling Project - Barnsley Fern

This project visualizes the Barnsley Fern fractal using SDL2 and multi-threading to scale the computation for faster rendering. It allows for zooming and panning, enabling users to explore different parts of the fractal.

## Features

- **Barnsley Fern Fractal**: A recursive fractal based on simple affine transformations.
- **Multi-threading**: Utilizes multiple threads to divide the workload and improve performance.
- **Zooming and Panning**: Mouse-controlled zoom and drag functionality for interactive exploration.
- **SDL2 Rendering**: Uses SDL2 for window creation and rendering.

## Requirements

- SDL2 development libraries
- C compiler (e.g., GCC)
- pthread library

## Installation

### On Linux (Ubuntu example)
1. Install SDL2 and pthread libraries:

   ```bash
   sudo apt-get install libsdl2-dev
   sudo apt-get install pthreads

2. Clone the repository:

   ```bash
   git clone https://github.com/JohannLuke/Fractal-Scaling.git
   cd Fractal-Scaling

3. Compile and run:

   ```bash
   gcc -o barnsley_fern barnsley_fern.c -lSDL2 -lpthread
   ./barnsley_fern

## Usage

### Controls

- **Zoom In/Out**: Use the mouse scroll wheel to zoom in or out.
- **Pan**: Click and drag the mouse to move the fractal view around.
- **Reset View**: Press the `R` key to reset the fractal's scale and offset.
- **Iterations**: Upon running, the program will ask for the number of iterations. The default is 50,000 iterations if no input is given.

### Working

```c
Enter number of iterations (default 50000): 100000
