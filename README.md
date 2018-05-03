# OpenGL-Football-Game
A Football Game written in C++ using the OpenGL Library.
<p>
A Simple Football game that allows the user to change the position of the ball horizontally and decide the verical position of the ball to shoot and then shoot it. The wall has several hoops drawn which the user is supposed to hit with the football and earn points.<br>
The user has 10 attempts after which the game ends or the game ends if the user has hit all the hoops on the wall.<br>
The bounding limits are the walls. If the user shoots the ball above the wall, the ball resets back to it's original position and the user loses an attempt. The same happens if the user hits the wall at any place other than the hoop.<br>
A Timer Function is used to reload the frames every 25ms to ensure a smooth animation. The Ball rotates when the user shoots the ball to produce a proper illusion of a moving ball.<br>
Uses Perspective View to produce a 3D Environment. The Vertical and Horizontal Position Bar, and all the text are displayed using Orthographic Projection (2D).<br>
Basic Lighting used to produce the 3D Effect. The Lighting conditions change over time. The Ambient light becomes darker and then lighter. This loops throughout the game. <br>
4 Textured planes are used for the walls - Front, Left, Right and Back wall. A Plane is used for the textured grass surface. A Textured Ball is created using gluSphere. Flowers are drawn infront of the Front Wall. Plants are drawn infront of the Right and Left walls.<br>
8 Hoops are displayed on the front wall. Each has a score drawn within the hoop. The larger the hoop, the lower the score. The Hoop points are as follows:<br>
<ul>
    <li>Green Hoop : 10 Points</li>
    <li>Red Hoop : 20 Points</li>
    <li>Blue Hoop : 30 Points</li>
</ul>
<br>
</p>

<small>Uses Object Oriented Programming concepts</small>

<h3>Controls</h3>
<ul>
    <li>Left Arrow Key - Move the ball to the left horizontally</li>
    <li>Right Arrow Key - Move the ball to the right horizontally</li>
    <li>Up Arrow Key - Move the vertical shooting point up</li>
    <li>Down Arrow Key - Move the vertical shooting point down</li>
    <li>A - Rotate Camera horizontally: Left</li>
    <li>D - Rotate Camera horizontally: Right</li>
    <li>F - Able to obtain a closeup view. The Camera is position inches behind the ball</li>
    <li>T - Able to obtain a top view. The Camera moves far behind the ball bringing the entire field in view</li>
    <li>R - Reset the Game</li>
</ul>

<h3>Demo</h3>
<hr>
<img src='' alt=''>
