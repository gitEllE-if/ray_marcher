# 🔵 Raymarching Distance Fields

Ray marching algorithm, used in combination with a special kind of function called **SDF - signed distance functions**, can create cool things in real time.

There aren’t using polygonal meshes and are instead using mathematical equations and the scene is drawn on a single quad that covers the camera’s field of vision.

The C code is boilerplate to run the OpenGL. The raymarching magic occurs in **fragment shader** wich determine the screen pixel colours.

![screenshot](screenshot/rm.gif)

## 🟣 Signed distance functions

SDF is a function that takes in a point as input and returns the shortest distance from that point to the surface any object in the scene. SDF additionally returns a negative number if the input point is inside of an object.

So our SDF, for a sphere centered at the origin with radius 1, looks like this:
```
f(x,y,z) = sqrt(x^2 + y^2 + z^2) - 1

f(1, 0, 0)   = 0      <- on the surface
f(0, 0, 0.5) = -0.5   <- inside
f(0, 3, 0)   = 2      <- outside
```
When we’re working in GLSL shader code, formulas like this will be vectorized:
```
  _      _
f(p) = ||p|| - 1
```
Which, in GLSL, translates to this:
```
float sphere_SDF(vec3 p)
{
	return length(p) - 1.0;
}
```
More amazing SDFs you can find [here](https://iquilezles.org/www/articles/distfunctions/distfunctions.htm)

## 🔵 Raymarching

Raymarching is the alternative technique to Raytracing.
Instead of casting out a ray and attempting to intersect a line with an object, raymarching marches along a ray based on the scene's SDF until the distance output from the function is < 0.
We start at the camera, and move a point along the view ray, step by step. At each step, we check “Does the SDF evaluate to a negative number at this point?“. If it does, we’re hit something, else we keep going up to some maximum number of steps along the ray.

The lighting model used is the [Blinn-Phong lighting model](https://habr.com/ru/post/353054/)

## Basics

- GCC
- OpenGL
- GLSL 3.3

## Dependencies

- glew 2.1.0
- glfw 3.3.2

## 🟢 Run

```
git clone https://github.com/gitEllE-if/ray_marcher.git rayMarch

cd rayMarch

make

./RM
```
The program was tested and run on MacOS (Mojave, HighSierra, Catalina)


![screenshot](screenshot/rm.png)

## 🟣 Shadertoy

Shadertoy is a tool that lets you experiment shaders without needing to write any OpenGL/WebGL boilerplate. You just write a fragment shader and watch how it work.
It can be accessed [here](https://www.shadertoy.com/view/WssfzS)
