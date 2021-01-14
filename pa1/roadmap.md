# Roadmap

## Todo

- [x] Naive z-buffer
  - use 2D bounding boxes (AABB) to check if a pixel is inside a triangle's
    projection
- [x] Hierarchical z-buffer
  - [ ] Optimization: binary lifting for LCA
- [x] OcTree construction
  - [ ] Add `rendered` boolean flag to class `Triangle` to avoid rendering a
        triangle more than once.
- [ ] Scan conversion

## Steps

- [x] Transformation
  1. - [x] create a simple triangle
  2. - [x] load the triangle into a Scene object
  3. - [x] render the scene
- [x] Barycentric coordinates
- [x] Normal fragment shader
- [ ] Read obj's normal values
- [x] Hierarchical z-buffer value checking

## Optional

- [ ] omp
- [ ] Remove dependency `glm`
- [x] Add class `timer` for benchmarking

> Author: Blurgy <gy@blurgy.xyz>
> Date:   Nov 23 2020, 15:18 [CST]
