#pragma once

#include "Triangle.hpp"
#include "global.hpp"

#include <array>
#include <vector>

struct Node4 : Node<4> {
    Node4()
        : fa{nullptr}, depth{-std::numeric_limits<flt>::max()},
          children{nullptr} {}

    // Returns which quadrant of current node that coordinate (x, y) is in
    int idof(size_t const &x, size_t const &y) {
        if (this->isleaf) {
            return -1;
        }
        return 2 * (y > this->split.second) + (x > this->split.first);
    }

    // Father of this node
    Node4 *fa;

    // Southwestern and Northeastern corners
    pss sw, ne;
    // Splitting point
    pss split;
    // Depth (z) value
    flt depth;

    /* Node's children has following orders:
     *                  |-------|
     *                  | 2 | 3 |
     *                  |-------|
     *                  | 0 | 1 |
     *                  o-------|
     *                  ^
     *                  origin
     *
     * Caveat: some of the children might be nullptr, because input buffer is
     *         not always a square, causing one dimentions be divided up
     *         earlier than the other.
     * */
    std::array<Node4 *, 4> children;
};

class Pyramid {
  private:
    // Screen size, in pixels
    size_t h, w;

    // Leaf nodes, containing finest depth values
    std::vector<Node4 *> nodes;

  private:
    // Helper functions
    void pushup(Node4 *node) const;
    // Recursively update all tree nodes' `tdep` value.
    void update_tdep(Node4 *node) const;
    // Build pyramid recursively, root node has index 0.
    // @param     sw: Southwestern coordinate (INclusive) of this node's area.
    // @param     ne: Northeastern coordinate (EXclusive) of this node's area.
    Node4 *const build(pss const &sw, pss const &ne,
                       Node4 *const fa = nullptr);
    // Lowest common ancestor of two nodes
    Node4 *const lca(Node4 *a, Node4 *b) const;
    // Leaf node corresponding to image coordinate (x, y), pass this->root to
    // the 3rd parameter.
    Node4 *which(int x, int y, Node4 *node) const;

  public:
    // Root of depth MIP-map
    Node4 *root;

  public:
    Pyramid();
    Pyramid(size_t const &height, size_t const &width);

    // Frontend for MIP-map construction.
    void construct();

    // Clear depths recursively for `node` and all of its descendants.
    void clear(Node4 *node);

    // Set depth value at given image coordinate (x, y), and update the
    // pyramid.
    void setz(size_t const &x, size_t const &y, flt const &zval);

    // Bottom-up visibility checking method.  Computes the least common
    // ancestor node of the 3 vertices of triangle `t`, then check if `t` is
    // visible in that node.
    bool visible(Triangle const &t);
    // Top-down visibility checking method.  Check whether or not the triangle
    // `t` is visible in the root node, if not, dive into its children to do
    // further checking.
    bool visible(Triangle const &t, Node4 *node = nullptr) const;

    // Get depth value's reference at image coordinate (x, y)
    flt &operator()(size_t const &x, size_t const &y);
    // Get depth value's const reference at image coordinate (x, y)
    flt const &operator()(size_t const &x, size_t const &y) const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Dec 07 2020, 16:28 [CST]
