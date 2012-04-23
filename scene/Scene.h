/*
 *	Contains elements, lights etc. and can be rendered.
 *	Author: Dino Wernli
 */

#ifndef SCENE_H_
#define SCENE_H_

#include<memory>
#include<vector>

class Element;

class Scene {
 public:
  Scene();
  virtual ~Scene();

  // Takes ownership of the passed element.
  void AddElement(Element* element);
 private:
  std::vector<std::unique_ptr<Element> > elements_;
};

#endif  /* SCENE_H_ */
