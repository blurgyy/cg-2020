#pragma once

#include "global.hpp"

#include <array>
#include <vector>

struct Node4 : Node<4> {
    Node4()
        : depth(-std::numeric_limits<flt>::infinity()), children{nullptr} {}

    flt depth;

    /* Node's children has following orders:
     * |-------|
     * | 2 | 3 |
     * |-------|
     * | 0 | 1 |
     * |-------|
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

    // Actual data
    std::vector<flt> raw;

    // Root of depth MIP-map
    Node4 *root;

  private:
    // MIP-map construction helper functions
    void pushup(Node4 *node);
    // Build pyramid recursively, root node has index 0.
    // @param     sw: Southwestern coordinate (INclusive) of this node's area.
    // @param     ne: Northeastern coordinate (EXclusive) of this node's area.
    Node4 *build(pss const &sw, pss const &ne);

  public:
    Pyramid();
    Pyramid(size_t const &height, size_t const &width);

    void construct();

    // Get depth value's reference at image coordinate (x, y)
    flt &operator()(size_t const &x, size_t const &y);
    // Get depth value's const reference at image coordinate (x, y)
    flt const &operator()(size_t const &x, size_t const &y) const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Dec 07 2020, 16:28 [CST]
