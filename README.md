# Grateful Dead Ray Tracer

<img width="442" height="472" alt="Grateful Dead ray traced scene" src="https://github.com/user-attachments/assets/691a4381-6511-48e3-9e02-241b72edb475" />

# Recursive Ray Tracer

### C++ | OpenGL | GLM | COSC363 — University of Canterbury

A recursive ray tracer built in C++, rendering a fully enclosed 3D scene with reflection, refraction, texturing, and anti-aliasing — extended from a university-provided rendering framework.

---

## Overview

A ray-traced scene of custom primitives (torus, cone, cylinder) alongside reflective, refractive, and textured spheres, lit by multiple point lights and a spotlight. Built on top of a base `SceneObject`/`Ray`/render-loop framework provided as course scaffolding; all primitive math, lighting effects, and rendering features beyond that base were independently researched, derived, and implemented.

---

## Features

### Custom Geometry

- **Torus** — intersection solved via a quartic equation, numerically approximated with the Durand–Kerner method, with a bounding-sphere check for early ray rejection
- **Cone** — intersection and normals derived from base radius/height trigonometry
- **Cylinder** — quadratic ray intersection with an added flat end-cap

### Physically-Based Light Transport

- **Refraction** — two-pass Snell's law implementation (entry + exit surfaces) via `glm::refract`, recursive up to a configurable bounce depth
- **Reflection** — mirrored surfaces via recursive ray tracing
- **Soft shadows for transparent/refractive objects** — shadow rays lighten realistically when passing through glass-like geometry, rather than producing hard black shadows

### Lighting

- **Two global point lights** with full shadow ray calculation
- **Spotlight** with configurable cutoff angle, blended with global illumination

### Texturing

- **Spherical UV mapping** implemented from scratch using `atan2`/`asin` projection math to wrap a 2D bitmap onto a sphere

### Image Quality

- **4x supersample anti-aliasing** — 4 jittered sub-rays per pixel averaged in color space, toggleable for dev-speed vs. quality

---

## Tech Stack

**Language:** C++
**Graphics:** OpenGL, GLUT, GLM
**Imaging:** DevIL (texture loading)
**Build:** g++ (GCC)
**Dev Environment:** Windows 11 + WSL2 Ubuntu

---

## Performance

~16 seconds at 400×400 resolution, `MAX_STEPS=3`, anti-aliasing disabled, on an AMD Ryzen 5 3500U.

---

## Build & Run

```bash
sudo apt install freeglut3-dev libglm-dev libgl1-mesa-dev -y

g++ RayTracer.cpp Sphere.cpp Plane.cpp Cylinder.cpp Cone.cpp Torus.cpp Ray.cpp \
    SceneObject.cpp TextureBMP.cpp -o raytracer -lGL -lGLU -lglut -lm

./raytracer
```

---

## About

Recursive ray tracer implementing custom torus/cone/cylinder geometry, refraction, spotlighting, texture mapping, and anti-aliasing, built on a course-provided rendering framework. COSC363, University of Canterbury.
