#include "Pyramid.hpp"

#include <cassert>

Pyramid::Pyramid() {}
Pyramid::Pyramid(size_t const &height, size_t const &width)
    : h(height), w(width) {
    this->raw.resize(this->h * this->w,
                     -std::numeric_limits<flt>::infinity());
    this->construct();
}

flt &Pyramid::operator()(size_t const &x, size_t const &y) {
    return this->raw[w * y + x];
}
flt const &Pyramid::operator()(size_t const &x, size_t const &y) const {
    return this->raw[w * y + x];
}

void Pyramid::construct() {
    pss southwestern = std::make_pair(0, 0);
    pss northeastern = std::make_pair(this->w, this->h);
    debugm("Construction depth buffer MIP-map ..\n");
    this->root = build(southwestern, northeastern);
    msg("Hierarchical depth buffer constructed\n");
}

// private methods
void Pyramid::pushup(Node4 *node) {
    for (auto &child : node->children) {
        if (nullptr == child) {
            continue;
        }
        node->depth = std::min(node->depth, child->depth);
        child->fa   = node;
    }
}

Node4 *Pyramid::build(pss const &sw, pss const &ne) {
    int    x1 = sw.first, y1 = sw.second;
    int    x2 = ne.first, y2 = ne.second;
    Node4 *ret = new Node4;
    if (x1 >= x2 || y1 >= y2) {
        // Can't divide in either of x or y dimention.
        ret = nullptr;
        return ret;
    }
    if (x1 + 1 == x2 && y1 + 1 == y2) {
        // Leaf node, assign actual depth value to the node and return.
        ret->depth = (*this)(x1, y1);
        return ret;
    }
    int midx = (x1 + x2) >> 1;
    int midy = (y1 + y2) >> 1;
    // Note: `ne` has EXclusive coordinate.
    pss mid   = std::make_pair(midx, midy);
    pss mid_s = std::make_pair(midx, y1); // Southern mid point
    pss mid_w = std::make_pair(x1, midy); // Western mid point
    pss mid_n = std::make_pair(midx, y2); // Northern mid point
    pss mid_e = std::make_pair(x2, midy); // Eastern mid point
    // Southwestern child
    ret->children[0] = build(sw, mid);
    // Southeastern child
    ret->children[1] = build(mid_s, mid_e);
    // Northweastern child
    ret->children[2] = build(mid_w, mid_n);
    // Northeastern child
    ret->children[3] = build(mid, ne);
    // or this... ret->children[3] = build(mid, ne, (nodeid << 2) + 4);
    pushup(ret);
    return ret;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Dec 07 2020, 16:31 [CST]
