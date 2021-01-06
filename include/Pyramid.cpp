#include "Pyramid.hpp"

#include <cassert>

Pyramid::Pyramid() {}
Pyramid::Pyramid(size_t const &height, size_t const &width)
    : h{height}, w{width} {
    this->nodes.resize(this->h * this->w, nullptr);
    this->construct();
}

flt &Pyramid::operator()(size_t const &x, size_t const &y) {
    return this->nodes[this->w * y + x]->depth;
}
flt const &Pyramid::operator()(size_t const &x, size_t const &y) const {
    return this->nodes[this->w * y + x]->depth;
}

void Pyramid::construct() {
    pss southwestern = std::make_pair(0, 0);
    pss northeastern = std::make_pair(this->w, this->h);
    debugm("Constructing depth buffer MIP-map ..\n");
    this->root = build(southwestern, northeastern, nullptr);
    this->update_tdep(this->root);
    msg("Hierarchical depth buffer constructed\n");
}

void Pyramid::setz(size_t const &x, size_t const &y, flt const &zval) {
    (*this)(x, y) = zval;
    // debugm("finding node with (%zu, %zu)\n", x, y);
    Node4 *node = this->which(x, y, this->root);
    // debugm("found node with (%zu, %zu)\n", x, y);
    assert(node != nullptr);
    node->depth = zval;
    while (node != this->root) {
        // node->fa->depth = std::min(node->fa->depth, node->depth);
        node = node->fa;
        this->pushup(node);
    }
}

bool Pyramid::visible(Triangle const &t) {
    // NOTE: t has screenspace coordinates.
    flt    nearest_z = std::max(t.c().z, std::max(t.a().z, t.b().z));
    Node4 *a         = this->which(t.a().x, t.a().y, this->root);
    Node4 *b         = this->which(t.b().x, t.b().y, this->root);
    Node4 *c         = this->which(t.c().x, t.c().y, this->root);
    Node4 *anc       = this->lca(a, this->lca(b, c));
    if (nearest_z < anc->depth) {
        // Invisible if the entire image's farthest depth value is closer than
        // the triangle's nearest depth value.
        return false;
    }
    return true;
}

bool Pyramid::visible(Triangle const &t, Node4 *node) const {
    if (node == nullptr) {
        node = this->root;
    }
    flt nearest_z = std::max(t.c().z, std::max(t.a().z, t.b().z));
    if (nearest_z < node->depth) {
        // If triangle `t` has no vertex nearer than current node's nearest z
        // value, it is not visible.
        return false;
    }
    // If triangle `t` has some vertex nearer than current nodes' nearest z
    // value ..
    if (node->isleaf) {
        // If this node cannot be further devided (is a leaf node), the
        // triangle is visible.
        return true;
    }
    // One of the vertices of traingle `t` falls in this child's quadrant.
    Node4 *child = node->children[node->idof(t.a().x, t.a().y)];
    if (node->children[node->idof(t.b().x, t.b().y)] != child ||
        node->children[node->idof(t.c().x, t.c().y)] != child) {
        // If not all vertices of triangle `t` falls in the same quadrant,
        // i.e. if triangle `t` covers more than one quadrant of current node,
        // treat `t` as visible and do normal rendering.
        return true;
    }
    // If all 3 vertices of triangle `t` falls into the same quadrant, dive
    // into that quadrant and do further checking.
    return this->visible(t, child);
}

// private methods
void Pyramid::pushup(Node4 *node) const {
    flt ndepth = std::numeric_limits<flt>::infinity();
    for (auto &child : node->children) {
        if (nullptr == child) {
            continue;
        }
        ndepth = std::min(ndepth, child->depth);
    }
    node->depth = ndepth;
}

void Pyramid::update_tdep(Node4 *node) const {
    for (auto &child : node->children) {
        if (nullptr == child) {
            continue;
        }
        child->tdep = node->tdep + 1;
        update_tdep(child);
    }
}

Node4 *const Pyramid::lca(Node4 *a, Node4 *b) const {
    Node4 *anca = a, *ancb = b;
    while (anca != ancb) {
        if (anca->tdep > ancb->tdep) {
            anca = anca->fa;
        } else {
            ancb = ancb->fa;
        }
    }
    return anca;
}

Node4 *Pyramid::which(int x, int y, Node4 *node) const {
    x = clamp(x, 0, this->w - 1);
    y = clamp(y, 0, this->h - 1);
    return this->nodes[y * this->w + x];
}

Node4 *const Pyramid::build(pss const &sw, pss const &ne, Node4 *const fa) {
    int x1 = sw.first, y1 = sw.second;
    int x2 = ne.first, y2 = ne.second;
    if (x1 >= x2 || y1 >= y2) {
        // Can't divide in either of x or y dimention.
        return nullptr;
    }
    Node4 *ret = new Node4;
    ret->fa    = fa;
    ret->sw    = sw;
    ret->ne    = ne;
    ret->depth = -std::numeric_limits<flt>::infinity();
    if (x1 + 1 == x2 && y1 + 1 == y2) {
        // Leaf node, assign actual depth value to the node and return.
        ret->isleaf                    = true;
        ret->split                     = sw;
        this->nodes[y1 * this->w + x1] = ret;
        return ret;
    }
    int midx   = (x1 + x2) >> 1;
    int midy   = (y1 + y2) >> 1;
    ret->split = std::make_pair(midx, midy);
    // Note: `ne` has EXclusive coordinate.
    pss mid   = std::make_pair(midx, midy);
    pss mid_s = std::make_pair(midx, y1); // Southern mid point
    pss mid_w = std::make_pair(x1, midy); // Western mid point
    pss mid_n = std::make_pair(midx, y2); // Northern mid point
    pss mid_e = std::make_pair(x2, midy); // Eastern mid point
    // Southwestern child
    ret->children[0] = build(sw, mid, ret);
    // Southeastern child
    ret->children[1] = build(mid_s, mid_e, ret);
    // Northweastern child
    ret->children[2] = build(mid_w, mid_n, ret);
    // Northeastern child
    ret->children[3] = build(mid, ne, ret);
    pushup(ret);
    return ret;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Dec 07 2020, 16:31 [CST]
