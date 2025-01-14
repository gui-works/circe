#ifndef CIRCE_SCENE_BVH_H
#define CIRCE_SCENE_BVH_H

#include <circe/gl/scene/scene_object.h>

namespace circe::gl {

/* hierarchical structure
 * Bounding Volume Hierarchies.
 */
class BVH {
public:
  friend class BVHModel;
  /* Constructor.
   * @m **[in]**
   */
  BVH(SceneMeshObjectSPtr m);
  virtual ~BVH() {}

  SceneMeshObjectSPtr sceneMesh;

  int intersect(const hermes::Ray3 &ray, float *t = nullptr);
  bool isInside(const hermes::point3 &p);

private:
  struct BVHElement {
    BVHElement(size_t i, const hermes::bbox3 &b) : ind(i), bounds(b) {
      centroid = b.centroid();
    }
    size_t ind;
    hermes::bbox3 bounds;
    hermes::point3 centroid;
  };
  struct BVHNode {
    BVHNode() { children[0] = children[1] = nullptr; }
    void initLeaf(uint32_t first, uint32_t n, const hermes::bbox3 &b) {
      firstElementOffset = first;
      nElements = n;
      bounds = b;
    }
    void initInterior(uint32_t axis, BVHNode *c0, BVHNode *c1) {
      children[0] = c0;
      children[1] = c1;
      bounds = hermes::make_union(c0->bounds, c1->bounds);
      splitAxis = axis;
      nElements = 0;
    }
    hermes::bbox3 bounds;
    BVHNode *children[2];
    uint32_t splitAxis, firstElementOffset, nElements;
  };
  struct LinearBVHNode {
    hermes::bbox3 bounds;
    union {
      uint32_t elementsOffset;
      uint32_t secondChildOffset;
    };
    uint8_t nElements;
    uint8_t axis;
    uint8_t pad[2];
  };
  struct Comparepoints {
    Comparepoints(int d) { dim = d; }
    int dim;
    bool operator()(const BVHElement &a, const BVHElement &b) const {
      return a.centroid[dim] < b.centroid[dim];
    }
  };
  std::vector<uint32_t> orderedElements;
  std::vector<LinearBVHNode> nodes;
  BVHNode *root;
  BVHNode *recursiveBuild(std::vector<BVHElement> &buildData, uint32_t start,
                          uint32_t end, uint32_t *totalNodes,
                          std::vector<uint32_t> &orderedElements);
  uint32_t flattenBVHTree(BVHNode *node, uint32_t *offset);
  bool intersect(const hermes::bbox3 &bounds, const hermes::Ray3 &ray,
                 const hermes::vec3 &invDir, const uint32_t dirIsNeg[3]) const;
};

} // namespace circe

#endif // CIRCE_SCENE_MESH_H
