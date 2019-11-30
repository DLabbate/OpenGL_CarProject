COMP 371 Final Project
Written by Domenic Labbate (40063296)
Note that the controls use different keys from project outline!

REFERENCES
- This assignment uses camera code based off of lab3. Creating a camera class along with movement of camera was inspired from https://learnopengl.com/Getting-started/Camera. 
- Assignment uses skeleton code from the laboratory
- Shader program is similar to lab1, except it uses ifstream to read information from shader files (.glsl)
- The use of a uniform to change colour was inspired by https://learnopengl.com/Getting-started/Shaders
- ***Lighting inspired by https://learnopengl.com/Lighting/Basic-Lighting
- ***Shadows inspired by https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping. Moreover, this program uses the same PCF algorithm (with slight modifications)
- Framebuffer callback inspired by https://learnopengl.com/Getting-started/Hello-Window (though it was modified to maintain the same aspect ratio).
- Smoke particles was inspired by Lab07. The code for the "Quad" class is similar (with some modifications). Moreover, the use of billboards was also inspired from lab07.

Final Project Features - BUMPER CARS
- Multiple cars on grid with random movement (user can choose between manual movement for his/her car or random movement)
- Collision detection between each car. When a car is hit, it translates a certain distance in the appropriate direction
- Motion blur effect when a car is hit!

NEW CONTROLS (from Final Project version 1.1)
- 4 : toggles between first person/third person view of car
- PgUp : toggles headlights and taillights on/off
- Backspace : toggles between different textures for the border surrounding the grid
- Tab : toggles between camera roaming freely or following car
- Enter : toggles between manual control of the car or random movement
- Del : toggles movement of each car on/off

NEW CONTROLS (from Assignment 2):
N : Enables grass ground (texture)
M : Enables regular grid
, : Enables regular car colour
. : Enables chrome car surface (phong model)
- : Disables Shadows
+ : Enables Shadows
7/8 : Turn wheels left/right

CONTROLS (from Assignment 1):
- Home : resets camera and world orientation to default
- W/S : moves car forwards/backwards (relative to where the car is facing!)
- A/D : moves car left/right (relative to where the car is facing!)
- Q/E : increases/decreases angle of car with respect to y-axis
- U/J : increases/decreases scale of the car (minimum scale of the car is set to 0.25)
- Z/X : shifts world orientation (around y-axis)
- C/V : shifts world orientation (around x-axis)
- P : changes rendering mode to points
- L : changes rendering mode to lines
- T : changes rendering mode to triangles
- Space : positions car at a random point on the grid
- Left/Right keys : moves camera left/right (relative to where the camera is facing)
- Up/Down keys: moves camera up/down
- 1/2 : Zooms in/out
- Mouse left (while it is held down) : tracks x position of mouse to pan
- Mouse right (while it is held down) : tracks y position of mouse to tilt

ADDITIONAL FEATURES:
- 3 : resets car to origin, and also resets the scale and angle of car
- 9/0 : increases/decreases the speed of the wheels
- R/F : look up/down with camera
- 4 : Camera follows the car. Note that the world orientation should be reset before using this option! The option to follow the car can be togged on/off (by pressing '4'). Moreover, the camera/world orientation can be reset by pressing the 'Home' button at any time.

