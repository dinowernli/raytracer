/*
 * Author: Dino Wernli
 */

#include "renderer.h"

#include <memory>

#include "proto/configuration.pb.h"
#include "renderer/sampler/sample.h"
#include "renderer/sampler/sampler.h"
#include "renderer/sampler/scanline_sampler.h"
#include "renderer/updatable.h"
#include "scene/camera.h"
#include "scene/point_light.h"
#include "scene/scene.h"
#include "util/ray.h"

class Light;

Renderer::Renderer(Scene* scene, Sampler* sampler)
    : scene_(scene), sampler_(sampler) {
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

  for(std::unique_ptr<Sample> sample(sampler_->NextSample());
      sample.get() != NULL;
      sample.reset(sampler_->NextSample())) {
    Ray ray = camera->GenerateRay(*sample);
    sample->set_color(TraceColor(ray));
    sampler_->AcceptSample(*sample);
  }

  for(auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    it->get()->Update(sampler_->image());
  }
}

Color3 Renderer::TraceColor(const Ray& ray) {
  // TODO(dinow): Implement this. Return gray for now.
  return Color3(0.5, 0.5, 0.5);
}

// static
Renderer* Renderer::FromConfig(const raytracer::Configuration& config) {
  // TODO(dinow): Parse config and pass corresponding objects to new renderer.
  Scene* scene = new Scene();
  scene->set_camera(new Camera(Point3(0, 0, 0), Vector3(0, 0, 1),
                               Vector3(0, 1, 0), PI / 6.0, 300, 500));
  Light* light = new PointLight(Point3(0, 0, 0), Color3(0, 0, 0));
  scene->AddLight(light);

  Renderer* result = new Renderer(scene, new ScanlineSampler());
  return result;
}
