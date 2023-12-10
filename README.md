# Motor Engine UPC
Engine developed for the master Advanced Programming for AAA Video Games at UPC.  The goal of this release was to deliver a geometry viewer.

![image](https://github.com/acpuig/Engine/assets/83670087/a51c392f-5c7f-4f4d-9da4-643f88d52740)

## Funcionalities
The code sets up a user interface for configuring various aspects of the application.

Configuration Window:
 - The **Application** section allows configuring OpenGL settings.
 - The **Window** section provides configuration options related to the application window.
 - The **Information** section displays details about SDL version, software (OpenGL, Glew, GLSL), and hardware (GPU, brand).
 - The **Models** section handles the configuration of models, including textures.

Main Menu Bar :
- The **GitHub** menu includes items for navigating to the user's GitHub profile and documentation.
 - The **Quit** menu allows the user to choose between quitting or continuing the application.

## Camera Motion
Camera Motion Control:
 - The W and S keys control forward and backward movement, respectively.
 - The A and D keys control left and right lateral movement, respectively.
 - The Q and E keys control upward and downward movement, respectively.
 
Camera Reset:
 - The R key resets the camera orientation.
 
Camera Rotation:
 - The left and right arrow keys perform rotations around the Y-axis (yaw).

"Look at" operation:
 - The F key performs a "look at" operation.
 
Zoom (Zoom):
 - The mouse wheel controls the zoom. Scrolling the wheel up zooms out, and scrolling the wheel down zooms in.
