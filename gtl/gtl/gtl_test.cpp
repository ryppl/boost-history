#include "gtl.hpp"
#include <time.h>
#include <stdlib.h>

namespace gtl {
  inline bool testPolygon45SetRect() {
    std::vector<point_data<int> > points;
    points.push_back(point_data<int>(0,0));
    points.push_back(point_data<int>(0,10));
    points.push_back(point_data<int>(10,10));
    points.push_back(point_data<int>(10,0));
    polygon_45_data<int> poly;
    poly.set(points.begin(), points.end());
      polygon_45_set_data<int> ps;
    ps.insert(poly);
    std::vector<polygon_45_data<int> > polys;
    ps.get_polygons(polys);
    std::cout << polys.size() << std::endl;
    for(unsigned int i = 0; i < polys.size(); ++i) {
      std::cout << polys[i] << std::endl;
    }
    return true;
  }

  inline bool testPolygon45Set() {
    polygon_45_formation<int>::Polygon45Formation pf(true);
    typedef boolean_op_45<int>::Vertex45 Vertex45;
    std::vector<Vertex45> data;
    // result == 0 8 -1 1
    data.push_back(Vertex45(point_data<int>(0, 8), -1, 1));
    // result == 0 8 1 -1
    data.push_back(Vertex45(point_data<int>(0, 8), 1, -1));
    // result == 4 0 1 1
    data.push_back(Vertex45(point_data<int>(4, 0), 1, 1));
    // result == 4 0 2 1
    data.push_back(Vertex45(point_data<int>(4, 0), 2, 1));
    // result == 4 4 2 -1
    data.push_back(Vertex45(point_data<int>(4, 4), 2, -1));
    // result == 4 4 -1 -1
    data.push_back(Vertex45(point_data<int>(4, 4), -1, -1));
    // result == 4 12 1 1
    data.push_back(Vertex45(point_data<int>(4, 12), 1, 1));
    // result == 4 12 2 1
    data.push_back(Vertex45(point_data<int>(4, 12), 2, 1));
    // result == 4 16 2 -1
    data.push_back(Vertex45(point_data<int>(4, 16), 2, 1));
    // result == 4 16 -1 -1
    data.push_back(Vertex45(point_data<int>(4, 16), -1, -1));
    // result == 6 2 1 -1
    data.push_back(Vertex45(point_data<int>(6, 2), 1, -1));
    // result == 6 14 -1 1
    data.push_back(Vertex45(point_data<int>(6, 14), -1, 1));
    // result == 6 2 -1 1
    data.push_back(Vertex45(point_data<int>(6, 2), -1, 1));
    // result == 6 14 1 -1
    data.push_back(Vertex45(point_data<int>(6, 14), 1, -1));
    // result == 8 0 -1 -1
    data.push_back(Vertex45(point_data<int>(8, 0), -1, -1));
    // result == 8 0 2 -1
    data.push_back(Vertex45(point_data<int>(8, 0), 2, -1));
    // result == 8 4 2 1
    data.push_back(Vertex45(point_data<int>(8, 4), 2, 1));
    // result == 8 4 1 1
    data.push_back(Vertex45(point_data<int>(8, 4), 1, 1));
    // result == 8 12 -1 -1
    data.push_back(Vertex45(point_data<int>(8, 12), -1, -1));
    // result == 8 12 2 -1
    data.push_back(Vertex45(point_data<int>(8, 12), 2, -1));
    // result == 8 16 2 1
    data.push_back(Vertex45(point_data<int>(8, 16), 2, 1));
    // result == 8 16 1 1
    data.push_back(Vertex45(point_data<int>(8, 16), 1, 1));
    // result == 12 8 1 -1
    data.push_back(Vertex45(point_data<int>(12, 8), 1, -1));
    // result == 12 8 -1 1
    data.push_back(Vertex45(point_data<int>(12, 8), -1, 1));

    data.push_back(Vertex45(point_data<int>(6, 4), 1, -1));
    data.push_back(Vertex45(point_data<int>(6, 4), 2, -1));
    data.push_back(Vertex45(point_data<int>(6, 12), -1, 1));
    data.push_back(Vertex45(point_data<int>(6, 12), 2, 1));
    data.push_back(Vertex45(point_data<int>(10, 8), -1, -1));
    data.push_back(Vertex45(point_data<int>(10, 8), 1, 1));

    std::sort(data.begin(), data.end());
    std::vector<polygon_45_data<int> > polys;
    pf.scan(polys, data.begin(), data.end());
    polygon_45_set_data<int> ps;
    std::cout << "inserting1\n";
    //std::vector<point_data<int> > points;
    //points.push_back(point_data<int>(0,0));
    //points.push_back(point_data<int>(0,10));
    //points.push_back(point_data<int>(10,10));
    //points.push_back(point_data<int>(10,0));
    //Polygon45 poly;
    //poly.set(points.begin(), points.end());
    //ps.insert(poly);
    ps.insert(polys[0]);

    polygon_45_set_data<int> ps2;
    std::cout << "inserting2\n";
    ps2.insert(polys[0]);
    std::cout << "applying boolean\n";
    ps |= ps2;
    std::vector<polygon_45_data<int> > polys2;
    std::cout << "getting result\n";
    ps.get_polygons(polys2);
    std::cout << ps2 << std::endl;
    std::cout << ps << std::endl;
    std::cout << polys[0] << std::endl;
    std::cout << polys2[0] << std::endl;
    if(polys != polys2) std::cout << "test Polygon45Set failed\n";
    return polys == polys2;
  }

  inline bool testPolygon45SetPerterbation() {
    polygon_45_formation<int>::Polygon45Formation pf(true);
    typedef boolean_op_45<int>::Vertex45 Vertex45;
    std::vector<Vertex45> data;
    // result == 0 8 -1 1
    data.push_back(Vertex45(point_data<int>(0, 80), -1, 1));
    // result == 0 8 1 -1
    data.push_back(Vertex45(point_data<int>(0, 80), 1, -1));
    // result == 4 0 1 1
    data.push_back(Vertex45(point_data<int>(40, 0), 1, 1));
    // result == 4 0 2 1
    data.push_back(Vertex45(point_data<int>(40, 0), 2, 1));
    // result == 4 4 2 -1
    data.push_back(Vertex45(point_data<int>(40, 40), 2, -1));
    // result == 4 4 -1 -1
    data.push_back(Vertex45(point_data<int>(40, 40), -1, -1));
    // result == 4 12 1 1
    data.push_back(Vertex45(point_data<int>(40, 120), 1, 1));
    // result == 4 12 2 1
    data.push_back(Vertex45(point_data<int>(40, 120), 2, 1));
    // result == 4 16 2 -1
    data.push_back(Vertex45(point_data<int>(40, 160), 2, 1));
    // result == 4 16 -1 -1
    data.push_back(Vertex45(point_data<int>(40, 160), -1, -1));
    // result == 6 2 1 -1
    data.push_back(Vertex45(point_data<int>(60, 20), 1, -1));
    // result == 6 14 -1 1
    data.push_back(Vertex45(point_data<int>(60, 140), -1, 1));
    // result == 6 2 -1 1
    data.push_back(Vertex45(point_data<int>(60, 20), -1, 1));
    // result == 6 14 1 -1
    data.push_back(Vertex45(point_data<int>(60, 140), 1, -1));
    // result == 8 0 -1 -1
    data.push_back(Vertex45(point_data<int>(80, 0), -1, -1));
    // result == 8 0 2 -1
    data.push_back(Vertex45(point_data<int>(80, 0), 2, -1));
    // result == 8 4 2 1
    data.push_back(Vertex45(point_data<int>(80, 40), 2, 1));
    // result == 8 4 1 1
    data.push_back(Vertex45(point_data<int>(80, 40), 1, 1));
    // result == 8 12 -1 -1
    data.push_back(Vertex45(point_data<int>(80, 120), -1, -1));
    // result == 8 12 2 -1
    data.push_back(Vertex45(point_data<int>(80, 120), 2, -1));
    // result == 8 16 2 1
    data.push_back(Vertex45(point_data<int>(80, 160), 2, 1));
    // result == 8 16 1 1
    data.push_back(Vertex45(point_data<int>(80, 160), 1, 1));
    // result == 12 8 1 -1
    data.push_back(Vertex45(point_data<int>(120, 80), 1, -1));
    // result == 12 8 -1 1
    data.push_back(Vertex45(point_data<int>(120, 80), -1, 1));

    data.push_back(Vertex45(point_data<int>(60, 40), 1, -1));
    data.push_back(Vertex45(point_data<int>(60, 40), 2, -1));
    data.push_back(Vertex45(point_data<int>(60, 120), -1, 1));
    data.push_back(Vertex45(point_data<int>(60, 120), 2, 1));
    data.push_back(Vertex45(point_data<int>(100, 80), -1, -1));
    data.push_back(Vertex45(point_data<int>(100, 80), 1, 1));

    std::sort(data.begin(), data.end());
    std::vector<polygon_45_data<int> > polys;
    pf.scan(polys, data.begin(), data.end());
    polygon_45_set_data<int> ps;
    std::cout << "inserting1\n";
    //std::vector<point_data<int> > points;
    //points.push_back(point_data<int>(0,0));
    //points.push_back(point_data<int>(0,10));
    //points.push_back(point_data<int>(10,10));
    //points.push_back(point_data<int>(10,0));
    //Polygon45 poly;
    //poly.set(points.begin(), points.end());
    //ps.insert(poly);
    polygon_45_set_data<int> preps(polys[0]);
   
    ps.insert(polys[0]);
    convolve(polys[0], point_data<int>(0, 1) );

    polygon_45_set_data<int> ps2;
    std::cout << "inserting2\n";
    ps2.insert(polys[0]);
    std::cout << "applying boolean\n";
    ps |= ps2;
    std::vector<polygon_45_data<int> > polys2;
    std::cout << "getting result\n";
    ps.get_polygons(polys2);
    std::cout << preps << std::endl;
    std::cout << ps2 << std::endl;
    std::cout << ps << std::endl;
    std::cout << polys[0] << std::endl;
    std::cout << polys2[0] << std::endl;
    //if(polys != polys2) std::cout << "test Polygon45Set failed\n";
    //return polys == polys2;
    return true;
  }

  inline int testPolygon45SetDORA() {
    std::cout << "testPolygon45SetDORA" << std::endl;
    std::vector<point_data<int> > pts;
    pts.push_back(point_data<int>(0, 0));
    pts.push_back(point_data<int>(10, 0));
    pts.push_back(point_data<int>(10, 10));
    pts.push_back(point_data<int>(0, 10));
    polygon_45_data<int> apoly;
    apoly.set(pts.begin(), pts.end());
    polygon_45_set_data<int> ps(apoly);
    polygon_45_set_data<int> ps2(ps);
    ps2 = apoly;
    std::vector<polygon_45_data<int> > apolys;
    apolys.push_back(apoly);
    ps2.insert(apolys.begin(), apolys.end());
    apolys.clear();
    ps2.get(apolys);
    std::cout << apolys.size() << std::endl;
    std::cout << (ps == ps2) << std::endl;
    std::cout << !(ps != ps2) << std::endl;
    ps2.clear();
    std::cout << (ps2.value().empty()) << std::endl;
    ps2.set(apolys.begin(), apolys.end());
    ps2.set(ps.value());
    ps.clean();
    ps2.set_clean(ps.value());
    ps2.insert(ps.value().begin(), ps.value().end());
    ps2.clear();
    for(polygon_45_set_data<int>::iterator_type itr = ps.begin();
        itr != ps.end(); ++itr) {
      ps2.insert(*itr);
    }
    std::vector<polygon_45_with_holes_data<int> > apolywhs;
    ps2.get_polygons_with_holes(apolywhs);
    std::cout << apolywhs.size() << std::endl;
    ps2 += 1;
    apolywhs.clear();
    ps2.get_polygons_with_holes(apolywhs);
    if(apolywhs.size()) std::cout << apolywhs[0] << std::endl;
    ps2 -= 1;
    apolywhs.clear();
    ps2.get_polygons_with_holes(apolywhs);
    if(apolywhs.size()) std::cout << apolywhs[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    rectangle_data<int> rect;
    extents(rect, apolywhs[0]);
    ps2.clear();
    ps2.insert(rect);
    ps2.extents(rect);
    ps2.clear();
    ps2.insert(rect);
    ps2.clear();
    ps2.insert(apolywhs[0]);
    apolywhs.clear();
    ps2.get_trapezoids(apolywhs);
    if(apolywhs.size()) std::cout << apolywhs[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    ps2 *= ps;
    std::cout << (ps2 == ps) << std::endl;
    ps2 ^= ps;
    std::cout << ps2.empty() << std::endl;
    axis_transformation atr(axis_transformation::WS);
    ps2 = ps;
    ps.transform(atr);
    transformation<int> tr(atr);
    tr.invert();
    ps.transform(tr);
    ps.scale_up(2);
    ps.scale_down(2);
    std::cout << (ps2 == ps) << std::endl;
    pts.clear();
    pts.push_back(point_data<int>(0,0));
    pts.push_back(point_data<int>(10,10));
    pts.push_back(point_data<int>(10,11));
    pts.push_back(point_data<int>(0,21));
    apoly.set(pts.begin(), pts.end());
    ps2.clear();
    ps2.insert(apoly);
    ps2 -= 1;
    apolywhs.clear();
    ps2.get_polygons_with_holes(apolywhs);
    if(apolywhs.size()) std::cout << apolywhs[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    pts.clear();
    pts.push_back(point_data<int>(0, 0));
    pts.push_back(point_data<int>(10, 10));
    pts.push_back(point_data<int>(0, 20));
    apoly.set(pts.begin(), pts.end());
    ps2.clear();
    ps2.insert(apoly);
    pts.clear();
    pts.push_back(point_data<int>(0, 5));
    pts.push_back(point_data<int>(10, 15));
    pts.push_back(point_data<int>(0, 25));
    apoly.set(pts.begin(), pts.end());
    ps2.insert(apoly);
    apolywhs.clear();
    ps2.get_polygons_with_holes(apolywhs);
    if(apolywhs.size()) std::cout << apolywhs[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    return 0;

  }

}
using namespace gtl;

bool testInterval() {
  interval_data<int> interval(0, 10), interval2(10, 20);
  if(!abuts(interval, interval2)) return false;
  if(!boundaries_intersect(interval, interval2)) return false;
  if(boundaries_intersect(interval, interval2, false)) return false;
  if(intersect(interval, interval2, false)) return false;
  if(!intersect(interval, interval2)) return false;
  if(euclidean_distance(interval, interval2) != 0) return false;
  encompass(interval, interval2);
  set(interval, LOW, 0);
  high(interval, 10);
  scale(interval, 2.0f);
  scale(interval, 0.5f);
  if(low(interval) != 0) return false;
  if(high(interval) != 10) return false;
  move(interval, 10);
  if(!equivalence(interval, interval2)) return false;
  flip(interval, 10);
  bloat(interval, -2);
  shrink(interval, -2);
  flip(interval, 10);
  if(!equivalence(interval, interval2)) return false;
  interval_data<int> half = get_half(interval, LOW);
  if(high(half) != 15) return false;
  convolve(interval, interval2);
  if(high(interval) != 40) return false;
  deconvolve(interval, interval2);
  if(!equivalence(interval, interval2)) return false;
  reflected_convolve(interval, interval2);
  if(low(interval) != -10) return false;
  reflected_deconvolve(interval, interval2);
  if(!equivalence(interval, interval2)) return false;
  euclidean_distance(interval, 0);
  move(interval, 20);
  if(euclidean_distance(interval, interval2) != 10) return false;
  interval = interval2;
  move(interval, -5);
  if(!intersects(interval, interval2)) return false;
  move(interval, 15);
  if(!abuts(interval, interval2)) return false;
  if(abuts(interval, interval2, HIGH)) return false;
  move(interval, 10);
  generalized_intersect(interval, interval2);
  move(interval, -10);
  if(!equivalence(interval, interval2)) return false;
  if(get(interval, LOW) != low(interval)) return false;
  if(get(interval, HIGH) != high(interval)) return false;
  if(center(interval2) != 15) return false;
  if(delta(interval2) != 10) return false;
  assign(interval, interval2);
  low(interval, 0);
  if(low(interval) != 0) return false;
  high(interval, 10);
  join_with(interval, interval2);
  if(high(interval) != high(interval2)) return false;
  return true;
}

bool testRectangle() {
  rectangle_data<int> rect, rect2;
  horizontal(rect, interval_data<long long>(0, 10));
  vertical(rect, interval_data<long long>(20, 30));
  xl(rect2, 0);
  xh(rect2, 10);
  yl(rect2, 20);
  yh(rect2, 30);
  if(euclidean_distance(rect, rect2) != 0) return false;
  if(euclidean_distance(rect2, rect) != 0) return false;
  set(rect, HORIZONTAL, interval_data<long long>(0, 10));
  if(!equivalence(horizontal(rect), interval_data<long long>(0, 10))) return false;
  if(!equivalence(vertical(rect2), interval_data<long long>(20, 30))) return false;
  if(xl(rect) != 0) return false;
  if(xh(rect) != 10) return false;
  if(yl(rect) != 20) return false;
  if(yh(rect) != 30) return false;
  move(rect, HORIZONTAL, 10);
  if(xl(rect) != 10) return false;
  set_points(rect, point_data<int>(0, 20), point_data<long long>(10, 30));
  if(xl(rect) != 0) return false;
  convolve(rect, rect2);
  if(xh(rect) != 20) return false;
  deconvolve(rect, rect2);
  if(xh(rect) != 10) return false;
  reflected_convolve(rect, rect2);
  reflected_deconvolve(rect, rect2);
  if(!equivalence(rect, rect2)) return false;
  convolve(rect, point_data<long long>(100, 200));
  if(xh(rect) != 110) return false;
  deconvolve(rect, point_data<int>(100, 200));
  if(!equivalence(rect, rect2)) return false;
  xh(rect, 100);
  if(delta(rect, HORIZONTAL) != 100) return false;
  if(area(rect) != 1000) return false;
  if(half_perimeter(rect) != 110) return false;
  if(perimeter(rect) != 220) return false;
  if(guess_orientation(rect) != HORIZONTAL) return false;
  return true;
}


bool testPolygon() {
  int rect[4] = {0, 10, 20, 30};
  iterator_compact_to_points<int*, point_data<int> > itr(rect, rect+4);
  iterator_compact_to_points<int*, point_data<int> > itr_end(rect, rect+4);
  std::vector<point_data<int> > points;
  points.insert(points.end(), itr, itr_end);
  polygon_90_data<int> p90;
  assign(p90, rectangle_data<int>(interval_data<int>(0, 10), interval_data<int>(20, 30)));
  if(winding(p90) != COUNTERCLOCKWISE) return false;
  polygon_45_data<int> p45;
  assign(p45, rectangle_data<int>(interval_data<int>(0, 10), interval_data<int>(20, 30)));
  if(winding(p45) != COUNTERCLOCKWISE) return false;
  polygon_data<int> p;
  assign(p, rectangle_data<int>(interval_data<int>(0, 10), interval_data<int>(20, 30)));
  if(winding(p) != COUNTERCLOCKWISE) return false;
  set_compact(p90, rect, rect+4);
  if(winding(p90) != COUNTERCLOCKWISE) return false;
  points.clear();
  points.push_back(point_data<int>(0, 0));
  points.push_back(point_data<int>(10, 10));
  points.push_back(point_data<int>(0, 20));
  points.push_back(point_data<int>(-10, 10));
  set_points(p45, points.begin(), points.end());
  if(winding(p45) != COUNTERCLOCKWISE) return false;
  std::swap(points[1], points[3]);
  set_points(p, points.begin(), points.end());
  if(winding(p) == COUNTERCLOCKWISE) return false;
  point_data<int> cp;
  center(cp, p);
  if(cp != point_data<int>(0, 10)) return false;
  move(p, HORIZONTAL, 3);
  rectangle_data<int> bounding_box;
  extents(bounding_box, p);
  if(bounding_box != rectangle_data<int>(interval_data<int>(-7, 13), interval_data<int>(0, 20))) return false;
  if(area(p90) != 400) return false;
  if(area(p45) != 200) return false;
  if(perimeter(p90) != 80) return false;
  return true;
}

bool testPolygonAssign() {
  polygon_data<int> p;
  polygon_45_data<int> p_45;
  polygon_90_data<int> p_90;
  polygon_with_holes_data<int> p_wh;
  polygon_45_with_holes_data<int> p_45_wh;
  polygon_90_with_holes_data<int> p_90_wh;
  assign(p, p);
  assign(p, p_45);
  assign(p, p_90);
  //assign(p, p_wh);
  //assign(p, p_45_wh);
  //assign(p, p_90_wh);
  //assign(p_45, p);
  assign(p_45, p_45);
  assign(p_45, p_90);
  //assign(p_45, p_wh);
  //assign(p_45, p_45_wh);
  //assign(p_45, p_90_wh);
  //assign(p_90, p);
  //assign(p_90, p_45);
  assign(p_90, p_90);
  //assign(p_90, p_wh);
  //assign(p_90, p_45_wh);
  //assign(p_90, p_90_wh);
  assign(p_wh, p); 
  assign(p_wh, p_45); 
  assign(p_wh, p_90); 
  assign(p_wh, p_wh);
  assign(p_wh, p_45_wh);
  assign(p_wh, p_90_wh);
  //assign(p_45_wh, p); 
  assign(p_45_wh, p_45); 
  assign(p_45_wh, p_90); 
  //assign(p_45_wh, p_wh);
  assign(p_45_wh, p_45_wh);
  //assign(p_90_wh, p); 
  //assign(p_90_wh, p_45);
  assign(p_90_wh, p_90);
  assign(p_90_wh, p_90_wh);
  return true;
}

int testPropertyMerge() {
  rectangle_data<int> rect1 = construct<rectangle_data<int> >(0, 1, 10, 11);
  rectangle_data<int> rect2 = construct<rectangle_data<int> >(5, 6, 17, 18);
  property_merge_90<int, int> pm;
  pm.insert(rect1, 0);
  pm.insert(rect2, 1);
  std::map<std::set<int>, polygon_90_set_data<int> > result;
  pm.merge(result);
  std::vector<rectangle_data<int> > rects;
  std::set<int> key;
  key.insert(0);
  result[key].get(rects);
  std::cout << rects.size() << std::endl;
  std::vector<polygon_data<int> > polys;
  result[key].get(polys);
  std::cout << polys.size() << std::endl;
  std::vector<polygon_90_with_holes_data<int> > polywhs;
  result[key].get(polywhs);
  std::cout << polys.size() << std::endl;
  return result.size();
}

bool testPolygonWithHoles() {
  int rect[4] = {0, 10, 20, 30};
  iterator_compact_to_points<int*, point_data<int> > itr(rect, rect+4);
  iterator_compact_to_points<int*, point_data<int> > itr_end(rect, rect+4);
  std::vector<point_data<int> > points;
  points.insert(points.end(), itr, itr_end);
  polygon_45_with_holes_data<int> p45wh;
  assign(p45wh, rectangle_data<int>(interval_data<int>(0, 10), interval_data<int>(20, 30)));
  if(winding(p45wh) != COUNTERCLOCKWISE) return false;
  polygon_45_with_holes_data<int> p45;
  assign(p45, rectangle_data<int>(interval_data<int>(0, 10), interval_data<int>(20, 30)));
  if(winding(p45) != COUNTERCLOCKWISE) return false;
  polygon_45_with_holes_data<int> p;
  assign(p, rectangle_data<int>(interval_data<int>(0, 10), interval_data<int>(20, 30)));
  if(winding(p) != COUNTERCLOCKWISE) return false;
  set_compact(p45wh, rect, rect+4);
  if(winding(p45wh) != COUNTERCLOCKWISE) return false;
  points.clear();
  points.push_back(point_data<int>(0, 0));
  points.push_back(point_data<int>(10, 10));
  points.push_back(point_data<int>(0, 20));
  points.push_back(point_data<int>(-10, 10));
  set_points(p45, points.begin(), points.end());
  if(winding(p45) != COUNTERCLOCKWISE) return false;
  std::swap(points[1], points[3]);
  set_points(p, points.begin(), points.end());
  if(winding(p) == COUNTERCLOCKWISE) return false;
  point_data<int> cp;
  center(cp, p);
  if(cp != point_data<int>(0, 10)) return false;
  move(p, HORIZONTAL, 3);
  rectangle_data<int> bounding_box;
  extents(bounding_box, p);
  if(bounding_box != rectangle_data<int>(interval_data<int>(-7, 13), interval_data<int>(0, 20))) return false;
  if(area(p45wh) != 400) return false;
  if(area(p45) != 200) return false;
  if(perimeter(p45wh) != 80) return false;
  return true;
}

using namespace gtl;

typedef int Unit;
typedef point_data<int> Point;
typedef interval_data<int> Interval;
typedef rectangle_data<int> Rectangle;
typedef polygon_90_data<int> Polygon;
typedef polygon_90_with_holes_data<int> PolygonWithHoles;
typedef polygon_45_data<int> Polygon45;
typedef polygon_45_with_holes_data<int> Polygon45WithHoles;
typedef polygon_90_set_data<int> PolygonSet;
typedef polygon_45_set_data<int> Polygon45Set;
typedef axis_transformation AxisTransform;
typedef transformation<int> Transform;

int getRandomBool() {
  return rand()%2;
}
int getRandomInt() {
  return rand()%6-2;
}
Point getRandomPoint() {
  int x = rand()%8;
  int y = rand()%8;
  return Point(x, y);
}
Polygon45 getRandomTriangle() {
  Point pts[3];
  pts[0] = getRandomPoint();
  pts[1] = pts[2] = pts[0];
  int disp = getRandomInt();
  bool dir = getRandomBool();
  x(pts[2], x(pts[2]) + disp);
  x(pts[1], x(pts[1]) + disp);
  if(dir)
    y(pts[1], y(pts[1]) + disp);
  else 
    y(pts[1], y(pts[1]) - disp);
  return Polygon45(pts, pts+3);
}

bool nonInteger45StessTest() {
  for(unsigned int tests = 0; tests < 10; ++tests) {
    Polygon45Set ps1, ps2;
    std::vector<Polygon45> p45s;
    for(unsigned int i = 0; i < 10; ++i) {
      Polygon45 p45 = getRandomTriangle();
      p45s.push_back(p45);
      ps1.insert(p45);
      scale_up(p45, 2);
      ps2.insert(p45);
    }
    std::vector<Polygon45> polys;
    ps1.get(polys);
    Polygon45Set ps3;
    for(unsigned int i = 0; i < polys.size(); ++i) {
      scale_up(polys[i], 2);
      ps3.insert(polys[i]);
    }
    Polygon45Set ps4 = ps3 ^ ps2;
    std::vector<Polygon45> polys_error;
    ps4.get(polys_error);
    for(unsigned int i = 0; i < polys_error.size(); ++i) {
      //if(polys_error[i].size() > 3) return false;
      if(area(polys_error[i]) != 1) {
        if(area(polys_error[i]) == 2) {
          //if two area 1 errors merge it will have area 2
          continue;
        }
        std::cout << "test failed\n";
        for(unsigned int j =0; j < p45s.size(); ++j) {
          std::cout << p45s[j] << std::endl;
        }
        return false;
      }
    }
  }
  return true;
}

bool validate_polygon_set_op(Polygon45Set& ps45_o,
                             const Polygon45Set& ps45_1,
                             const Polygon45Set& ps45_2,
                             int op_type) {
  Polygon45Set s_ps_45_o(ps45_o);
  Polygon45Set s_ps_45_1(ps45_1);
  Polygon45Set s_ps_45_2(ps45_2);
  s_ps_45_o.scale_up(2);
  s_ps_45_1.scale_up(2);
  s_ps_45_2.scale_up(2);
  Polygon45Set s_ps_45_validate;
  if(op_type == 0) {
    s_ps_45_validate = s_ps_45_1 + s_ps_45_2;
    s_ps_45_validate += Rectangle(4, 4, 6, 6);
  } else if(op_type == 1) {
    s_ps_45_validate = s_ps_45_1 * s_ps_45_2;
    s_ps_45_validate -= Rectangle(4, 4, 6, 6);
  } else if(op_type == 2) {
    s_ps_45_validate = s_ps_45_1 ^ s_ps_45_2;
    s_ps_45_validate -= Rectangle(4, 4, 6, 6);
  } else {
    s_ps_45_validate = s_ps_45_1 - s_ps_45_2;
    s_ps_45_validate -= Rectangle(4, 4, 6, 6);
  }
  if(s_ps_45_validate != s_ps_45_o) {
    std::cout << "TEST FAILED\n";
    std::vector<Polygon45> polys;
    s_ps_45_o.get(polys);
    std::cout << "Result:\n";
    for(unsigned int i = 0; i < polys.size(); ++i) {
      std::cout << polys[i] << std::endl;
    }
    polys.clear();
    s_ps_45_validate.get(polys);
    std::cout << "Expected Result:\n";
    for(unsigned int i = 0; i < polys.size(); ++i) {
      std::cout << polys[i] << std::endl;
    }
    //redo the operation, set breakpoints here
    switch (op_type) {
    case 0:
      ps45_o = ps45_1 + ps45_2;
      ps45_o.get(polys);//needed to force clean
      break;
    case 1:
      ps45_o = ps45_1 * ps45_2;
      break;
    case 2:
      ps45_o = ps45_1 ^ ps45_2;
      break;
    default:
      ps45_o = ps45_1 - ps45_2;
    };
    //redo the check, set breakpoints here
    if(op_type == 0) {
      s_ps_45_validate = s_ps_45_1 + s_ps_45_2;
      s_ps_45_validate += Rectangle(4, 4, 6, 6);
      s_ps_45_validate.get(polys);
    } else if(op_type == 1) {
      s_ps_45_validate = s_ps_45_1 * s_ps_45_2;
      s_ps_45_validate -= Rectangle(4, 4, 6, 6);
    } else if(op_type == 2) {
      s_ps_45_validate = s_ps_45_1 ^ s_ps_45_2;
      s_ps_45_validate -= Rectangle(4, 4, 6, 6);
    } else {
      s_ps_45_validate = s_ps_45_1 - s_ps_45_2;
      s_ps_45_validate -= Rectangle(4, 4, 6, 6);
    }
    return false;
  }
  return true;
}

bool test_two_polygon_sets(const Polygon45Set& ps45_1,
                           const Polygon45Set& ps45_2) {
  std::cout << "test two polygon sets \n";
  std::vector<Polygon45> polys;
  ps45_1.get(polys);
  std::cout << "LVALUE:\n";
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
  }
  polys.clear();
  ps45_2.get(polys);
  std::cout << "RVALUE:\n";
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
  }
  Polygon45Set ps45_o;
  std::cout << "OR\n";
  ps45_o = ps45_1 + ps45_2;
  polys.clear();
  ps45_o.get(polys);
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
  }
  if(!validate_polygon_set_op(ps45_o, ps45_1, ps45_2, 0)) return false;
  std::cout << "AND\n";
  ps45_o = ps45_1 * ps45_2;
  polys.clear();
  ps45_o.get(polys);
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
  }
  if(!validate_polygon_set_op(ps45_o, ps45_1, ps45_2, 1)) return false;
  std::cout << "XOR\n";
  ps45_o = ps45_1 ^ ps45_2;
  polys.clear();
  ps45_o.get(polys);
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
  }
  if(!validate_polygon_set_op(ps45_o, ps45_1, ps45_2, 2)) return false;
  std::cout << "SUBTRACT\n";
  ps45_o = ps45_1 - ps45_2;
  polys.clear();
  ps45_o.get(polys);
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
  }
  if(!validate_polygon_set_op(ps45_o, ps45_1, ps45_2, 3)) return false;
  return true;
}

bool test_two_polygons(const Polygon45& p45_1,
                       const Polygon45& p45_2) {
  Polygon45Set ps45_1, ps45_2;
  ps45_1.insert(p45_1);
  ps45_2.insert(p45_2);
  ps45_1.insert(rectangle_data<int>(10, -100, 20, 100));
  ps45_2.insert(rectangle_data<int>(0, 10, 100, 20));
  if(!test_two_polygon_sets(ps45_1, ps45_2)) return false;
  Polygon45Set ps45_1_c = ps45_1 - Rectangle(0, 0, 2, 5);
  Polygon45Set ps45_2_c = ps45_2 - Rectangle(0, 0, 2, 5);
  if(!test_two_polygon_sets(ps45_1_c, ps45_2_c)) return false;
  if(!test_two_polygon_sets(ps45_1_c, ps45_2)) return false;
  if(!test_two_polygon_sets(ps45_1, ps45_2_c)) return false;
  return true;
}

bool test_45_touch() {
  using namespace gtl;
  connectivity_extraction_45<int> ce;
  rectangle_data<int> rect1(0, 0, 10, 10);
  rectangle_data<int> rect2(5, 5, 15, 15);
  rectangle_data<int> rect3(5, 20, 15, 25);
  ce.insert(rect1);
  ce.insert(rect2);
  ce.insert(rect3);
  std::vector<std::set<int> > graph(3);
  ce.extract(graph);
  if(graph[0].size() == 1 && graph[1].size() == 1 && graph[2].size() == 0) {
    std::set<int>::iterator itr = graph[0].begin();
    std::cout << *itr << std::endl;
    std::set<int>::iterator itr1 = graph[1].begin();
    std::cout << *itr1 << std::endl;
    return true;
  }
  std::cout << "test failed\n";
  return false;
}

bool test_45_touch_ur() {
  using namespace gtl;
  connectivity_extraction_45<int> ce;
  rectangle_data<int> rect1(0, 0, 5, 5);
  rectangle_data<int> rect2(5, 5, 10, 10);
  ce.insert(rect1);
  ce.insert(rect2);
  std::vector<std::set<int> > graph(2);
  ce.extract(graph);
  if(graph[0].size() == 1 && graph[1].size() == 1) {
    std::set<int>::iterator itr = graph[0].begin();
    std::cout << *itr << std::endl;
    std::set<int>::iterator itr1 = graph[1].begin();
    std::cout << *itr1 << std::endl;
    return true;
  }
  std::cout << "test failed\n";
  return false;
}

bool test_45_touch_r() {
  using namespace gtl;
  connectivity_extraction_45<int> ce;
  rectangle_data<int> rect1(0, 0, 5, 5);
  rectangle_data<int> rect2(5, 0, 10, 5);
  ce.insert(rect1);
  ce.insert(rect2);
  std::vector<std::set<int> > graph(2);
  ce.extract(graph);
  if(graph[0].size() == 1 && graph[1].size() == 1) {
    std::set<int>::iterator itr = graph[0].begin();
    std::cout << *itr << std::endl;
    std::set<int>::iterator itr1 = graph[1].begin();
    std::cout << *itr1 << std::endl;
    return true;
  }
  std::cout << "test failed\n";
  return false;
}

bool test_45_touch_boundaries() {
  using namespace gtl;
  connectivity_extraction_45<int> ce;
  rectangle_data<int> rect1(0, 0, 10, 10);
  rectangle_data<int> rect2(10, 0, 20, 10);
  rectangle_data<int> rect3(20, 0, 30, 10);
  rectangle_data<int> rect4(0, 10, 10, 20);
  rectangle_data<int> rect5(10, 10, 20, 20);
  rectangle_data<int> rect6(20, 10, 30, 20);
  rectangle_data<int> rect7(0, 20, 10, 30);
  rectangle_data<int> rect8(10, 20, 20, 30);
  rectangle_data<int> rect9(20, 20, 30, 30);
  ce.insert(rect1);
  ce.insert(rect2);
  ce.insert(rect3);
  ce.insert(rect4);
  ce.insert(rect5);
  ce.insert(rect6);
  ce.insert(rect7);
  ce.insert(rect8);
  ce.insert(rect9);
  std::vector<std::set<int> > graph(9);
  ce.extract(graph);
  for(unsigned int i = 0; i < 9; ++i) {
    std::cout << i << ": ";
    for(std::set<int>::iterator itr = graph[i].begin(); itr != graph[i].end(); ++itr) {
      std::cout << *itr << " ";
    } std::cout << std::endl;
  }
  if(graph[0].size() == 3 && graph[1].size() == 5 && graph[2].size() == 3 &&
     graph[3].size() == 5 && graph[4].size() == 8 && graph[5].size() == 5 &&
     graph[6].size() == 3 && graph[7].size() == 5 && graph[8].size() == 3) {
    return true;
  }
  std::cout << "test failed\n";
  return false;
}

bool test_45_concept_interact() {
  using namespace gtl;
  std::vector<polygon_45_data<int> > polys;
  polys += rectangle_data<int>(10, 10, 20, 20);
  polys += rectangle_data<int>(15, 15, 25, 25);
  polys += rectangle_data<int>(5, 25, 10, 35);
  interact(polys, rectangle_data<int>(0, 0, 13, 13));
  if(polys.size() != 1) return false;
  return true;
}

bool test_get_rectangles() {
  using namespace gtl;
  polygon_90_set_data<int> ps(VERTICAL);
  ps += rectangle_data<int>(0, 0, 10, 10);
  ps += rectangle_data<int>(5, 5, 15, 15);
  std::vector<polygon_90_data<int> > polys;
  ps.get_rectangles(polys, HORIZONTAL);
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
  }
  if(polys.size() != 3) return false;
  std::vector<rectangle_data<int> > rects;
  ps.get_rectangles(rects, HORIZONTAL);
  for(unsigned int i = 0; i < rects.size(); ++i) {
    std::cout << rects[i] << std::endl;
  }
  if(rects.size() != 3) return false;
  if(!equivalence(rects[2], rectangle_data<int>(5,10,15,15))) return false;

  get_rectangles(polys, rects, VERTICAL);
  get_rectangles(rects, polys, HORIZONTAL);
  return equivalence(rects, polys);
}

bool test_get_trapezoids() {
  using namespace gtl;
  polygon_45_set_data<int> ps;
  ps += rectangle_data<int>(0, 0, 10, 10);
  ps += rectangle_data<int>(5, 5, 15, 15);
  std::vector<polygon_45_data<int> > polys;
  ps.get_trapezoids(polys, HORIZONTAL);
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
  }
  if(polys.size() != 3) return false;
  std::vector<polygon_45_data<int> > rects;
  ps.get_trapezoids(rects, HORIZONTAL);
  for(unsigned int i = 0; i < rects.size(); ++i) {
    std::cout << rects[i] << std::endl;
  }
  if(rects.size() != 3) return false;
  if(!equivalence(rects[2], rectangle_data<int>(5,10,15,15))) return false;
  get_trapezoids(polys, rects, VERTICAL);
  get_trapezoids(rects, polys, HORIZONTAL);
  return equivalence(rects, polys);
}

bool test_SQRT1OVER2() {
  Point pts[] = {
    Point(100, 100),
    Point(0, 100),
    Point(100, 200),
    Point(0, 300),
    Point(100, 400),
    Point(0, 500),
    Point(100, 500),
    Point(100, 600),
    Point(200, 500),
    Point(300, 600),
    Point(400, 500),
    Point(500, 600),
    Point(500, 500),
    Point(600, 500),
    Point(500, 400),
    Point(600, 300),
    Point(500, 200),
    Point(600, 100),
    Point(500, 100),
    Point(500, 0),
    Point(400, 100),
    Point(300, 0),
    Point(200, 100),
    Point(100, 0),
    Point(100, 100)
  };
  Polygon45 p45(pts, pts+25);
  std::cout << is_45(p45) << std::endl;
  std::cout << p45 << std::endl;
  Polygon45Set ps45;
  ps45 += p45;
  ps45.resize(10, SQRT1OVER2, ORTHOGONAL);
  std::vector<Polygon45> polys;
  ps45.get(polys);
  if(polys.size() != 1) return false;
  Point pts2[] = {
    Point(90, 90),
    Point(-10, 90),
    Point(-10, 100),
    Point(90, 200),
    Point(-10, 300),
    Point(90, 400),
    Point(-10, 500),
    Point(-10, 510),
    Point(90, 510),
    Point(90, 610),
    Point(100, 610),
    Point(200, 510),
    Point(300, 610),
    Point(400, 510),
    Point(500, 610),
    Point(510, 610),
    Point(510, 510),
    Point(610, 510),
    Point(610, 500),
    Point(510, 400),
    Point(610, 300),
    Point(510, 200),
    Point(610, 100),
    Point(610, 90),
    Point(510, 90),
    Point(510, -10),
    Point(500, -10),
    Point(400, 90),
    Point(300, -10),
    Point(200, 90),
    Point(100, -10),
    Point(90, -10),
    Point(90, 90)
  };
  Polygon45 p45reference(pts2, pts2+33);
  std::cout << is_45(polys[0]) << std::endl;
  std::cout << polys[0] << std::endl;
  std::cout << p45reference << std::endl;
  std::cout << is_45(p45reference) << std::endl;
  if(!equivalence(polys[0], p45reference)) {
    std::cout << "polys don't match\n";
    return false;
  }
  ps45.resize(-10, SQRT1OVER2, ORTHOGONAL);
  polys.clear();
  ps45.get(polys);
  if(polys.size() != 1) return false;
  std::cout << is_45(polys[0]) << std::endl;
  std::cout << polys[0] << std::endl;
  if(!equivalence(polys[0], p45)) {
    std::cout << "polys don't match\n";
    return false;
  }
  ps45.resize(11, SQRT1OVER2, UNFILLED);
  polys.clear();
  ps45.get(polys);
  if(polys.size() != 1) return false;
  std::cout << is_45(polys[0]) << std::endl;
  std::cout << polys[0] << std::endl;
  return true;
}

bool test_scaling_by_floating(){
  Point pts[] = {
    Point(1, 1),
    Point(10, 1),
    Point(1, 10)
  };
  Polygon45 poly(pts, pts+3);
  Polygon45Set ps45;
  ps45 += poly;
  ps45.scale(double(2.5));
  std::vector<Polygon45> polys;
  ps45.get(polys);
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
    std::cout << area(polys[i]) << std::endl;
  }
  if(polys.size() != 1) return false;
  if(area(polys[0]) != 242) return false;
  scale(ps45, double(1)/double(2.5));
  polys.clear();
  ps45.get(polys);
  for(unsigned int i = 0; i < polys.size(); ++i) {
    std::cout << polys[i] << std::endl;
  }
  return equivalence(polys, poly);
}

bool test_directional_resize() {
  std::vector<Rectangle> rects;
  rects.push_back(Rectangle(0, 0, 100, 100));
  resize(rects, -10, 10, -10, 10);
  for(unsigned int i = 0; i < rects.size(); ++i) {
    std::cout << rects[i] << std::endl;
  }
  if(rects.size() != 1) return false;
  if(rects[0] != Rectangle(10, 10, 110, 110)) return false;

  return true;
}

bool test_self_xor() {
  std::vector<Rectangle> rects;
  rects.push_back(Rectangle(0, 0, 10, 10));
  rects.push_back(Rectangle(5, 5, 15, 15));
  self_xor(rects);
  for(unsigned int i = 0; i < rects.size(); ++i) {
    std::cout << rects[i] << std::endl;
  }
  if(rects.size() == 4) return true;
  else return false;
}

bool test_grow_and_45() {
  polygon_45_set_data<int> ps;
  ps.insert(Rectangle(0, 0, 5, 5));
  ps.insert(Rectangle(5, 5, 15, 15));
  grow_and(ps, 2);
  std::vector<polygon_45_data<int> > rects;
  ps.get_trapezoids(rects);
  for(unsigned int i = 0; i < rects.size(); ++i) {
    std::cout << rects[i] << std::endl;
  }
  if(rects.size() != 1) return false;
  return equivalence(rects, Rectangle(3, 3, 7, 7));
}

bool test_self_xor_45() {
  polygon_45_set_data<int> ps;
  ps.insert(Rectangle(0, 0, 10, 10));
  ps.insert(Rectangle(5, 5, 15, 15));
  self_xor(ps);
  std::vector<polygon_45_data<int> > rects;
  ps.get_trapezoids(rects);
  for(unsigned int i = 0; i < rects.size(); ++i) {
    std::cout << rects[i] << std::endl;
  }
  if(rects.size() == 4) return true;
  else return false;
}

bool testViewCopyConstruct() {
  PolygonSet ps1, ps2;
  ps1.insert(Rectangle(0, 0, 10, 10));
  ps2.insert(Rectangle(5, 5, 15, 15));
  PolygonSet psr = ps1 - ps2;
  std::vector<Rectangle> rects;
  rects += psr;
  for(unsigned int i = 0; i < rects.size(); ++i)
    std::cout << rects[i] << std::endl;
  if( rects.size() != 2) return false;
  Polygon45Set ps45_1, ps45_2;
  ps45_1.insert(Rectangle(0, 0, 10, 10));
  ps45_2.insert(Rectangle(5, 5, 15, 15));
  Polygon45Set ps45_r = ps45_1 - ps45_2;
  std::vector<Polygon45> polys;
  ps45_r.get_trapezoids(polys);
  for(unsigned int i = 0; i < polys.size(); ++i)
    std::cout << polys[i] << std::endl;
  if( polys.size() != 2) return false;
  return true;
}

bool testpip() {
  std::vector<Point> pts;
  pts.push_back(Point(0, 0));
  pts.push_back(Point(10, 0));
  pts.push_back(Point(20, 10));
  pts.push_back(Point(0, 20));
  pts.push_back(Point(30, 40));
  pts.push_back(Point(-10, 50));
  pts.push_back(Point(-20, -20));
  pts.push_back(Point(0, 0));
  polygon_data<int> poly;
  poly.set(pts.begin(), pts.end());
  for(unsigned int i = 0; i < pts.size(); ++i) {
    if(!contains(poly, pts[i], true)) return false;
    if(contains(poly, pts[i], false)) return false;
  }
  Point pt(0, -10);
  if(contains(poly, pt)) return false;
  Point p2(0, 1);
  if(!contains(poly, p2)) return false;
  return true;
}

int main() {
  if(!testpip()) return 1;
  {
    PolygonSet ps;
    Polygon p;
    assign(ps, p);
  }
  if(!testViewCopyConstruct()) return 1;
  if(!test_grow_and_45()) return 1;
  if(!test_self_xor_45()) return 1;
  if(!test_self_xor()) return 1;
  if(!test_directional_resize()) return 1;
  if(!test_scaling_by_floating()) return 1;
  if(!test_SQRT1OVER2()) return 1;
  if(!test_get_trapezoids()) return 1;
  if(!test_get_rectangles()) return 1;
  if(!test_45_concept_interact()) return 1;
  if(!test_45_touch_r()) return 1;
  if(!test_45_touch_ur()) return 1;
  if(!test_45_touch()) return 1;
  if(!test_45_touch_boundaries()) return 1;
  {
  Point pts[] = {Point(0,0), Point(5, 5), Point(5, 0)};
  Polygon45 p45(pts, pts+3);
  pts[1] = Point(0, 5);
  Polygon45 p452(pts, pts+3);
  if(!test_two_polygons(p45,p452)) return 1;
  pts[2] = Point(5,5);
  p45.set(pts, pts+3);
  if(!test_two_polygons(p45,p452)) return 1;
  pts[0] = Point(5,0);
  p452.set(pts, pts+3);
  if(!test_two_polygons(p45, p452)) return 1;
  Point pts2[] = {Point(0,5), Point(5, 5), Point(5, 0)};
  Point pts3[] = {Point(0,0), Point(5, 5), Point(5, 0)};
  p45.set(pts2, pts2 + 3);
  p452.set(pts3, pts3+3);
  if(!test_two_polygons(p45, p452)) return 1;
  Point pts4[] = {Point(0, 5), Point(2, 3), Point(2,5)};
  Point pts5[] = {Point(0,0), Point(5, 5), Point(5, 0)};
  p45.set(pts4, pts4+3);
  p452.set(pts5, pts5+3);
  //if(!test_two_polygons(p45, p452)) return 1;
  }
  {
  std::vector<point_data<int> > pts;
  pts.push_back(point_data<int>(0, 0));
  pts.push_back(point_data<int>(10, 0));
  pts.push_back(point_data<int>(10, 10));
  pts.push_back(point_data<int>(0, 10));
  std::vector<point_data<int> > pts2;
  pts2.push_back(point_data<int>(0, 0));
  pts2.push_back(point_data<int>(10, 10));
  pts2.push_back(point_data<int>(0, 20));
  pts2.push_back(point_data<int>(-10, 10));
  std::vector<point_data<int> > pts3;
  pts3.push_back(point_data<int>(0, 0));
  pts3.push_back(point_data<int>(10, 11));
  pts3.push_back(point_data<int>(0, 20));
  pts3.push_back(point_data<int>(-100, 8));
  polygon_data<int> p; p.set(pts3.begin(), pts3.end());
  polygon_45_data<int> p45; p45.set(pts2.begin(), pts2.end());
  polygon_90_data<int> p90; p90.set(pts.begin(), pts.end());
  polygon_with_holes_data<int> pwh; pwh.set(pts3.begin(), pts3.end());
  polygon_45_with_holes_data<int> p45wh; p45wh.set(pts2.begin(), pts2.end());
  polygon_90_with_holes_data<int> p90wh; p90wh.set(pts.begin(), pts.end());
  assign(p, p90);
  assign(p, p45);
  assign(p, p);
  //illegal: assign(p, p90wh);
  //illegal: assign(p, p45wh);
  //illegal: assign(p, pwh);

  assign(p45, p90);
  assign(p45, p45);
  //illegal: assign(p45, p);
  //illegal: assign(p45, p90wh);
  //illegal: assign(p45, p45wh);
  //illegal: assign(p45, pwh);

  assign(p90, p90);
  //illegal: assign(p90, p45);
  //illegal: assign(p90, p);
  //illegal: assign(p90, p90wh);
  //illegal: assign(p90, p45wh);
  //illegal: assign(p90, pwh);

  assign(pwh, p90);
  assign(pwh, p45);
  assign(pwh, p);
  assign(pwh, p90wh);
  assign(pwh, p45wh);
  assign(pwh, pwh);

  assign(p45wh, p90);
  assign(p45wh, p45);
  //illegal: assign(p45wh, p);
  assign(p45wh, p90wh);
  assign(p45wh, p45wh);
  //illegal: assign(p45wh, pwh);

  assign(p90wh, p90);
  //illegal: assign(p90wh, p45);
  //illegal: assign(p90wh, p);
  assign(p90wh, p90wh);
  //illegal: assign(p90wh, p45wh);
  //illegal: assign(p90wh, pwh);
  pts.clear();
  pts.push_back(point_data<int>(0, 0));
  pts.push_back(point_data<int>(3, 0));
  pts.push_back(point_data<int>(0, 1));
  p.set(pts.begin(), pts.end());
  std::cout << std::endl; std::cout << (area(p90));
  std::cout << std::endl; std::cout << (area(p45));
  std::cout << std::endl; std::cout << (area(p));
  std::cout << std::endl; std::cout << (area(p90wh));
  std::cout << std::endl; std::cout << (area(p45wh));
  std::cout << std::endl; std::cout << (area(pwh));
  std::cout << std::endl;
  point_data<int> pt(1, 1);
  std::cout << contains(p, pt) << std::endl;
  std::cout << contains(p90, pt) << std::endl;
  
  interval_data<int> ivl = construct<interval_data<int> >(0, 10);
  std::cout << get(ivl, LOW) << std::endl;
  set(ivl, HIGH, 20);

  std::cout << perimeter(p) << std::endl;
  if(winding(p) == LOW) std::cout << "LOW" << std::endl;
  if(winding(p) == HIGH) std::cout << "HIGH" << std::endl;
  rectangle_data<long long> rd;
  std::cout << extents(rd, p) << std::endl;
  std::cout << rd << std::endl;

  boolean_op::testBooleanOr<int>();

  std::vector<rectangle_data<int> > rects1, rects2;
  rects2.push_back(rectangle_data<int>(0, 0, 10, 10));
  print_is_polygon_90_set_concept((polygon_90_set_data<int>()));
  print_is_mutable_polygon_90_set_concept((polygon_90_set_data<int>()));
  print_is_polygon_90_set_concept((polygon_90_data<int>()));
  print_is_polygon_90_set_concept((std::vector<polygon_90_data<int> >()));
  assign(rects1, rects2);
  polygon_90_set_data<int> ps90;
  assign(ps90, rects2);
  assign(rects2, ps90);
  assign(ps90, p90);
  assign(rects2, p90);
  std::cout << p90 << std::endl;
  for(unsigned int i = 0; i < rects2.size(); ++i) {
    std::cout << rects2[i] << std::endl;
  }
  bloat(rects2, 10);
  shrink(rects2[0], 10);
  for(unsigned int i = 0; i < rects2.size(); ++i) {
    std::cout << rects2[i] << std::endl;
  }
  move(rects2[0], HORIZONTAL, 30);
  assign(rects1, rects2 + p90);
  std::cout << "result of boolean or\n";
  for(unsigned int i = 0; i < rects1.size(); ++i) {
    std::cout << rects1[i] << std::endl;
  }
  rects1 -= p90;
  std::cout << "result of boolean not\n";
  for(unsigned int i = 0; i < rects1.size(); ++i) {
    std::cout << rects1[i] << std::endl;
  }
  rects1 += p90;
  std::cout << "result of boolean OR\n";
  for(unsigned int i = 0; i < rects1.size(); ++i) {
    std::cout << rects1[i] << std::endl;
  }
  rects1 *= p90;
  std::cout << "result of boolean AND\n";
  for(unsigned int i = 0; i < rects1.size(); ++i) {
    std::cout << rects1[i] << std::endl;
  }
  rects1 ^= rects2;
  std::cout << "result of boolean XOR\n";
  for(unsigned int i = 0; i < rects1.size(); ++i) {
    std::cout << rects1[i] << std::endl;
  }
  rects2.clear();
  get_max_rectangles(rects2, p90);
  std::cout << "result of max rectangles\n";
  for(unsigned int i = 0; i < rects2.size(); ++i) {
    std::cout << rects2[i] << std::endl;
  }
  rects2.clear();
  //operator += and -= don't support polygons, so + and - should not exist
//   rects2 += p90 + 6;
//   std::cout << "result of resize\n";
//   for(unsigned int i = 0; i < rects2.size(); ++i) {
//     std::cout << rects2[i] << std::endl;
//   }
//   std::cout << "result of resize\n";
   std::vector<polygon_90_with_holes_data<int> > polyswh1, polyswh2;
//   polyswh1 += p90 -2;
//   for(unsigned int i = 0; i < polyswh1.size(); ++i) {
//     std::cout << polyswh1[i] << std::endl;
//   }
//   std::cout << "result of resize\n";
   std::vector<polygon_90_data<int> > polys1, polys2;
   polys1 += p90;
   polys1 -= 2;
//   polys1 += p90 -2;
   for(unsigned int i = 0; i < polys1.size(); ++i) {
     std::cout << polys1[i] << std::endl;
   }

  boolean_op_45<int>::testScan45();
  polygon_45_formation<int>::testPolygon45Formation();
  polygon_45_formation<int>::testPolygon45Tiling();

  axis_transformation atr;
  transform(p, atr);
  transform(p45, atr);
  transform(p90, atr);
  transform(pwh, atr);
  transform(p45wh, atr);
  transform(p90wh, atr);
  scale_up(p, 2);
  scale_up(p45, 2);
  scale_up(p90, 2);
  scale_up(pwh, 2);
  scale_up(p45wh, 2);
  scale_up(p90wh, 2);
  scale_down(p, 2);
  scale_down(p45, 2);
  scale_down(p90, 2);
  scale_down(pwh, 2);
  scale_down(p45wh, 2);
  scale_down(p90wh, 2);
  std::vector<polygon_45_data<int> > p45s1, p45s2;
  std::cout << equivalence(p45s1, p45s2) << std::endl;
  std::cout << equivalence(p45, p45wh) << std::endl;
  std::cout << equivalence(p90, p45wh) << std::endl;
  gtl::assign(p45s1, p90);
  p90 = polys1[0];
  move(p90, orientation_2d(HORIZONTAL), 8);
  std::cout << p90 << std::endl << p45wh << std::endl;
  polygon_45_set_data<int> ps45 = p90 + p45wh;
  assign(p45s1, ps45);
  std::cout << "result\n";
  for(unsigned int i = 0; i < p45s1.size(); ++i) {
    std::cout << p45s1[i] << std::endl;
  }
  std::cout << equivalence(p, pwh) << std::endl;
  std::cout << equivalence(p90, pwh) << std::endl;
  std::cout << equivalence(p45, pwh) << std::endl;
  std::cout << equivalence(pwh, pwh) << std::endl;
  p + pwh;
  p90 + pwh;
  p45 + pwh;
  std::cout << testInterval() << std::endl;
  std::cout << testRectangle() << std::endl;
  std::cout << testPolygon() << std::endl;
  std::cout << testPropertyMerge() << std::endl;
  std::cout << testPolygonAssign() << std::endl;
  std::cout << testPolygonWithHoles() << std::endl;
  std::cout << (polygon_arbitrary_formation<int>::testPolygonArbitraryFormationRect()) << std::endl;
  std::cout << (polygon_arbitrary_formation<int>::testPolygonArbitraryFormationP1()) << std::endl;
  std::cout << (polygon_arbitrary_formation<int>::testPolygonArbitraryFormationP2()) << std::endl;
  std::cout << (polygon_arbitrary_formation<int>::testPolygonArbitraryFormationPolys()) << std::endl;
  std::cout << (polygon_arbitrary_formation<int>::testPolygonArbitraryFormationSelfTouch1()) << std::endl;
  std::cout << (polygon_arbitrary_formation<int>::testPolygonArbitraryFormationSelfTouch2()) << std::endl;
  std::cout << (polygon_arbitrary_formation<int>::testPolygonArbitraryFormationSelfTouch3()) << std::endl;
  std::cout << (polygon_arbitrary_formation<int>::testSegmentIntersection()) << std::endl;
  std::cout << (property_merge<int, int>::test_insertion()) << std::endl;
  std::cout << (line_intersection<int>::test_verify_scan()) << std::endl;
  std::cout << (line_intersection<int>::test_validate_scan()) << std::endl;
  std::cout << (scanline<int, int>::test_scanline()) << std::endl;
  std::cout << (property_merge<int, int>::test_merge()) << std::endl;
  std::cout << (property_merge<int, int>::test_intersection()) << std::endl;
  std::cout << (polygon_arbitrary_formation<int>::testPolygonArbitraryFormationColinear()) << std::endl;
  std::cout << (property_merge<int, int>::test_manhattan_intersection()) << std::endl;
  std::cout << (test_arbitrary_boolean_op<int>()) << std::endl;
  }
  {
    polygon_set_data<int> psd;
    rectangle_data<int> rect;
    set_points(rect, point_data<int>(0, 0), point_data<int>(10, 10));
    psd.insert(rect);
    polygon_set_data<int> psd2;
    set_points(rect, point_data<int>(5, 5), point_data<int>(15, 15));
    psd2.insert(rect);
    std::vector<polygon_data<int> > pv;
    polygon_set_data<int> psd3;
    psd3 = psd + psd2;
    psd3.get(pv);
    for(unsigned int i = 0; i < pv.size(); ++i) {
      std::cout << pv[i] << std::endl;
    }
    psd += psd2;
    pv.clear();
    psd3.get(pv);
    for(unsigned int i = 0; i < pv.size(); ++i) {
      std::cout << pv[i] << std::endl;
    }
  }
  {
    polygon_90_set_data<int> psd;
    rectangle_data<int> rect;
    set_points(rect, point_data<int>(0, 0), point_data<int>(10, 10));
    psd.insert(rect);
    polygon_90_set_data<int> psd2;
    set_points(rect, point_data<int>(5, 5), point_data<int>(15, 15));
    psd2.insert(rect);
    std::vector<polygon_90_data<int> > pv;
    interact(psd, psd2);
    assign(pv, psd);
    for(unsigned int i = 0; i < pv.size(); ++i) {
      std::cout << pv[i] << std::endl;
    }

    connectivity_extraction_90<int> ce;
    ce.insert(pv[0]);
    ce.insert(psd2);
    std::vector<std::set<int> > graph(2);
    ce.extract(graph);
    if(graph[0].size() == 1) std::cout << "connectivity extraction is alive\n";

    std::vector<rectangle_data<long long> > lobs;
    get_max_rectangles(lobs, psd);
    if(lobs.size() == 1) std::cout << "max rectangles is alive\n";

    std::vector<rectangle_data<int> > rv;
    rv.push_back(rect);
    set_points(rect, point_data<int>(0, 0), point_data<int>(10, 10));
    rv.push_back(rect);
    self_intersect(rv);
    if(rv.size() == 1) {
      assign(rect, rv.back());
      std::cout << rect << std::endl;
    }

    assign(rv, rv + 1);
    std::cout << rv.size() << std::endl;
    if(rv.size() == 1) {
      assign(rect, rv.back());
      std::cout << rect << std::endl;
    }
    assign(rv, rv - 1);
    if(rv.size() == 1) {
      assign(rect, rv.back());
      std::cout << rect << std::endl;
    }
    rv += 1;
    if(rv.size() == 1) {
      assign(rect, rv.back());
      std::cout << rect << std::endl;
    }
    rv -= 1;
    if(rv.size() == 1) {
      assign(rect, rv.back());
      std::cout << rect << std::endl;
    }
    rv.clear();
    set_points(rect, point_data<int>(0, 0), point_data<int>(10, 10));
    rv.push_back(rect);
    set_points(rect, point_data<int>(12, 12), point_data<int>(20, 20));
    rv.push_back(rect);
    grow_and(rv, 7);
    if(rv.size() == 1) {
      assign(rect, rv.back());
      std::cout << rect << std::endl;
    }
    std::cout << area(rv) << std::endl;
    std::cout << area(rv) << std::endl;
    
    scale_up(rv, 10);
    std::cout << area(rv) << std::endl;
    scale_down(rv, 7);
    std::cout << area(rv) << std::endl;
    if(rv.size() == 1) {
      assign(rect, rv.back());
      std::cout << rect << std::endl;
    }
    keep(rv, 290, 300, 7, 24, 7, 24);
    if(rv.size() == 1) {
      assign(rect, rv.back());
      std::cout << rect << std::endl;
    }
    keep(rv, 300, 310, 7, 24, 7, 24);
    if(rv.empty()) std::cout << "keep is alive\n";
  }
  {
//     typedef int Unit;
//     typedef point_data<int> Point;
//     typedef interval_data<int> Interval;
//     typedef rectangle_data<int> Rectangle;
//     typedef polygon_90_data<int> Polygon;
//     typedef polygon_90_with_holes_data<int> PolygonWithHoles;
//     typedef polygon_45_data<int> Polygon45;
//     typedef polygon_45_with_holes_data<int> Polygon45WithHoles;
//     typedef polygon_90_set_data<int> PolygonSet;
//     //typedef polygon_45_set_data<int> Polygon45Set;
//     typedef axis_transformation AxisTransform;
//     typedef transformation<int> Transform;
    //test polygon45 area, polygon45 with holes area
    std::vector<Point> pts;
    pts.clear();
    pts.push_back(Point(10, 10));
    pts.push_back(Point(15, 10));
    pts.push_back(Point(10, 15));
    Polygon45 polyHole;
    polyHole.set(pts.begin(), pts.end());
    pts.clear();
    pts.push_back(Point(10, 0));
    pts.push_back(Point(20, 10));
    pts.push_back(Point(20, 30));
    pts.push_back(Point(0, 50));
    pts.push_back(Point(0, 10));
    Polygon45WithHoles polyWHoles;
    polyWHoles.set(pts.begin(), pts.end());
    polyWHoles.set_holes(&polyHole, (&polyHole)+1);
     std::cout << polyWHoles << std::endl;
    std::cout << area(polyWHoles) << std::endl;
    std::cout << area(polyWHoles) << std::endl;
     //test polygon45, polygon45with holes transform
     AxisTransform atr(AxisTransform::EAST_SOUTH);
     Polygon45WithHoles p45wh(polyWHoles);
     transform(polyWHoles, atr);
     std::cout << polyWHoles << std::endl;
     Transform tr(atr);
     tr.invert();
     transform(polyWHoles, tr);
     std::cout << polyWHoles << std::endl;
     if(area(polyWHoles) != 687.5) return 1;
     //test polygon, polygon with holes transform
     Polygon ph;
     assign(ph, Rectangle(10, 10, 20, 20));
     PolygonWithHoles pwh;
     assign(pwh, Rectangle(0, 0, 100, 100));
     pwh.set_holes(&ph, (&ph)+1);
     std::cout << area(pwh) << std::endl;
     transform(pwh, atr);
     std::cout << pwh << std::endl;
     std::cout << area(pwh) << std::endl;
     transform(pwh, tr);
     std::cout << pwh << std::endl;
     std::cout << area(pwh) << std::endl;
     if(area(pwh) != 9900) return 1;
     
    //test point scale up / down
    Point pt(10, 10);
    scale_up(pt, 25);
    if(pt != Point(250, 250)) return 1;
    std::cout << pt << std::endl;
    scale_down(pt, 25);
    if(pt != Point(10, 10)) return 1;
    std::cout << pt << std::endl;
    scale_down(pt, 25);
    if(pt != Point(0, 0)) return 1;
    std::cout << pt << std::endl;

    //test polygon, polygon with holes scale up down
    PolygonWithHoles tmpPwh(pwh);
    scale_up(pwh, 25);
    std::cout << pwh << std::endl;
    scale_down(pwh, 25);
    if(area(pwh) != area(tmpPwh)) return 1;
    std::cout << pwh << std::endl;
    scale_down(pwh, 25);
    std::cout << pwh << std::endl;
    //test polygon45, polygon45 with holes is45
    std::cout << is_45(polyHole) << std::endl;
    if(is_45(polyHole) != true) return 1;
    pts.clear();
    pts.push_back(Point(10, 10));
    pts.push_back(Point(15, 10));
    pts.push_back(Point(10, 16));
    polyHole.set(pts.begin(), pts.end());
    std::cout << is_45(polyHole) << std::endl;
    if(is_45(polyHole) != false) return 1;
    //test polygon45, polygon45 with holes snap 45
    snap_to_45(polyHole);
    std::cout << is_45(polyHole) << std::endl;
    if(is_45(polyHole) != true) return 1;
    std::cout << polyHole << std::endl;
    //test polygon45, polygon45 with holes scalue up down
    scale_up(polyHole, 10000);
    std::cout << polyHole << std::endl;
    scale_down(polyHole, 3);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 5);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 7);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 13);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_up(polyHole, 3);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    pts.clear();
    pts.push_back(Point(11, 1));
    pts.push_back(Point(21, 11));
    pts.push_back(Point(11, 21));
    pts.push_back(Point(1, 11));
    polyHole.set(pts.begin(), pts.end());
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 3);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_up(polyHole, 10000);
    std::cout << polyHole << std::endl;
    scale_down(polyHole, 3);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 5);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 7);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 13);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_up(polyHole, 3);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;
    scale_down(polyHole, 2);
    std::cout << is_45(polyHole) << " " << polyHole << std::endl;
    if(is_45(polyHole) != true) return 1;

    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_up(polyWHoles, 100013);
    std::cout << polyWHoles << std::endl;
    scale_down(polyWHoles, 3);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 2);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 3);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 2);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 3);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 2);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 3);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 2);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 3);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 2);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 3);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 3);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 2);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 3);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 2);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 3);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;
    scale_down(polyWHoles, 2);
    std::cout << is_45(polyWHoles) << " " << polyWHoles << std::endl;
    if(is_45(polyWHoles) != true) return 1;

    std::cout << (boolean_op_45<Unit>::testScan45()) << std::endl;
    std::cout << (polygon_45_formation<Unit>::testPolygon45Formation()) << std::endl;
    std::cout << (polygon_45_formation<Unit>::testPolygon45Tiling()) << std::endl;


    {
      PolygonSet ps;
      Rectangle rect;
      ps.insert(Rectangle(0, 0, 10, 10));
      std::cout << area(ps) << std::endl;
      if(area(ps) != 100) return 1;
      scale_up(ps, 3);
      std::cout << area(ps) << std::endl;
      if(area(ps) != 900) return 1;
      scale_down(ps, 2);
      std::cout << area(ps) << std::endl;
      if(area(ps) != 225) return 1;
      transform(ps, atr);
      std::vector<Rectangle> rv;
      rv.clear();
      ps.get(rv);
      if(rv.size() == 1) {
        assign(rect, rv.back());
        std::cout << rect << std::endl;
      }
      transform(ps, tr);
      rv.clear();
      ps.get(rv);
      if(rv.size() == 1) {
        assign(rect, rv.back());
        std::cout << rect << std::endl;
      }
    }
//     //test polygon45set transform
//     pts.clear();
//     pts.push_back(Point(10, 10));
//     pts.push_back(Point(15, 10));
//     pts.push_back(Point(10, 15));
//     polyHole.set(pts.begin(), pts.end());
//     Polygon45Set ps451, ps452;
//     ps451.insert(polyHole);
//     ps452 = ps451;
//     std::cout << (ps451 == ps452) << std::endl;
//     if(ps451 != ps452) return 1;
//     ps451.transform(atr);
//     std::cout << (ps451 == ps452) << std::endl;
//     if(ps451 == ps452) return 1;
//     ps451.transform(tr);
//     std::cout << (ps451 == ps452) << std::endl;
//     if(ps451 != ps452) return 1;
  
//     //test polygon45set area
//     std::cout << ps451.area() << std::endl;
//     if(ps451.area() != 12.5) return 1;
//     //test polygon45set scale up down
//     ps451.scaleUp(3);
//     std::cout << ps451.area() << std::endl;
//     if(ps451.area() != 112.5) return 1;
//     ps451.scaleDown(2);
//     std::cout << ps451.area() << std::endl;
//     if(ps451.area() != 32) return 1;
//     //test polygonset scalue up down
  }
  {
    std::cout << (testPolygon45SetRect()) << std::endl;
    //testPolygon45SetPerterbation(); //re-enable after non-intersection fix
    testPolygon45Set();
    //testPolygon45SetDORA();  //re-enable after non-intersection fix
    polygon_45_set_data<int> ps45_1, ps45_2, ps45_3;
    ps45_1.insert(rectangle_data<int>(0, 0, 10, 10));
    ps45_2.insert(rectangle_data<int>(5, 5, 15, 15));
    std::vector<polygon_45_data<int> > p45s;
    ps45_3 = ps45_1 | ps45_2;
    ps45_3.get(p45s);
    if(p45s.size()) std::cout << p45s[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    p45s.clear();
    ps45_3 = ps45_1 + ps45_2;
    ps45_3.get(p45s);
    if(p45s.size()) std::cout << p45s[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    p45s.clear();
    ps45_3 = ps45_1 * ps45_2;
    ps45_3.get(p45s);
    if(p45s.size()) std::cout << p45s[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    p45s.clear();
    ps45_3 = ps45_1 - ps45_2;
    ps45_3.get(p45s);
    if(p45s.size()) std::cout << p45s[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    p45s.clear();
    ps45_3 = ps45_1 ^ ps45_2;
    ps45_3.get(p45s);
    if(p45s.size() == 2) std::cout << p45s[0] << " " << p45s[1] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    std::vector<point_data<int> > pts;
    pts.clear();
    pts.push_back(point_data<int>(7, 0));
    pts.push_back(point_data<int>(20, 13));
    pts.push_back(point_data<int>(0, 13));
    pts.push_back(point_data<int>(0, 0));
    polygon_45_data<int> p45_1(pts.begin(), pts.end());
    ps45_3.clear();
    ps45_3.insert(p45_1);
    p45s.clear();
    ps45_3.get(p45s);
    if(p45s.size()) std::cout << p45s[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    ps45_3 += 1;
    p45s.clear();
    ps45_3.get(p45s);
    if(p45s.size()) std::cout << p45s[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
    ps45_3 -= 1;
    p45s.clear();
    ps45_3.get(p45s);
    if(p45s.size()) std::cout << p45s[0] << std::endl;
    else {
      std::cout << "test failed\n";
      return 1;
    }
  }
  {
    polygon_90_set_data<int> p90sd;
    p90sd.insert(rectangle_data<int>(0, 0, 10, 10));
    std::vector<rectangle_data<int> > rects;
    std::vector<polygon_90_data<int> > polys90;
    std::vector<polygon_90_with_holes_data<int> > pwhs90;
    assign(rects, p90sd);
    assign(polys90, p90sd);
    assign(pwhs90, p90sd);
    std::cout << equivalence(rects, polys90) << std::endl;
    std::cout << equivalence(pwhs90, polys90) << std::endl;
    pwhs90.clear();
    assign(pwhs90, polys90);
    std::cout << equivalence(pwhs90, polys90) << std::endl;
  }
  {
    polygon_45_set_data<int> p45sd;
    p45sd.insert(rectangle_data<int>(0, 0, 10, 10));
    std::vector<rectangle_data<int> > rects;
    std::vector<polygon_45_data<int> > polys45;
    std::vector<polygon_45_with_holes_data<int> > pwhs45;
    get_trapezoids(polys45, p45sd);
    assign(polys45, p45sd);
    assign(pwhs45, p45sd);
    std::cout << equivalence(pwhs45, polys45) << std::endl;
    pwhs45.clear();
    assign(pwhs45, polys45);
    std::cout << equivalence(pwhs45, polys45) << std::endl;
  }
  {
    polygon_set_data<int> psd;
    psd.insert(rectangle_data<int>(0, 0, 10, 10));
    std::vector<polygon_data<int> > polys;
    std::vector<polygon_with_holes_data<int> > pwhs;
    assign(polys, psd);
    assign(pwhs, psd);
    std::cout << equivalence(pwhs, polys) << std::endl;
    pwhs.clear();
    assign(pwhs, polys);
    std::cout << equivalence(pwhs, polys) << std::endl;
  }
  {
    typedef point_3d_data<int> Point3D;
    Point3D p3d1(0, 1, 3), p3d2(0, 1, 2);
    if(equivalence(p3d1, p3d2)) return 1;
    if(euclidean_distance(p3d1, p3d2) != 1) return 1;
    if(euclidean_distance(p3d1, p3d2, PROXIMAL) != 1) return 1;
    if(manhattan_distance(p3d1, p3d2) != 1) return 1;
    assign(p3d1, p3d2);
    if(!equivalence(p3d1, p3d2)) return 1;
    p3d1 = construct<Point3D>(x(p3d1), y(p3d1), z(p3d1));
    if(!equivalence(p3d1, p3d2)) return 1;
    convolve(p3d1, p3d2);
    if(equivalence(p3d1, p3d2)) return 1;
    deconvolve(p3d1, p3d2);
    if(!equivalence(p3d1, p3d2)) return 1;
    if(get(p3d1, PROXIMAL) != 2) return 1;
    scale(p3d1, anisotropic_scale_factor<double>(2, 2, 2));
    if(equivalence(p3d1, p3d2)) return 1;
    scale_down(p3d1, 2);
    if(!equivalence(p3d1, p3d2)) return 1;
    scale_up(p3d1, 2);
    if(equivalence(p3d1, p3d2)) return 1;
    scale_down(p3d1, 2);
    set(p3d1, PROXIMAL, 3);
    if(equivalence(p3d1, p3d2)) return 1;
    axis_transformation atr = axis_transformation::END;
    transform(p3d1, atr);
    if(z(p3d1) != -3) return 1;
    z(p3d1, 2);
    if(!equivalence(p3d1, p3d2)) return 1;
  }
  {
    polygon_90_set_data<int> ps1(HORIZONTAL), ps2(VERTICAL);
    ps1 += rectangle_data<int>(0, 0, 10, 120);
    assign(ps1, ps2);
    std::cout << equivalence(ps1, ps2) << std::endl;
  }
  {
    std::vector<rectangle_data<long long> > lobs, input;
    input.push_back(rectangle_data<long long>(0, 0, 10, 10));
    input.push_back(rectangle_data<long long>(10, 5, 15, 15));
    get_max_rectangles(lobs, input);
    if(lobs.size() == 3) std::cout << "max rectangles is correct\n";
  }
  {
    polygon_set_data<int> ps1, ps2, ps3;
    ps1.insert(rectangle_data<int>(0, 0, 10, 10));
    ps2.insert(rectangle_data<int>(0, 0, 15, 5));
    ps3.insert(rectangle_data<int>(0, 0, 20, 2));
    std::cout << area(ps1 + ps2) << std::endl;
    keep(ps1, 0, 100, 0, 100, 0, 100);
    if(empty(ps1)) return 1;
    rectangle_data<int> bbox;
    extents(bbox, ps1);
    std::cout << bbox << std::endl;
    //resize(ps1, 1);
    //shrink(ps1, 1);
    //bloat(ps1, 1);
    scale_up(ps1, 2);
    scale_down(ps1, 2);
    axis_transformation atr;
    transform(ps1, atr);
    std::cout << area(ps1) << std::endl;
    if(area(ps1) != 100) return 1;
    clear(ps1);
    if(!empty(ps1)) return 1;
    ps1 = ps2 * ps3;
    ps1 *= ps2;
    ps1 - ps2;
    ps1 -= ps2;
    ps1 ^ ps2;
    ps1 ^= ps2;
    ps1 | ps2;
    ps1 |= ps2;
  }
  {
    polygon_45_set_data<int> ps45_1, ps45_2;
    ps45_1.insert(rectangle_data<int>(0, 0, 10, 10));
    keep(ps45_1, 0, 1000, 0, 1000, 0, 1000);
    std::cout << area(ps45_1) << std::endl;
    std::cout << empty(ps45_1) << std::endl;
    rectangle_data<int> bbox;
    extents(bbox, ps45_1);
    std::cout << bbox << std::endl;
    resize(ps45_1, 1);
    shrink(ps45_1, 1);
    bloat(ps45_1, 1);
    scale_up(ps45_1, 2);
    scale_down(ps45_1, 2);
    axis_transformation atr;
    transform(ps45_1, atr);
    std::cout << area(ps45_1) << std::endl;
    if(area(ps45_1) != 144) return 1;
    clear(ps45_1);
    if(!empty(ps45_1)) return 1;
  }
  {
    std::vector<polygon_45_data<int> > p45v;
    p45v + p45v;
    p45v *= p45v;
    p45v += p45v;
    p45v - p45v;
    p45v -= p45v;
    p45v ^ p45v;
    p45v ^= p45v;
    p45v | p45v;
    p45v |= p45v;
    p45v + 1;
    p45v += 1;
    p45v - 1;
    p45v -= 1;
    p45v + (p45v + p45v);
  }
  {
    polygon_45_set_data<int> ps45;
    polygon_90_set_data<int> ps90;
    std::vector<polygon_90_with_holes_data<int> > p90whv;
    ps45.insert(ps90);
    ps45.insert(p90whv);
    ps45.insert(p90whv + p90whv);
    
    ps45.insert(polygon_90_with_holes_data<int>());
    polygon_with_holes_data<int> pwh;
    snap_to_45(pwh);
  }
  {
    point_data<int> pt(1,2);
    point_3d_data<int> pt3d(1,2,3);
    equivalence(pt, pt3d);
    deconvolve(pt, pt3d);
    manhattan_distance(pt, pt3d);
    move(pt, HORIZONTAL, 1);
    scale(pt, anisotropic_scale_factor<double>(2, 2, 2));
    pt = pt3d;
  }
  {
    polygon_90_set_data<int> ps90_1, ps90_2;
    ps90_1.insert(rectangle_data<int>(0, 0, 10, 10));
    keep(ps90_1, 0, 1000, 0, 1000, 0, 1000);
    std::cout << area(ps90_1) << std::endl;
    std::cout << empty(ps90_1) << std::endl;
    rectangle_data<int> bbox;
    extents(bbox, ps90_1);
    std::cout << bbox << std::endl;
    resize(ps90_1, 1);
    shrink(ps90_1, 1);
    bloat(ps90_1, 1);
    scale_up(ps90_1, 2);
    scale_down(ps90_1, 2);
    scale(ps90_1, anisotropic_scale_factor<double>(2, 2, 2));
    scale(ps90_1, anisotropic_scale_factor<double>(0.5, 0.5, 0.5));
    axis_transformation atr;
    transform(ps90_1, atr);
    std::cout << area(ps90_1) << std::endl;
    if(area(ps90_1) != 144) return 1;
    clear(ps90_1);
    if(!empty(ps90_1)) return 1;
  }
  if(!nonInteger45StessTest()) return 1;
  std::cout << "ALL TESTS COMPLETE\n";
  return 0;
}
