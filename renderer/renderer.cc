/*
 * Author: Dino Wernli
 */

#include "renderer.h"

#include <memory>

#include "proto/configuration.pb.h"
#include "renderer/intersection_data.h"
#include "renderer/sampler/sample.h"
#include "renderer/sampler/sampler.h"
#include "renderer/sampler/scanline_sampler.h"
#include "renderer/shader/phong_shader.h"
#include "renderer/shader/shader.h"
#include "renderer/updatable.h"
#include "scene/camera.h"
#include "scene/point_light.h"
#include "scene/scene.h"
#include "util/ray.h"

class Light;

Renderer::Renderer(Scene* scene, Sampler* sampler, Shader* shader)
    : scene_(scene), sampler_(sampler), shader_(shader) {
}

Renderer::~Renderer() {
}

void Renderer::AddListener(Updatable* listener) {
  for (auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    if (listener == it->get()) {
      return;
    }
  }
  listeners_.push_back(std::unique_ptr<Updatable>(listener));
}

void Renderer::Start() {
  // We can skip the NULL-check for camera because we may assume that the
  // sampler can handle this. If camera is NULL, the loop below will terminate
  // instantly.
  const Camera* camera = &scene_->camera();
  sampler_->Init(camera);

  Sample sample;
  while(sampler_->NextSample(&sample)) {
    Ray ray = camera->GenerateRay(sample);
    sample.set_color(TraceColor(ray));
    sampler_->AcceptSample(sample);
  }

  for(auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    it->get()->Update(sampler_->image());
  }
}

Color3 Renderer::TraceColor(const Ray& ray) {
  IntersectionData data;
  bool intersects = scene_->Intersect(ray, &data);
  if (intersects) {
    return shader_->Shade(data);
  } else {
    // TODO(dinow): Return background color of scene.
    return Color3(0.5, 0.5, 0.5);
  }
}

// static
Renderer* Renderer::FromConfig(const raytracer::Configuration& config) {
  // TODO(dinow): Parse config and pass corresponding objects to new renderer.
  Scene* scene = new Scene();
  scene->set_camera(new Camera(Point3(0, 0, 0), Vector3(0, 0, 1),
                               Vector3(0, 1, 0), PI / 6.0, 300, 500));
  Light* light = new PointLight(Point3(0, 0, 0), Color3(0, 0, 0));
  scene->AddLight(light);

  Sampler* sampler = new ScanlineSampler();
  Shader* shader = new PhongShader();

  Renderer* result = new Renderer(scene, sampler, shader);
  return result;
}
