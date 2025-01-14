#include <circe/gl/io/graphics_display.h>
#include <circe/gl/io/viewport_display.h>

namespace circe::gl {

ViewportDisplay::ViewportDisplay(int _x, int _y, int _width, int _height)
    : x(_x), y(_y), width(_width), height(_height) {
  renderer.reset(new DisplayRenderer(width, height));
}

void ViewportDisplay::render(const std::function<void(CameraInterface *)> &f) {
  if (prepareRenderCallback)
    prepareRenderCallback(*this);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  // TODO fix process
//  renderer->process([&]() {
//    if (f)
//      f(camera.get());
//    if (renderCallback)
//      renderCallback(camera.get());
//  });
  //  glDisable(GL_DEPTH_TEST);
  GraphicsDisplay &gd = GraphicsDisplay::instance();
  glViewport(x, y, width, height);
  glScissor(x, y, width, height);
  glEnable(GL_SCISSOR_TEST);
  circe::gl::GraphicsDisplay::clearScreen(clear_screen_color);
//  glEnable(GL_DEPTH_TEST);
// TODO fix post-process
//  renderer->render();
  if (f)
    f(camera.get());
  glDisable(GL_SCISSOR_TEST);
  if (renderEndCallback)
    renderEndCallback();
}

void ViewportDisplay::mouse(double x, double y) {
  if (mouseCallback)
    mouseCallback(x, y);
  camera->mouseMove(getMouseNPos());
}

void ViewportDisplay::scroll(double dx, double dy) {
  camera->mouseScroll(getMouseNPos(), hermes::vec2(dx, dy));
}

void ViewportDisplay::button(int b, int a, int m) {
  if (buttonCallback)
    buttonCallback(b, a, m);
  camera->mouseButton(a, b, getMouseNPos());
}

void ViewportDisplay::key(int k, int scancode, int action, int modifiers) const {
  if (keyCallback)
    keyCallback(k, scancode, action, modifiers);
}

hermes::point2 ViewportDisplay::getMouseNPos() {
  int viewport[] = {0, 0, width, height};
  hermes::point2 mp =
      GraphicsDisplay::instance().getMousePos() - hermes::vec2(x, y);
  return hermes::point2((mp.x - viewport[0]) / viewport[2] * 2.0 - 1.0,
                        (mp.y - viewport[1]) / viewport[3] * 2.0 - 1.0);
}

bool ViewportDisplay::hasMouseFocus() const {
  hermes::point2 mp =
      GraphicsDisplay::instance().getMousePos() - hermes::vec2(x, y);
  return (mp.x >= 0.f && mp.x <= width && mp.y >= 0.f && mp.y <= height);
}

hermes::point3 ViewportDisplay::viewCoordToNormDevCoord(hermes::point3 p) const {
  float v[] = {0, 0, static_cast<float>(width), static_cast<float>(height)};
  return hermes::point3((p.x - v[0]) / (v[2] / 2.0) - 1.0,
                        (p.y - v[1]) / (v[3] / 2.0) - 1.0, 2 * p.z - 1.0);
}

hermes::point3 ViewportDisplay::unProject(const CameraInterface &c,
                                          hermes::point3 p) {
  return hermes::inverse(c.getTransform()) * p;
}

hermes::point3 ViewportDisplay::unProject() {
  return hermes::inverse(camera->getTransform()) *
      hermes::point3(getMouseNPos().x, getMouseNPos().y, 0.f);
}

} // namespace circe
