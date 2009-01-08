/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_MAX_COVER_HPP
#define GTL_MAX_COVER_HPP
namespace gtl {

  template <typename Unit>
  struct MaxCover {
    typedef interval_data<Unit> Interval;
    typedef rectangle_data<Unit> Rectangle;

    class Node {
    private:
      std::vector<Node*> children_;
      std::set<Interval> tracedPaths_;
    public:
      Rectangle rect;
      Node() {}
      Node(const Rectangle rectIn) : rect(rectIn) {}
      typedef typename std::vector<Node*>::iterator iterator;
      inline iterator begin() { return children_.begin(); }
      inline iterator end() { return children_.end(); }
      inline void add(Node* child) { children_.push_back(child); }
      inline bool tracedPath(const Interval& ivl) const {
        return tracedPaths_.find(ivl) != tracedPaths_.end();
      }
      inline void addPath(const Interval& ivl) {
        tracedPaths_.insert(tracedPaths_.end(), ivl);
      }
    };

    typedef std::pair<std::pair<Unit, Interval>, Node* > EdgeAssociation;
  
    class lessEdgeAssociation : public std::binary_function<const EdgeAssociation&, const EdgeAssociation&, bool> {
    public:
      inline lessEdgeAssociation() {}
      inline bool operator () (const EdgeAssociation& elem1, const EdgeAssociation& elem2) const {
        if(elem1.first.first < elem2.first.first) return true;
        if(elem1.first.first > elem2.first.first) return false;
        return elem1.first.second < elem2.first.second;
      }
    };

    template <class cT>
    static inline void getMaxCover(cT& outputContainer, Node* node, orientation_2d orient) {
      Interval rectIvl = node->rect.get(orient);
      if(node->tracedPath(rectIvl)) {
        return;
      }
      node->addPath(rectIvl);
      if(node->begin() == node->end()) {
        outputContainer.push_back(copy_construct<typename cT::value_type, Rectangle>(node->rect));
        return;
      }
      bool writeOut = true;
      for(typename Node::iterator itr = node->begin(); itr != node->end(); ++itr) {
        getMaxCover(outputContainer, *itr, orient, node->rect); //get rectangles down path
        Interval nodeIvl = (*itr)->rect.get(orient);
        if(contains(nodeIvl, rectIvl, true)) writeOut = false;
      }
      if(writeOut) {
        outputContainer.push_back(copy_construct<typename cT::value_type, Rectangle>(node->rect));
      }
    }

    template <class cT>
    static inline void getMaxCover(cT& outputContainer, Node* node, orientation_2d orient, 
                                   const Rectangle& rect) {
      Interval rectIvl = rect.get(orient);
      Interval nodeIvl = node->rect.get(orient);
      if(!intersect(rectIvl, nodeIvl, false)) {
        return;
      }
      if(node->tracedPath(rectIvl)) {
        return;
      }
      node->addPath(rectIvl);
      Rectangle nextRect(rectIvl, rectIvl);
      Unit low = rect.get(orient.get_perpendicular()).low();
      Unit high = node->rect.get(orient.get_perpendicular()).high();
      nextRect.set(orient.get_perpendicular(), Interval(low, high));
      bool writeOut = true;
      rectIvl = nextRect.get(orient);
      for(typename Node::iterator itr = node->begin(); itr != node->end(); ++itr) {
        nodeIvl = (*itr)->rect.get(orient);
        if(contains(nodeIvl, rectIvl, true)) writeOut = false;
        getMaxCover(outputContainer, *itr, orient, nextRect);
      }
      if(writeOut) {
        outputContainer.push_back(copy_construct<typename cT::value_type, Rectangle>(nextRect));
      }
    }

    //computeDag populates the node of the iterator range of Nodes with parent child
    //relationships by adding the child node to the parent node's list of children
    //iterator range is assummed to be in topological order meaning all node's trailing
    //edges are in sorted order
    template <class iT>
    static inline void computeDag(iT beginNode, iT endNode, orientation_2d orient,
                           unsigned int size) {
      std::vector<EdgeAssociation> leadingEdges; 
      leadingEdges.reserve(size);
      for(iT iter = beginNode; iter != endNode; ++iter) {
        Node* nodep = &(*iter);
        Unit leading = nodep->rect.get(orient.get_perpendicular()).low();
        Interval rectIvl = nodep->rect.get(orient);
        leadingEdges.push_back(EdgeAssociation(std::pair<Unit, Interval>(leading, rectIvl), nodep));
      }
      std::sort(leadingEdges.begin(), leadingEdges.end(), lessEdgeAssociation());
      typename std::vector<EdgeAssociation>::iterator leadingBegin = leadingEdges.begin();
      iT trailingBegin = beginNode;
      while(leadingBegin != leadingEdges.end()) {
        EdgeAssociation& leadingSegment = (*leadingBegin);
        Unit trailing = (*trailingBegin).rect.get(orient.get_perpendicular()).high();
        Interval ivl = (*trailingBegin).rect.get(orient);
        std::pair<Unit, Interval> trailingSegment(trailing, ivl);
        if(leadingSegment.first.first < trailingSegment.first) {
          ++leadingBegin;
          continue;
        }
        if(leadingSegment.first.first > trailingSegment.first) {
          ++trailingBegin;
          continue;
        }
        if(leadingSegment.first.second.high() <= trailingSegment.second.low()) {
          ++leadingBegin;
          continue;
        }
        if(trailingSegment.second.high() <= leadingSegment.first.second.low()) {
          ++trailingBegin;
          continue;
        }
        //leading segment intersects trailing segment
        (*trailingBegin).add((*leadingBegin).second);
        if(leadingSegment.first.second.high() > trailingSegment.second.high()) {
          ++trailingBegin;
          continue;
        }
        if(trailingSegment.second.high() > leadingSegment.first.second.high()) {
          ++leadingBegin;
          continue;
        }
        ++leadingBegin;
        ++trailingBegin;
      }
    }

    template <class cT>
    static inline void getMaxCover(cT& outputContainer,
                                   const std::vector<Rectangle>& rects, orientation_2d orient) {
      if(rects.empty()) return;
      std::vector<Node> nodes;
      {
        if(rects.size() == 1) {
          outputContainer.push_back(copy_construct<typename cT::value_type, Rectangle>(rects[0]));
          return;
        }
        nodes.reserve(rects.size());
        for(unsigned int i = 0; i < rects.size(); ++i) { nodes.push_back(Node(rects[i])); }
      }
      computeDag(nodes.begin(), nodes.end(), orient, nodes.size());
      for(unsigned int i = 0; i < nodes.size(); ++i) {
        getMaxCover(outputContainer, &(nodes[i]), orient);
      }
    }


  };

}

#endif
