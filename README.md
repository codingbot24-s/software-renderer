The goal of this project is to understand how a basic graphics pipeline works internally by implementing the major rendering steps without relying on a GPU rendering API.

This project is intentionally small and educational rather than production-ready. It focuses on learning how pixels, geometry, transformations, rasterization, depth testing, textures, and lighting come together to produce a 3D image.

📺 Built Live on Stream
Built this software renderer live on stream from scratch 70% is in the stream, exploring the rendering pipeline step by step.
🎥 [Watch the stream on YouTube](https://www.youtube.com/@devNobu/playlists)

1. Framebuffer + Pixel Output

A software framebuffer for storing and directly manipulating individual pixels.

2. Line Rasterization

Rasterizes lines directly onto the framebuffer using software-based algorithms.

3. Triangle Rasterization

Fills and rasterizes triangles by determining which pixels lie inside the triangle.

4. Vector / Matrix Transformations

Implements vector and matrix operations for transforming 2D and 3D geometry.


5. Perspective Projection

Projects 3D geometry onto a 2D screen with realistic perspective.

6. Depth Buffer

Uses a depth buffer to determine which surfaces are visible when geometry overlaps.

7. Texture Mapping

Maps 2D textures onto 3D geometry using UV coordinates.

8. Perspective-Correct Interpolation

Correctly interpolates texture coordinates and other vertex attributes across projected triangles.

9. Basic Lighting

Applies basic lighting calculations to produce simple shading on 3D surfaces.

10. Load and Render a Real 3D Model

Loads and renders a real 3D model through the software rendering pipeline.

![beetle filled](assets/beetle.png)
![chest-textured](assets/chest-textured.png)
![chest-textured-phong-shaded](assets/chest-textured-phong-shaded.png)
![cube-fill](assets/cube-fill.png)
![cube-phong-shaded](assets/cube-phong-shaded.png)
![cube-textured](assets/cube-textured.png)
![gun-fill](assets/gun-fill.png)
![gun-metal-textured](assets/gun-metal-textured.png)
![gun-phong-shaded-textured](assets/gun-phong-shaded-textured.png)
![gun-phong-textured](assets/gun-textured.png)
![statue-fill](assets/statue-fill.png)
![statue](assets/statue-textured-metal.png)
![statue](assets/statue-textured-phong-shaded.png)



