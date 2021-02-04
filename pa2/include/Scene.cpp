#include "Scene.hpp"

Scene::Scene() {}
Scene::Scene(tinyobj::ObjReader const &loader) : root{nullptr} {
    auto const &attrib = loader.GetAttrib();
    for (tinyobj::shape_t const &shape : loader.GetShapes()) {
        std::size_t index_offset = 0;
        // Loop over faces in shape
        for (std::size_t fi = 0; fi < shape.mesh.num_face_vertices.size();
             ++fi) {
            std::size_t numverts = shape.mesh.num_face_vertices[fi];
            if (numverts != 3) {
                errorm("Non-triangular mesh is unhandled\n");
            }
            std::array<vec3, 3> vtx, nor;
            std::array<vec2, 3> tex;
            for (std::size_t v = 0; v < numverts; ++v) {
                // Access vertices
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                flt vx = attrib.vertices[3 * idx.vertex_index + 0];
                flt vy = attrib.vertices[3 * idx.vertex_index + 1];
                flt vz = attrib.vertices[3 * idx.vertex_index + 2];
                flt nx = attrib.normals[3 * idx.vertex_index + 0];
                flt ny = attrib.normals[3 * idx.vertex_index + 1];
                flt nz = attrib.normals[3 * idx.vertex_index + 2];
                flt tu = attrib.texcoords[2 * idx.vertex_index + 0];
                flt tv = attrib.texcoords[2 * idx.vertex_index + 1];
                vtx[v] = vec3{vx, vy, vz};
                nor[v] = vec3{nx, ny, nz};
                tex[v] = vec2{tu, tv};
            }
            index_offset += numverts;
            int      matid = shape.mesh.material_ids[fi];
            Triangle newtri{vtx, nor, tex};
            newtri.set_material(loader.GetMaterials()[matid]);
            this->orig_tris.push_back(newtri);
        }
    }
}

/* public */

void Scene::to_camera_space(Camera const &cam) {
    this->tris.clear();
    this->lights.clear();
    this->area_of_lights = 0;
    for (Triangle const &t : this->orig_tris) {
        Triangle vt = t * cam.view_matrix();
        this->tris.push_back(vt);
        if (vt.material()->has_emission) {
            this->lights.push_back(vt);
            this->area_of_lights += vt.area();
        }
    }
}

void Scene::build_BVH() {
    if (this->root) {
        delete this->root;
    }
    this->root = new BVHNode();
    this->root->build(this->triangles(), nullptr);
}

Intersection Scene::sample_light(Intersection const &isect) const {
    Intersection ret;
    if (this->lights.size() == 0) {
        return ret;
    }

    vec3            light_pos;
    Triangle const *light_source;

    flt threshold = uniform() * this->area_of_lights;
    flt acc_area  = 0;
    for (Triangle const &t : this->lights) {
        acc_area += t.area();
        if (acc_area >= threshold) {
            light_pos    = t.sample();
            light_source = &t;
            break;
        }
    }
    vec3 npos = isect.position + epsilon * isect.normal;
    Ray  checker_ray{npos, light_pos - npos};
    ret = this->intersect(checker_ray);
    if (!equal(ret.distance, glm::length(light_pos - isect.position))) {
        // The sampled light ray is occluded.
        ret.occurred = false;
    } else if (sign(glm::dot(ret.normal, checker_ray.direction)) >= 0) {
        // The sampled light ray shoots the other way.
        ret.occurred = false;
    } else {
        // The sampled light ray is not occluded.
        /* Do nothing */
    }
    return ret;
}

vec3 Scene::shoot(Ray const &ray, flt const &rr, int const &bounce) const {
    vec3 l_dir{0}, l_indir{0};

    Intersection isect = this->intersect(ray);

    if (isect) {
        if (isect.tri->material()->has_emission) {
            if (bounce || sign(glm::dot(isect.normal, ray.direction)) >= 0) {
                return vec3{0};
            } else {
                return isect.tri->material()->emission /
                       glm::dot(ray.origin - isect.position,
                                ray.origin - isect.position);
            }
        }

        vec3 wo = -ray.direction;

        Intersection light_sample = this->sample_light(isect);
        if (light_sample) { // Calculate direct illumination
            vec3 wi = glm::normalize(light_sample.position - isect.position);
            flt  pdf_light = 1.0 / light_sample.tri->area();
            vec3 emission  = light_sample.tri->material()->emission;
            vec3 fr        = isect.tri->material()->fr(wi, wo, isect.normal);

            l_dir = emission * fr * glm::dot(wi, isect.normal) *
                    glm::dot(-wi, light_sample.normal) /
                    glm::dot(light_sample.position - isect.position,
                             light_sample.position - isect.position) /
                    pdf_light;
        }

        if (uniform() < rr) { // Russian roulette
            vec3 wi = isect.tri->material()->sample(wo, isect.normal);
            Ray  nray{isect.position, wi};
            vec3 fr  = isect.tri->material()->fr(wi, wo, isect.normal);
            flt  pdf = isect.tri->material()->pdf(wi, wo, isect.normal);
            l_indir  = this->shoot(nray, rr, bounce + 1) * fr *
                      glm::dot(wi, isect.normal) / pdf / rr;
        }
    }

    return l_dir + l_indir;
}

std::vector<Triangle> const &Scene::triangles() const { return this->tris; }

/* Private */

Intersection Scene::intersect(Ray const &ray) const {
    // /* Naively loop over all primitives */
    // Intersection ret;
    // for (Triangle const &t : this->triangles()) {
    // Intersection isect = ray.intersect(t);
    // if (isect.occurred && isect.distance < ret.distance) {
    // ret = isect;
    // }
    // }
    // return ret;
    /* Use bounding volume hierarchy */
    return this->root->intersect(ray);
}
// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 31 2021, 21:13 [CST]
