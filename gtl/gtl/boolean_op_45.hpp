/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_BOOLEAN_OP_45_HPP
#define GTL_BOOLEAN_OP_45_HPP
namespace gtl {

  template <typename Unit>
  struct boolean_op_45 {
    typedef point_data<Unit> Point;
    typedef typename coordinate_traits<Unit>::manhattan_area_type LongUnit;

    class Count2 {
    public:
      inline Count2() { counts[0] = counts[1] = 0; }
      //inline Count2(int count) { counts[0] = counts[1] = count; }
      inline Count2(int count1, int count2) { counts[0] = count1; counts[1] = count2; }
      inline Count2(const Count2& count) { counts[0] = count.counts[0]; counts[1] = count.counts[1]; }
      inline bool operator==(const Count2& count) const { return counts[0] == count.counts[0] && counts[1] == count.counts[1]; }
      inline bool operator!=(const Count2& count) const { return !((*this) == count); }
      inline Count2& operator=(int count) { counts[0] = counts[1] = count; return *this; }
      inline Count2& operator=(const Count2& count) { counts[0] = count.counts[0]; counts[1] = count.counts[1]; return *this; }
      inline int& operator[](bool index) { return counts[index]; }
      inline int operator[](bool index) const {return counts[index]; }
      inline Count2& operator+=(const Count2& count){
        counts[0] += count[0];
        counts[1] += count[1];
        return *this;
      }
      inline Count2& operator-=(const Count2& count){
        counts[0] -= count[0];
        counts[1] -= count[1];
        return *this;
      }
      inline Count2 operator+(const Count2& count) const {
        return Count2(*this)+=count;
      }
      inline Count2 operator-(const Count2& count) const {
        return Count2(*this)-=count;
      }
      inline Count2 invert() const {
        return Count2(-counts[0], -counts[1]);
      }
    private:
      int counts[2];
    };

//     inline std::ostream& operator<< (std::ostream& o, const Count2& c) {
//       o << c[0] << " " << c[1];
//       return o;
//     }

    class Scan45Element {
    public:
      Unit x;
      Unit y;
      int rise; //-1, 0, +1
      mutable Count2 count;
      inline Scan45Element(){}
      inline Scan45Element(Unit xIn, Unit yIn, int riseIn, Count2 countIn = Count2(0, 0)) :
        x(xIn), y(yIn), rise(riseIn), count(countIn) {}
      inline Scan45Element(const Scan45Element& that) :
        x(that.x), y(that.y), rise(that.rise), count(that.count) {}
      inline Scan45Element& operator=(const Scan45Element& that) {
        x = that.x; y = that.y; rise = that.rise; count = that.count; 
        return *this;
      }
      inline Unit evalAtX(Unit xIn) const {
        return y + rise * (xIn - x);
      }

      inline bool cross(Point& crossPoint, const Scan45Element& edge, Unit currentX) const {
        Unit y1 = evalAtX(currentX);
        Unit y2 = edge.evalAtX(currentX);
        int rise1 = rise;
        int rise2 = edge.rise;
        if(rise > edge.rise){
          if(y1 > y2) return false;
        } else if(rise < edge.rise){
          if(y2 > y1) return false;
          std::swap(y1, y2);
          std::swap(rise1, rise2);
        } else { return false; }
        if(rise1 == 1) {
          if(rise2 == 0) {
            crossPoint = Point(currentX + y2 - y1, y2);
          } else {
            //rise2 == -1
            Unit delta = (y2 - y1)/2;
            crossPoint = Point(currentX + delta, y1 + delta);
          }
        } else {
          //rise1 == 0 and rise2 == -1
          crossPoint = Point(currentX + y2 - y1, y1);
        }
        return true;
      }
    };

//     inline std::ostream& operator<< (std::ostream& o, const Scan45Element& c) {
//       o << c.x << " " << c.y << " " << c.rise << " " << c.count;
//       return o;
//     }

    class lessScan45ElementRise : public std::binary_function<Scan45Element, Scan45Element, bool> {
    public:
      inline lessScan45ElementRise() {} //default constructor is only constructor
      inline bool operator () (Scan45Element elm1, Scan45Element elm2) const {
        return elm1.rise < elm2.rise;
      }
    };

    class lessScan45Element : public std::binary_function<Scan45Element, Scan45Element, bool> {
    private:
      Unit *x_; //x value at which to apply comparison
      int *justBefore_;
    public:
      inline lessScan45Element() : x_(0) {}
      inline lessScan45Element(Unit *x, int *justBefore) : x_(x), justBefore_(justBefore) {}
      inline lessScan45Element(const lessScan45Element& that) : x_(that.x_), justBefore_(that.justBefore_) {}
      inline lessScan45Element& operator=(const lessScan45Element& that) { x_ = that.x_; justBefore_ = that.justBefore_; return *this; }
      inline bool operator () (const Scan45Element& elm1, const Scan45Element& elm2) const {
        Unit y1 = elm1.evalAtX(*x_);
        Unit y2 = elm2.evalAtX(*x_);
        if(y1 < y2) return true;
        if(y1 == y2) {
          //if justBefore is true we invert the result of the comparison of slopes
          if(*justBefore_) {
            return elm1.rise > elm2.rise;
          } else {
            return elm1.rise < elm2.rise;
          }
        }
        return false;
      }
    };

    class Scan45Count {
    public:
      inline Scan45Count() { counts[0] = counts[1] = counts[2] = counts[3] = 0; }
      inline Scan45Count(Count2 count) { counts[0] = counts[1] = counts[2] = counts[3] = count; }
      inline Scan45Count(const Count2& count1, const Count2& count2, const Count2& count3, 
                         const Count2& count4) { 
        counts[0] = count1; 
        counts[1] = count2; 
        counts[2] = count3;
        counts[3] = count4; 
      }
      inline Scan45Count(const Scan45Count& count) { 
        (*this) = count;
      }
      inline bool operator==(const Scan45Count& count) const { 
        for(unsigned int i = 0; i < 4; ++i) {
          if(counts[i] != count.counts[i]) return false; 
        }
        return true;
      }
      inline bool operator!=(const Scan45Count& count) const { return !((*this) == count); }
      inline Scan45Count& operator=(Count2 count) { 
        counts[0] = counts[1] = counts[2] = counts[3] = count; return *this; }
      inline Scan45Count& operator=(const Scan45Count& count) {
        for(unsigned int i = 0; i < 4; ++i) {
          counts[i] = count.counts[i]; 
        }
        return *this; 
      }
      inline Count2& operator[](int index) { return counts[index]; }
      inline Count2 operator[](int index) const {return counts[index]; }
      inline Scan45Count& operator+=(const Scan45Count& count){
        for(unsigned int i = 0; i < 4; ++i) {
          counts[i] += count.counts[i]; 
        }
        return *this;
      }
      inline Scan45Count& operator-=(const Scan45Count& count){
        for(unsigned int i = 0; i < 4; ++i) {
          counts[i] -= count.counts[i]; 
        }
        return *this;
      }
      inline Scan45Count operator+(const Scan45Count& count) const {
        return Scan45Count(*this)+=count;
      }
      inline Scan45Count operator-(const Scan45Count& count) const {
        return Scan45Count(*this)-=count;
      }
      inline Scan45Count invert() const {
        return Scan45Count(Count2(0,0))-=(*this);
      }
      inline Scan45Count& operator+=(const Scan45Element& element){
        counts[element.rise+1] += element.count; return *this;
      }
    private:
      Count2 counts[4];
    };

//     inline std::ostream& operator<< (std::ostream& o, const Scan45Count& c) {
//       o << c[0] << ", " << c[1] << ", ";
//       o << c[2] << ", " << c[3];
//       return o;
//     }

    typedef std::pair<Point, Scan45Count> Scan45Vertex;

//     inline std::ostream& operator<< (std::ostream& o, const Scan45Vertex& c) {
//       o << c.first << ": " << c.second;
//       return o;
//     }

    //index is the index into the vertex
    static inline Scan45Element getElement(const Scan45Vertex& vertex, int index) {
      return Scan45Element(vertex.first.x(), vertex.first.y(), index - 1, vertex.second[index]);
    }

    class lessScan45Point : public std::binary_function<Point, Point, bool> {
    public:
      inline lessScan45Point() {} //default constructor is only constructor
      inline bool operator () (const Point& v1, const Point& v2) const {
        return (v1.x() < v2.x()) || (v1.x() == v2.x() && v1.y() < v2.y());
      }
    };

    class lessScan45Vertex : public std::binary_function<Scan45Vertex, Scan45Vertex, bool> {
    public:
      inline lessScan45Vertex() {} //default constructor is only constructor
      inline bool operator () (const Scan45Vertex& v1, const Scan45Vertex& v2) const {
        return (v1.first.x() < v2.first.x()) || (v1.first.x() == v2.first.x() && v1.first.y() < v2.first.y());
      }
    };

    typedef std::vector<Scan45Vertex> Scan45Vector;

    static inline void sortScan45Vector(Scan45Vector& vec) {
      std::sort(vec.begin(), vec.end(), lessScan45Vertex());
    }

    //vetex45 is sortable
    class Vertex45 {
    public:
      Point pt;
      int rise; // 1, 0 or -1
      int count; //dxdydTheta
      inline Vertex45() {}
      inline Vertex45(const Point& point, int riseIn, int countIn) : pt(point), rise(riseIn), count(countIn) {}
      inline Vertex45(const Vertex45& vertex) : pt(vertex.pt), rise(vertex.rise), count(vertex.count) {}
      inline Vertex45& operator=(const Vertex45& vertex){ 
        pt = vertex.pt; rise = vertex.rise; count = vertex.count; return *this; }
      inline Vertex45(const std::pair<Point, Point>& vertex) {}
      inline Vertex45& operator=(const std::pair<Point, Point>& vertex){ return *this; }
      inline bool operator==(const Vertex45& vertex) const {
        return pt == vertex.pt && rise == vertex.rise && count == vertex.count; }
      inline bool operator!=(const Vertex45& vertex) const { return !((*this) == vertex); }
      inline bool operator==(const std::pair<Point, Point>& vertex) const { return false; }
      inline bool operator!=(const std::pair<Point, Point>& vertex) const { return !((*this) == vertex); }
      inline bool operator<(const Vertex45& vertex) const {
        if(pt.x() < vertex.pt.x()) return true;
        if(pt.x() == vertex.pt.x()) {
          if(pt.y() < vertex.pt.y()) return true;
          if(pt.y() == vertex.pt.y()) { return rise < vertex.rise; }
        }
        return false;
      }
      inline bool operator>(const Vertex45& vertex) const { return vertex < (*this); }
      inline bool operator<=(const Vertex45& vertex) const { return !((*this) > vertex); }
      inline bool operator>=(const Vertex45& vertex) const { return !((*this) < vertex); }
      inline Unit evalAtX(Unit xIn) const { return pt.y() + rise * (xIn - pt.x()); }
    };

//     inline std::ostream& operator<< (std::ostream& o, const Vertex45& c) {
//       o << c.pt << " " << c.rise << " " << c.count;
//       return o;
//     }

    //when scanning Vertex45 for polygon formation we need a scanline comparator functor
    class lessVertex45 : public std::binary_function<Vertex45, Vertex45, bool> {
    private:
      Unit *x_; //x value at which to apply comparison
      int *justBefore_;
    public:
      inline lessVertex45() : x_(0) {}
      inline lessVertex45(Unit *x, int *justBefore) : x_(x), justBefore_(justBefore) {}
      inline lessVertex45(const lessVertex45& that) : x_(that.x_), justBefore_(that.justBefore_) {}
      inline lessVertex45& operator=(const lessVertex45& that) { x_ = that.x_; justBefore_ = that.justBefore_; return *this; }
      inline bool operator () (const Vertex45& elm1, const Vertex45& elm2) const {
        Unit y1 = elm1.evalAtX(*x_);
        Unit y2 = elm2.evalAtX(*x_);
        if(y1 < y2) return true;
        if(y1 == y2) {
          //if justBefore is true we invert the result of the comparison of slopes
          if(*justBefore_) {
            return elm1.rise > elm2.rise;
          } else {
            return elm1.rise < elm2.rise;
          }
        }
        return false;
      }
    };

    // 0 right to left
    // 1 upper right to lower left
    // 2 high to low
    // 3 upper left to lower right
    // 4 left to right
    // 5 lower left to upper right
    // 6 low to high
    // 7 lower right to upper left
    static inline int classifyEdge45(const Point& prevPt, const Point& nextPt) {
      if(prevPt.x() == nextPt.x()) {
        //2 or 6
        return predicated_value(prevPt.y() < nextPt.y(), 6, 2);
      }
      if(prevPt.y() == nextPt.y()) {
        //0 or 4
        return predicated_value(prevPt.x() < nextPt.x(), 4, 0);
      }
      if(prevPt.x() < nextPt.x()) {
        //3 or 5
        return predicated_value(prevPt.y() < nextPt.y(), 5, 3);
      }
      //prevPt.x() > nextPt.y()
      //1 or 7
      return predicated_value(prevPt.y() < nextPt.y(), 7, 1);
    }

    class Scan45 {
    private:
      //definitions
      typedef std::set<Scan45Element, lessScan45Element> Scan45Data;
      typedef typename Scan45Data::iterator iterator;
      typedef typename Scan45Data::const_iterator const_iterator;
      typedef std::set<Point, lessScan45Point> CrossQueue;
   
      //data
      Scan45Data scanData_;
      CrossQueue crossQueue_;
      Scan45Vector crossVector_;
      Scan45Vector nonIntegerIntersectionVector_;
      Unit x_;
      int justBefore_;
      int op_; // 0 for OR, 1 for AND, 2 for subtract and 3 for xor
    public:
      inline Scan45() : x_((std::numeric_limits<Unit>::min)()), justBefore_(false), op_(0) {
        lessScan45Element lessElm(&x_, &justBefore_);
        scanData_ = std::set<Scan45Element, lessScan45Element>(lessElm);
      }
      inline Scan45(int op) : x_((std::numeric_limits<Unit>::min)()), justBefore_(false), op_(op) {
        lessScan45Element lessElm(&x_, &justBefore_);
        scanData_ = std::set<Scan45Element, lessScan45Element>(lessElm);
      }
      inline Scan45(const Scan45& that) { (*this) = that; }
      inline Scan45& operator=(const Scan45& that) {
        x_ = that.x_;
        justBefore_ = that.justBefore_;
        crossQueue_ = that.crossQueue_; 
        crossVector_ = that.crossVector_; 
        op_ = that.op_;
        lessScan45Element lessElm(&x_, &justBefore_);
        scanData_ = std::set<Scan45Element, lessScan45Element>(lessElm);
        for(const_iterator itr = that.scanData_.begin(); itr != that.scanData_.end(); ++itr){
          scanData_.insert(scanData_.end(), *itr);
        }
        return *this;
      }

//       bool check_invariant() {
//         lessScan45Element lessElm(&x_, &justBefore_);
//         typename Scan45Data::iterator previtr = scanData_.begin();
//         for(typename Scan45Data::iterator itr = scanData_.begin(); itr != scanData_.end(); ++itr) {
//           if(itr != scanData_.begin()) {
//             if(lessElm(*itr, *previtr)) return false;
//           }
//           previtr = itr;
//         }
//         return true;
//       }
//       void assert_invariant() {
//         if(!check_invariant()) {
//           std::cout << "tree invariant violated at " << x_ << " just before is " << justBefore_ << std::endl; //break here
//         }
//       }

      //cT is an output container of Vertex45
      //iT is an iterator over Scan45Vertex elements
      template <class cT, class iT>
      void scan(cT& output, iT inputBegin, iT inputEnd) {
        //std::cout << "1\n";
        while(inputBegin != inputEnd) {
          //std::cout << "2\n";
          //std::cout << "x_ = " << x_ << std::endl;
          //std::cout << "scan line size: " << scanData_.size() << std::endl;
          //for(iterator iter = scanData_.begin();
          //     iter != scanData_.end(); ++iter) {
          //   std::cout << "scan element\n";
          //   std::cout << *iter << " " << iter->evalAtX(x_) << std::endl;
          // }
          // std::cout << "cross queue size: " << crossQueue_.size() << std::endl;
          // std::cout << "cross vector size: " << crossVector_.size() << std::endl;
          //for(CrossQueue::iterator cqitr = crossQueue_.begin(); cqitr != crossQueue_.end(); ++cqitr) {
          //   std::cout << *cqitr << " ";
          //} std::cout << std::endl;
          Unit nextX = (*inputBegin).first.x();
          if(!crossVector_.empty() && crossVector_[0].first.x() < nextX) nextX = crossVector_[0].first.x();
          if(nextX != x_) {
            //std::cout << "3\n";
            //we need to move to the next scanline stop
            //we need to process cross events, set scanline to the lowest x of the cross events
            //if(nextX == 48032) { //48031
              //std::cout << "at bad x, current x is " << x_ << std::endl; //break here
            //}
            if(!crossQueue_.empty() &&
               (*crossQueue_.begin()).x() < nextX) {
              //std::cout << "4\n";
              nextX = std::min(nextX, (*crossQueue_.begin()).x());
            }
            //set scanline to the x of the nonIntegerEvents (should be x_ + 1)
            if(!nonIntegerIntersectionVector_.empty() &&
               (*nonIntegerIntersectionVector_.begin()).first.x() < nextX) {
              nextX = (*nonIntegerIntersectionVector_.begin()).first.x();
            }
            //std::cout << "6\n";
            //assert_invariant();
            justBefore_ = true;
            x_ = nextX;
            //assert_invariant();
            advance_(output);
            //merge the non integer intersection events with the cross events
            mergeCross_(nonIntegerIntersectionVector_.begin(), nonIntegerIntersectionVector_.end());
            nonIntegerIntersectionVector_.clear();
            //assert_invariant();
            justBefore_ = false;
            //assert_invariant();
            if(!crossVector_.empty() &&
               nextX == (*inputBegin).first.x()) {
              inputBegin = mergeCross_(inputBegin, inputEnd);
            }
            processEvent_(output, crossVector_.begin(), crossVector_.end());
            crossVector_.clear();
          } else {
            //std::cout << "7\n";
            //our scanline has progressed to the event that is next in the queue
            inputBegin = processEvent_(output, inputBegin, inputEnd);
          }
        }
        //std::cout << "done scanning\n";
      }

    private:
      //functions

      template <class cT>
      inline void advance_(cT& output) {
        //process all cross points on the cross queue at the current x_
        //std::cout << "advance_\n";
        std::vector<iterator> eraseVec;
        while(!crossQueue_.empty() &&
              (*crossQueue_.begin()).x() == x_){
          //std::cout << "loop\n";
          //pop point off the cross queue
          Point crossPoint = *(crossQueue_.begin());
          //std::cout << crossPoint << std::endl;
          //for(iterator iter = scanData_.begin();
          //    iter != scanData_.end(); ++iter) {
          //  std::cout << "scan element\n";
          //  std::cout << *iter << " " << iter->evalAtX(x_) << std::endl;
          //}
          crossQueue_.erase(crossQueue_.begin());
          Scan45Vertex vertex(crossPoint, Scan45Count());
          iterator lowIter = lookUp_(vertex.first.y());
          //std::cout << "searching at: " << vertex.first.y() << std::endl;
          //if(lowIter == scanData_.end()) std::cout << "could not find\n";
          //else std::cout << "found: " << *lowIter << std::endl;
          if(lowIter == scanData_.end() ||
             lowIter->evalAtX(x_) != vertex.first.y()) {
            //   std::cout << "skipping\n";
            //there weren't any edges at this potential cross point
            continue;
          }
          Count2 countBelow(0, 0);
          iterator searchDownItr = lowIter;
          while(searchDownItr != scanData_.begin()
                && searchDownItr->evalAtX(x_) == vertex.first.y()) {
            //get count from below
            --searchDownItr;
            countBelow = searchDownItr->count;
          }
          //std::cout << "Below Count: " << countBelow << std::endl;
          Scan45Count count(countBelow);
          unsigned int numEdges = 0;
          iterator eraseItrs[3];
          while(lowIter != scanData_.end() &&
                lowIter->evalAtX(x_) == vertex.first.y()) {
            for(int index = lowIter->rise +1; index >= 0; --index)
              count[index] = lowIter->count;
            //std::cout << count << std::endl;
            eraseItrs[numEdges] = lowIter;
            ++numEdges;
            ++lowIter;
          }
          if(numEdges == 1) {
            //look for the next crossing point and continue
            //std::cout << "found only one edge\n";
            //if findCross_ returns true it means the iterator is about to cross its neighbor at non integer
            //point, we need to handle this by faking an event
            if(!findCross_(eraseItrs[0])) {
              continue;
            }
          }
          //before we erase the elements we need to decide if they should be written out
          Count2 currentCount = countBelow;
          for(unsigned int i = 0; i < numEdges; ++i) {
            int edgeType = applyLogic(currentCount, eraseItrs[i]->count);
            //std::cout << "cross logic: " << edgeType << std::endl;
            if(edgeType == 1) {
              output.insert(output.end(), Vertex45(crossPoint, eraseItrs[i]->rise, -1));
              //output.insert(output.end(), std::pair<Point, Point>(Point(eraseItrs[i]->x, eraseItrs[i]->y),
              //                                                    crossPoint));
              //std::cout << "write out: " << Point(eraseItrs[i]->x, eraseItrs[i]->y) << " " << crossPoint << std::endl;
            } else if(edgeType == -1) {
              output.insert(output.end(), Vertex45(crossPoint, eraseItrs[i]->rise, 1));
              //output.insert(output.end(), std::pair<Point, Point>(crossPoint,
              //                                     Point(eraseItrs[i]->x, eraseItrs[i]->y)));
              //std::cout << "write out: " << crossPoint << " " << Point(eraseItrs[i]->x, eraseItrs[i]->y) << std::endl;
            }
            currentCount = eraseItrs[i]->count;
          }
          //schedule erase of the elements
          for(unsigned int i = 0; i < numEdges; ++i) {
            eraseVec.push_back(eraseItrs[i]);
          }
         
          //take the derivative wrt theta of the count at the crossing point
          vertex.second[2] = count[2] - countBelow;
          vertex.second[1] = count[1] - count[2];
          vertex.second[0] = count[0] - count[1];
          //add the point, deriviative pair into the cross vector
          //std::cout << "LOOK HERE!\n";
          //std::cout << count << std::endl;
          //std::cout << vertex << std::endl;
          crossVector_.push_back(vertex);
        }
        //erase crossing elements
        std::vector<iterator> searchVec;
        //assert_invariant();
        for(unsigned int i = 0; i < eraseVec.size(); ++i) {
          //when erasing an element we need to see if the previous element will cross
          //the next element, so add the previous element to the search list
          if(eraseVec[i] != scanData_.begin()) {
            iterator searchItr = eraseVec[i];
            --searchItr;
            if(searchVec.empty() ||
               searchVec.back() != searchItr)
              searchVec.push_back(searchItr);
          }
          scanData_.erase(eraseVec[i]);
        }
        //assert_invariant();
        //have to search for edges on either side of removed elements
        //crossing only after all elements were removed because two
        //removed elements may be adjacent
        for(unsigned int i = 0; i < searchVec.size(); ++i) {
          findCross_(searchVec[i]);
        }
      }
   
      template <class iT>
      inline iT mergeCross_(iT inputBegin, iT inputEnd) {
        Scan45Vector vec;
        std::swap(vec, crossVector_);
        iT mergeEnd = inputBegin;
        unsigned int mergeCount = 0;
        while(mergeEnd != inputEnd &&
              (*mergeEnd).first.x() == x_) {
          ++mergeCount;
          ++mergeEnd;
        }
        crossVector_.reserve(std::max(vec.capacity(), vec.size() + mergeCount));
        for(unsigned int i = 0; i < vec.size(); ++i){
          while(inputBegin != mergeEnd &&
                (*inputBegin).first.y() < vec[i].first.y()) {
            crossVector_.push_back(*inputBegin);
            ++inputBegin;
          }
          crossVector_.push_back(vec[i]);
        }
        while(inputBegin != mergeEnd){
          crossVector_.push_back(*inputBegin);
          ++inputBegin;
        }
        return inputBegin;
      }
   
      template <class cT, class iT>
      inline iT processEvent_(cT& output, iT inputBegin, iT inputEnd) {
        //std::cout << "processEvent_\n";
        //if(x_ == 48031) {
        //  std::cout << "processing bad x event 48031\n";
        //}
        Count2 verticalCount = Count2(0, 0);
        Point prevPoint;
        iterator prevIter = scanData_.end();
        Scan45Vertex niip_0, niip_1, niip_2;
        bool prevNonIntegerIntersection = false;
        Count2 upwardSlopingCount(0, 0);
        bool downwardSlash = false;
        Count2 downwardSlopingCount;
        Count2 downwardSlopingCountFromScanline;
        bool routeThroughUpperLeft = false;
        bool haveDeferredVertex = false;
        Scan45Vertex deferredVertex;
        Unit prevY = std::numeric_limits<Unit>::max();
        while(prevNonIntegerIntersection || haveDeferredVertex || (inputBegin != inputEnd && (*inputBegin).first.x() == x_)) {
          //std::cout << (*inputBegin) << std::endl;
          //std::cout << "loop\n";
          Scan45Vertex vertex;
          if(inputBegin != inputEnd)
            vertex = *inputBegin;
          //std::cout << vertex.first << std::endl;
          //if vertical count propigating up fake a null event at the next element
          if(haveDeferredVertex) {
            vertex = deferredVertex;
            haveDeferredVertex = false;
          } else {
            if(inputBegin == inputEnd || 
               (prevNonIntegerIntersection && (*inputBegin).first.y() > prevY+1) ||
               (verticalCount != Count2(0, 0) && (prevIter != scanData_.end() &&
                                                  prevIter->evalAtX(x_) < (*inputBegin).first.y()))) {
              //std::cout << "faking null event\n";
              vertex = Scan45Vertex(Point(x_, prevIter->evalAtX(x_)), Scan45Count());
            } else { 
              ++inputBegin; 
              //std::cout << "after increment\n";
              //accumulate overlapping changes in Scan45Count
              while(inputBegin != inputEnd &&
                    (*inputBegin).first.x() == x_ && 
                    (*inputBegin).first.y() == vertex.first.y()) {
                //std::cout << "accumulate\n";
                vertex.second += (*inputBegin).second;
                ++inputBegin;
              }
            }
          }
          Scan45Vertex t_niip_1, t_niip_2;
          bool currentNonIntegerIntersection = false;
          //check for down sloping input and up sloping
          //scanline element below it
          if(prevNonIntegerIntersection) {
            if(!downwardSlash) {
              //modify the input vertex here to re-route the downward sloping 45 edge
              //insert events at x_ + 1 to complete re-routing the downward sloping 45 edge
              //down sloping edge reroutes to the right, then down, then downward sloping
              vertex.second[0] = Count2(0, 0);
              vertex.second[0] -= downwardSlopingCountFromScanline;
              vertex.second[1] += downwardSlopingCount;
              niip_1.second[1] -= downwardSlopingCount;
              niip_1.second[3] -= downwardSlopingCount;
              niip_0.second[3] += downwardSlopingCount;
              niip_0.second[0] += downwardSlopingCount;
            }
            if(routeThroughUpperLeft) {
              vertex.second[3] += upwardSlopingCount;
              vertex.second[1] += upwardSlopingCount;
            }
            downwardSlopingCount = Count2(0, 0);
          } else {
            //check for down sloping input and up sloping
            //scanline element below it
            downwardSlopingCount = Count2(0, 0);
            if(vertex.second[0] != Count2(0, 0)) {
              iterator lowIter = lookUp_(vertex.first.y());
              if(lowIter != scanData_.begin()) {
                //get count from below
                --lowIter;
                if(vertex.first.y() - 1 != prevY && lowIter->rise == 1 && lowIter->evalAtX(x_) == vertex.first.y() - 1) {
                  //we have a upward sloping element in the scanline just below the input
                  //downward sloping edge at vertex
                  deferredVertex = vertex;
                  downwardSlopingCount = deferredVertex.second[0];
                  haveDeferredVertex = true;
                  vertex.first = Point(x_, vertex.first.y() - 1);
                  //prevY = vertex.first.y();
                  vertex.second = Scan45Count();
                  prevIter = lowIter;
//                   while(lowIter->evalAtX(x_) == vertex.first.y()) {
//                     unsigned int indexAt = lowIter->rise+1;
//                     vertex.second[indexAt] = lowIter->count;
//                     if(lowIter == scanData_.begin()) break;
//                     --lowIter;
//                     vertex.second[indexAt] -= lowIter->count;
//                   }
                  //while(!output.empty() &&
                  //      output.back().pt == vertex.first)
                  //  output.pop_back();
                }
              }
            }
          }
          prevY = vertex.first.y();
          if(!haveDeferredVertex)
            prevIter = lookUp_(vertex.first.y()-1);
          if(vertex.second[2] != Count2(0, 0) ||
             (prevIter != scanData_.end() && prevIter->rise == 1 &&
              prevIter->evalAtX(x_) == vertex.first.y())) {
            //look up any downward sloping edge in the tree
            downwardSlopingCountFromScanline = Count2(0, 0);
            iterator upIter = lookUp_(vertex.first.y() + 1);
            if(upIter != scanData_.end() && upIter->evalAtX(x_) == vertex.first.y() + 1) {
              if(upIter->rise == -1) {
                downwardSlopingCountFromScanline += upIter->count;
                if(upIter != scanData_.begin()) {
                  --upIter;
                  //we want the change in count at the edge
                  downwardSlopingCountFromScanline -= (upIter->count - verticalCount);
                }
              }
            }
            downwardSlopingCount += downwardSlopingCountFromScanline;
            if(downwardSlopingCount != Count2(0, 0) || 
              (inputBegin != inputEnd && //not at end of input range
               (*inputBegin).first.x() == x_ &&  //at current scanline stop
               (*inputBegin).first.y() == vertex.first.y() + 1)) { //at next integral y position
              iT tmpInputItr = inputBegin;
              while(tmpInputItr != inputEnd && //not at end of input range
                    (*tmpInputItr).first.x() == x_ &&  //at current scanline stop
                    (*tmpInputItr).first.y() == vertex.first.y() + 1) { //at next integral y position
                downwardSlopingCount += (*tmpInputItr).second[0];
                ++tmpInputItr;
              }
              if(downwardSlopingCount != Count2(0, 0)) {
                //we have hit a non integer intersection between 45 edges at this point in scanning
                //we need to massage the input to "route traffic" around the non-integer point
                //we can insert new input events into nonIntegerIntersectionVector_ to be handled at x_ + 1
                //we can modify the input events at the current vertex, next vertex and vertical count
                currentNonIntegerIntersection = true;
                prevY = vertex.first.y();
                upwardSlopingCount = vertex.second[2];
                Count2 countBelow;
                iterator lowIter = lookUp_(vertex.first.y());
                Count2 upwardSlopingCountFromScanline = Count2(0, 0);
                bool foundUpwardElement = false;
                if(lowIter != scanData_.end() &&
                   lowIter->rise == 1) {
                  upwardSlopingCountFromScanline = lowIter->count;
                  foundUpwardElement = true;
                }
                if(lowIter != scanData_.begin()) {
                  //get count from below
                  --lowIter;
                  countBelow = lowIter->count;
                  ++lowIter;
                }
                if(foundUpwardElement)
                  upwardSlopingCountFromScanline -= (countBelow - verticalCount);
                upwardSlopingCount += upwardSlopingCountFromScanline;
                //if there is an up sloping edge in the scanline
                //at this y location then we may need to include
                //it in our calculations of counts
                Count2 countOnBottom(countBelow);
                countOnBottom += vertex.second[0];
                countOnBottom += vertex.second[1];
                Count2 countOnLeft = countOnBottom + upwardSlopingCount;
                Count2 countOnTop = countOnLeft + downwardSlopingCount;
                Count2 countOnRight = countOnBottom + downwardSlopingCount;
                bool lb = applyLogic(countOnBottom);
                bool lt = applyLogic(countOnTop);
                bool ll = applyLogic(countOnLeft);
                bool lr = applyLogic(countOnRight);
                if(lb == ll && lt == lr && lt != lb) {
                  downwardSlash = true;
                } else {
                  downwardSlash = false;
                }
                //modify the input vertex here to re-route the upward sloping 45 edge
                //insert events at x_ + 1 to complete re-routing the upward sloping 45 edge
                t_niip_1.first = Point(x_ + 1, vertex.first.y());
                t_niip_2.first = Point(x_ + 1, vertex.first.y() + 1);
                routeThroughUpperLeft = false;
                if((lb && lr && !ll && !lt) ||
                   (!lb && !lr && ll && lt)) {
                  //upward slash case, no rerouting needed
                  lb = lr; //break here if needed
                } else {
                  if((!ll && !lr) ||
                     (ll && lt && lr && !lb) ||
                     (ll && lt && !lr && lb) ||
                     (ll && !lt && lr && lb) ||
                     (!ll && !lt && lr && !lb)) {
                    //route right then up then upward sloping
                    vertex.second[2] = Count2(0, 0);
                    vertex.second[2] -= upwardSlopingCountFromScanline;
                    vertex.second[1] += upwardSlopingCount;
                    t_niip_1.second[1] -= upwardSlopingCount;
                    t_niip_1.second[3] -= upwardSlopingCount;
                    t_niip_2.second[3] += upwardSlopingCount;
                    t_niip_2.second[2] += upwardSlopingCount;
                  } else {
                    //route up then right then upward sloping
                    routeThroughUpperLeft = true;
                    vertex.second[2] = Count2(0, 0);
                    vertex.second[2] -= upwardSlopingCountFromScanline;
                    vertex.second[3] -= upwardSlopingCount;
                    t_niip_2.second[1] -= upwardSlopingCount;
                    t_niip_2.second[2] += upwardSlopingCount;
                  }
                }

              }
            }
          }
          //write out non integer intersection events
          if(currentNonIntegerIntersection || prevNonIntegerIntersection) {
            if(niip_0.second != Scan45Count()) {
              //write out count below current y
              nonIntegerIntersectionVector_.push_back(niip_0);
              niip_0 = Scan45Vertex();
            }
            if(!currentNonIntegerIntersection) {
              if(niip_1.second != Scan45Count()) {
                //write out count at y
                nonIntegerIntersectionVector_.push_back(niip_1);
                niip_1 = Scan45Vertex();
              }
            }
            //rotate right side non integer intersection point outputs downward
            niip_0 = t_niip_1;
            niip_1 = t_niip_2;
            niip_2 = Scan45Vertex();
          }
          //std::cout << vertex.second << std::endl;
          //integrate vertex
          Count2 currentCount = verticalCount;// + vertex.second[0];
          for(unsigned int i = 0; i < 3; ++i) {
            vertex.second[i] = currentCount += vertex.second[i];
          }
          //std::cout << vertex.second << std::endl;
          //vertex represents the change in state at this point
         
          //get counts at current vertex
          Count2 countBelow;
          iterator lowIter = lookUp_(vertex.first.y());
          if(lowIter != scanData_.begin()) {
            //get count from below
            --lowIter;
            countBelow = lowIter->count;
            ++lowIter;
          }
          //std::cout << "Count Below: " << countBelow[0] << " " << countBelow[1] << std::endl;
          //std::cout << "vertical count: " << verticalCount[0] << " " << verticalCount[1] << std::endl;
          Scan45Count countAt(countBelow - verticalCount);
          //check if the vertical edge should be written out
          if(verticalCount != Count2(0, 0)) {
            int edgeType = applyLogic(countBelow - verticalCount, countBelow);
            //std::cout << "vertical logic: " << edgeType << std::endl;
            if(edgeType == 1) {
              //std::cout << "write out: " << vertex.first << " " << prevPoint << std::endl;
              output.insert(output.end(), Vertex45(prevPoint, 2, 1));
              output.insert(output.end(), Vertex45(vertex.first, 2, -1));
              //output.insert(output.end(), std::pair<Point, Point>(vertex.first, prevPoint));
            } else if(edgeType == -1){
              //std::cout << "write out: " << prevPoint << " " << vertex.first << std::endl;
              output.insert(output.end(), Vertex45(prevPoint, 2, -1));
              output.insert(output.end(), Vertex45(vertex.first, 2, 1));
              //output.insert(output.end(), std::pair<Point, Point>(prevPoint, vertex.first));
            }
          }
          currentCount = countBelow - verticalCount;
          while(lowIter != scanData_.end() &&
                lowIter->evalAtX(x_) == vertex.first.y()) {
            for(unsigned int i = lowIter->rise + 1; i < 3; ++i) {
              countAt[i] = lowIter->count;
            }
            Point lp(lowIter->x, lowIter->y);
            if(lp != vertex.first) {
              int edgeType = applyLogic(currentCount, lowIter->count);
              //std::cout << "edge logic: " << edgeType << std::endl;
              if(edgeType == 1) {
                //std::cout << "write out: " << lp << " " << vertex.first << std::endl;
                output.insert(output.end(), Vertex45(vertex.first, lowIter->rise, -1));
                //output.insert(output.end(), std::pair<Point, Point>(lp, vertex.first));
              } else if(edgeType == -1) {
                //std::cout << "write out: " << vertex.first << " " << lp << std::endl;
                output.insert(output.end(), Vertex45(vertex.first, lowIter->rise, 1));
                //output.insert(output.end(), std::pair<Point, Point>(vertex.first, lp));
              }
            }
            currentCount = lowIter->count;
            iterator nextIter = lowIter;
            ++nextIter;
            //std::cout << "erase\n";
            scanData_.erase(lowIter);
            lowIter = nextIter;
          }
          //assert_invariant();
          prevNonIntegerIntersection = currentNonIntegerIntersection;
          verticalCount += vertex.second[3];
          prevPoint = vertex.first;
          //std::cout << "new vertical count: " << verticalCount[0] << " " << verticalCount[1] << std::endl;
          prevIter = lowIter;
          //count represents the current state at this point
          //std::cout << vertex.second << std::endl;
          //std::cout << countAt << std::endl;
          //std::cout << "ADD\n";
          vertex.second += countAt;
          //std::cout << vertex.second << std::endl;
         
          //add elements to the scanline
          for(int i = 0; i < 3; ++i) {
            if(vertex.second[i] != countBelow) {
              //std::cout << "insert: " << vertex.first.x() << " " << vertex.first.y() << " " << i-1 <<
              //  " " << vertex.second[i][0] << " " << vertex.second[i][1] << std::endl;
              iterator insertIter = scanData_.insert(scanData_.end(), 
                                                     Scan45Element(vertex.first.x(), 
                                                                   vertex.first.y(), 
                                                                   i - 1, vertex.second[i]));
              findCross_(insertIter);
              int edgeType = applyLogic(countBelow, vertex.second[i]);
              if(edgeType) 
                output.insert(output.end(), Vertex45(vertex.first, i - 1, edgeType));
              //assert_invariant();
            }
            countBelow = vertex.second[i];
          }
        }
        //std::cout << "end processEvent\n";
        return inputBegin;
      }
   
      //iter1 is horizontal
      inline void scheduleCross0_(iterator iter1, iterator iter2) {
        //std::cout << "0, ";
        Unit y1 = iter1->evalAtX(x_);
        Unit y2 = iter2->evalAtX(x_);
        LongUnit delta = (LongUnit)abs((LongUnit)y1 - (LongUnit)y2);
        if(delta + x_ <= (std::numeric_limits<Unit>::max)())
          crossQueue_.insert(crossQueue_.end(), Point(x_ + delta, y1));
        //std::cout <<  Point(x_ + delta, y1);
      }

      //neither iter is horizontal
      inline bool scheduleCross1_(iterator iter1, iterator iter2) {
        //std::cout << "1, ";
        Unit y1 = iter1->evalAtX(x_);
        Unit y2 = iter2->evalAtX(x_);
        //std::cout << y1 << " " << y2 << ": ";
        //note that half the delta cannot exceed the positive inter range
        LongUnit delta = y1;
        delta -= y2;
        Unit UnitMax = (std::numeric_limits<Unit>::max)();
        if(delta & 1) {
          //delta is odd, division by 2 will result in integer trunctaion
          if(delta == 1) {
            //the cross point is not on the integer grid and cannot be represented
            // //we must throw an exception
            //std::string msg = "GTL 45 Boolean error, precision insufficient to represent edge intersection coordinate value.";
            //throw(msg);
            return true;
          } else {
            //note that result of this subtraction is always positive because itr1 is above itr2 in scanline
            LongUnit halfDelta2 = (LongUnit)((((LongUnit)y1) - y2)/2); 
            //note that halfDelta2 has been truncated
            if(halfDelta2 + x_ <= UnitMax && halfDelta2 + y2 <= UnitMax) {
              crossQueue_.insert(crossQueue_.end(), Point(x_+halfDelta2, y2+halfDelta2));
              crossQueue_.insert(crossQueue_.end(), Point(x_+halfDelta2, y2+halfDelta2+1));
            }
          }
        } else {
          LongUnit halfDelta = (LongUnit)((((LongUnit)y1) - y2)/2); 
          if(halfDelta + x_ <= UnitMax && halfDelta + y2 <= UnitMax)
            crossQueue_.insert(crossQueue_.end(), Point(x_+halfDelta, y2+halfDelta));
          //std::cout << Point(x_+halfDelta, y2+halfDelta);
        }
        return false;
      }
   
      inline bool findCross_(iterator iter) {
        //std::cout << "find cross ";
        iterator iteratorBelow = iter;
        iterator iteratorAbove = iter;
        if(iter != scanData_.begin() && iter->rise < 1) {
          --iteratorBelow;
          if(iter->rise == 0){
            if(iteratorBelow->rise == 1) {
              scheduleCross0_(iter, iteratorBelow);
            } 
          } else {
            //iter->rise == -1
            if(iteratorBelow->rise == 1) {
              return scheduleCross1_(iter, iteratorBelow);
            } else if(iteratorBelow->rise == 0) {
              scheduleCross0_(iteratorBelow, iter);
            }
          }
        }
        ++iteratorAbove;
        if(iteratorAbove != scanData_.end() && iter->rise > -1) {
          if(iter->rise == 0) {
            if(iteratorAbove->rise == -1) {
              scheduleCross0_(iter, iteratorAbove);
            }
          } else {
            //iter->rise == 1
            if(iteratorAbove->rise == -1) {
              return scheduleCross1_(iteratorAbove, iter);
            } else if(iteratorAbove->rise == 0) {
              scheduleCross0_(iteratorAbove, iter);
            }
          }
        } 
        //std::cout << std::endl; 
        return false;
      } 
   
      inline iterator lookUp_(Unit y){
        //if just before then we need to look from 1 not -1
        return scanData_.lower_bound(Scan45Element(x_, y, -1+2*justBefore_));
      }
   
      int applyLogic(Count2 count1, Count2 count2){
        bool l1 = applyLogic(count1);
        bool l2 = applyLogic(count2);
        if(l1 && !l2)
          return -1; //was true before and became false like a trailing edge
        if(!l1 && l2)
          return 1; //was false before and became true like a leading edge
        return 0; //no change in logic between the two counts
      }
      bool applyLogic(Count2 count) {
        if(op_ == 0) { //apply or
          return count[0] > 0 || count[1] > 0;
        }
        if(op_ == 1) { //apply and
          return count[0] > 0 && count[1] > 0;
        }
        if(op_ == 2) { //apply not
          return count[0] > 0 && !(count[1] > 0);
        }
        if(op_ == 3) { //apply xor
          return (count[0] > 0) ^ (count[1] > 0);
        }
        return false;
      }
    };


    static inline void print45Data(const std::set<Scan45Element, lessScan45Element>& data) {
      typename std::set<Scan45Element, lessScan45Element>::const_iterator iter;
      for(iter = data.begin(); iter != data.end(); ++iter) {
        std::cout << iter->x << " " << iter->y << " " << iter->rise << std::endl;
      }
    }

    static inline bool testScan45Data() {
      Unit x = 0;
      int justBefore = false;
      lessScan45Element lessElm(&x, &justBefore);
      std::set<Scan45Element, lessScan45Element> testData(lessElm);
      //Unit size = testData.size();
      typedef std::set<Scan45Element, lessScan45Element> Scan45Data;
      typename Scan45Data::iterator itr10 = testData.insert(testData.end(), Scan45Element(0, 10, 1));
      typename Scan45Data::iterator itr20 = testData.insert(testData.end(), Scan45Element(0, 20, 1));
      typename Scan45Data::iterator itr30 = testData.insert(testData.end(), Scan45Element(0, 30, -1));
      typename Scan45Data::iterator itr40 = testData.insert(testData.end(), Scan45Element(0, 40, -1));
      typename Scan45Data::iterator itrA = testData.lower_bound(Scan45Element(0, 29, -1));
      typename Scan45Data::iterator itr1 = testData.lower_bound(Scan45Element(0, 10, -1));
      x = 4;
      //now at 14 24 26 36
      typename Scan45Data::iterator itrB = testData.lower_bound(Scan45Element(4, 29, -1));
      typename Scan45Data::iterator itr2 = testData.lower_bound(Scan45Element(4, 14, -1));
      if(itr1 != itr2) std::cout << "test1 failed\n";
      if(itrA == itrB) std::cout << "test2 failed\n";
      //remove crossing elements
      testData.erase(itr20);
      testData.erase(itr30);
      x = 5;
      itr20 = testData.insert(testData.end(), Scan45Element(0, 20, 1));
      itr30 = testData.insert(testData.end(), Scan45Element(0, 30, -1));
      //now at 15 25 25 35
      typename Scan45Data::iterator itr = testData.begin();
      if(itr != itr10) std::cout << "test3 failed\n";
      ++itr;
      if(itr != itr30) std::cout << "test4 failed\n";
      ++itr;
      if(itr != itr20) std::cout << "test5 failed\n";
      ++itr;
      if(itr != itr40) std::cout << "test6 failed\n";
      std::cout << "done testing Scan45Data\n";
      return true;
    }
   
    static inline bool testScan45Rect() {
      std::cout << "testing Scan45Rect\n";
      Scan45 scan45(0);
      std::vector<Vertex45 > result;
      std::vector<Scan45Vertex> vertices;
      //is a Rectnagle(0, 0, 10, 10);
      Count2 count(1, 0);
      Count2 ncount(-1, 0);
      vertices.push_back(Scan45Vertex(Point(0,0), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      vertices.push_back(Scan45Vertex(Point(0,10), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(10,0), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(10,10), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      std::cout << "scanning\n";
      scan45.scan(result, vertices.begin(), vertices.end());
      std::cout << "done scanning\n";
      // result size == 8
      // result == 0 0 0 1
      // result == 0 0 2 1
      // result == 0 10 2 -1
      // result == 0 10 0 -1
      // result == 10 0 0 -1
      // result == 10 0 2 -1
      // result == 10 10 2 1
      // result == 10 10 0 1
      std::vector<Vertex45> reference;
      reference.push_back(Vertex45(Point(0, 0), 0, 1));
      reference.push_back(Vertex45(Point(0, 0), 2, 1));
      reference.push_back(Vertex45(Point(0, 10), 2, -1));
      reference.push_back(Vertex45(Point(0, 10), 0, -1));
      reference.push_back(Vertex45(Point(10, 0), 0, -1));
      reference.push_back(Vertex45(Point(10, 0), 2, -1));
      reference.push_back(Vertex45(Point(10, 10), 2, 1));
      reference.push_back(Vertex45(Point(10, 10), 0, 1));
      if(result != reference) {
        std::cout << "result size == " << result.size() << std::endl;
        for(unsigned int i = 0; i < result.size(); ++i) {
          //std::cout << "result == " << result[i]<< std::endl;
        }
        std::cout << "reference size == " << reference.size() << std::endl;
        for(unsigned int i = 0; i < reference.size(); ++i) {
          //std::cout << "reference == " << reference[i]<< std::endl;
        }
        return false;
      }
      std::cout << "done testing Scan45Rect\n";
      return true;
    }

    static inline bool testScan45P1() {
      std::cout << "testing Scan45P1\n";
      Scan45 scan45(0);
      std::vector<Vertex45 > result;
      std::vector<Scan45Vertex> vertices;
      //is a Rectnagle(0, 0, 10, 10);
      Count2 count(1, 0);
      Count2 ncount(-1, 0);
      vertices.push_back(Scan45Vertex(Point(0,0), Scan45Count(Count2(0, 0), Count2(0, 0), count, count)));
      vertices.push_back(Scan45Vertex(Point(0,10), Scan45Count(Count2(0, 0), Count2(0, 0), ncount, ncount)));
      vertices.push_back(Scan45Vertex(Point(10,10), Scan45Count(Count2(0, 0), Count2(0, 0), ncount, ncount)));
      vertices.push_back(Scan45Vertex(Point(10,20), Scan45Count(Count2(0, 0), Count2(0, 0), count, count)));
      std::cout << "scanning\n";
      scan45.scan(result, vertices.begin(), vertices.end());
      std::cout << "done scanning\n";
      // result size == 8
      // result == 0 0 1 1
      // result == 0 0 2 1
      // result == 0 10 2 -1
      // result == 0 10 1 -1
      // result == 10 10 1 -1
      // result == 10 10 2 -1
      // result == 10 20 2 1
      // result == 10 20 1 1
      std::vector<Vertex45> reference;
      reference.push_back(Vertex45(Point(0, 0), 1, 1));
      reference.push_back(Vertex45(Point(0, 0), 2, 1));
      reference.push_back(Vertex45(Point(0, 10), 2, -1));
      reference.push_back(Vertex45(Point(0, 10), 1, -1));
      reference.push_back(Vertex45(Point(10, 10), 1, -1));
      reference.push_back(Vertex45(Point(10, 10), 2, -1));
      reference.push_back(Vertex45(Point(10, 20), 2, 1));
      reference.push_back(Vertex45(Point(10, 20), 1, 1));
      if(result != reference) {
        std::cout << "result size == " << result.size() << std::endl;
        for(unsigned int i = 0; i < result.size(); ++i) {
          //std::cout << "result == " << result[i]<< std::endl;
        }
        std::cout << "reference size == " << reference.size() << std::endl;
        for(unsigned int i = 0; i < reference.size(); ++i) {
          //std::cout << "reference == " << reference[i]<< std::endl;
        }
        return false;
      }
      std::cout << "done testing Scan45P1\n";
      return true;
    }

    static inline bool testScan45P2() {
      std::cout << "testing Scan45P2\n";
      Scan45 scan45(0);
      std::vector<Vertex45 > result;
      std::vector<Scan45Vertex> vertices;
      //is a Rectnagle(0, 0, 10, 10);
      Count2 count(1, 0);
      Count2 ncount(-1, 0);
      vertices.push_back(Scan45Vertex(Point(0,0), Scan45Count(Count2(0, 0), count, ncount, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(10,0), Scan45Count(Count2(0, 0), ncount, count, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(10,10), Scan45Count(Count2(0, 0), ncount, count, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(20,10), Scan45Count(Count2(0, 0), count, ncount, Count2(0, 0))));
      std::cout << "scanning\n";
      scan45.scan(result, vertices.begin(), vertices.end());
      std::cout << "done scanning\n";
      // result size == 8
      // result == 0 0 0 1
      // result == 0 0 1 -1
      // result == 10 0 0 -1
      // result == 10 0 1 1
      // result == 10 10 1 1
      // result == 10 10 0 -1
      // result == 20 10 1 -1
      // result == 20 10 0 1
      std::vector<Vertex45> reference;
      reference.push_back(Vertex45(Point(0, 0), 0, 1));
      reference.push_back(Vertex45(Point(0, 0), 1, -1));
      reference.push_back(Vertex45(Point(10, 0), 0, -1));
      reference.push_back(Vertex45(Point(10, 0), 1, 1));
      reference.push_back(Vertex45(Point(10, 10), 1, 1));
      reference.push_back(Vertex45(Point(10, 10), 0, -1));
      reference.push_back(Vertex45(Point(20, 10), 1, -1));
      reference.push_back(Vertex45(Point(20, 10), 0, 1));
      if(result != reference) {
        std::cout << "result size == " << result.size() << std::endl;
        for(unsigned int i = 0; i < result.size(); ++i) {
          //std::cout << "result == " << result[i]<< std::endl;
        }
        std::cout << "reference size == " << reference.size() << std::endl;
        for(unsigned int i = 0; i < reference.size(); ++i) {
          //std::cout << "reference == " << reference[i]<< std::endl;
        }
        return false;
      }
      std::cout << "done testing Scan45P2\n";
      return true;
    }

    static inline bool testScan45And() {
      std::cout << "testing Scan45And\n";
      Scan45 scan45(1);
      std::vector<Vertex45 > result;
      std::vector<Scan45Vertex> vertices;
      //is a Rectnagle(0, 0, 10, 10);
      Count2 count(1, 0);
      Count2 ncount(-1, 0);
      vertices.push_back(Scan45Vertex(Point(0,0), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      vertices.push_back(Scan45Vertex(Point(0,10), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(10,0), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(10,10), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      count = Count2(0, 1);
      ncount = count.invert();
      vertices.push_back(Scan45Vertex(Point(2,2), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      vertices.push_back(Scan45Vertex(Point(2,12), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(12,2), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(12,12), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      sortScan45Vector(vertices);
      std::cout << "scanning\n";
      scan45.scan(result, vertices.begin(), vertices.end());
      std::cout << "done scanning\n";
      //result size == 8
      //result == 2 2 0 1
      //result == 2 2 2 1
      //result == 2 10 2 -1
      //result == 2 10 0 -1
      //result == 10 2 0 -1
      //result == 10 2 2 -1
      //result == 10 10 2 1
      //result == 10 10 0 1
      std::vector<Vertex45> reference;
      reference.push_back(Vertex45(Point(2, 2), 0, 1));
      reference.push_back(Vertex45(Point(2, 2), 2, 1));
      reference.push_back(Vertex45(Point(2, 10), 2, -1));
      reference.push_back(Vertex45(Point(2, 10), 0, -1));
      reference.push_back(Vertex45(Point(10, 2), 0, -1));
      reference.push_back(Vertex45(Point(10, 2), 2, -1));
      reference.push_back(Vertex45(Point(10, 10), 2, 1));
      reference.push_back(Vertex45(Point(10, 10), 0, 1));
      if(result != reference) {
        std::cout << "result size == " << result.size() << std::endl;
        for(unsigned int i = 0; i < result.size(); ++i) {
          //std::cout << "result == " << result[i]<< std::endl;
        }
        std::cout << "reference size == " << reference.size() << std::endl;
        for(unsigned int i = 0; i < reference.size(); ++i) {
          //std::cout << "reference == " << reference[i]<< std::endl;
        }
        return false;
      }
      std::cout << "done testing Scan45And\n";
      return true;
    }

    static inline bool testScan45Star1() {
      std::cout << "testing Scan45Star1\n";
      Scan45 scan45(0);
      std::vector<Vertex45 > result;
      std::vector<Scan45Vertex> vertices;
      //is a Rectnagle(0, 0, 10, 10);
      Count2 count(1, 0);
      Count2 ncount(-1, 0);
      vertices.push_back(Scan45Vertex(Point(0,8), Scan45Count(count, Count2(0, 0), ncount, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(8,0), Scan45Count(ncount, Count2(0, 0), Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(8,16), Scan45Count(Count2(0, 0), Count2(0, 0), count, count)));
      count = Count2(0, 1);
      ncount = count.invert();
      vertices.push_back(Scan45Vertex(Point(12,8), Scan45Count(count, Count2(0, 0), ncount, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(4,0), Scan45Count(Count2(0, 0), Count2(0, 0), count, count)));
      vertices.push_back(Scan45Vertex(Point(4,16), Scan45Count(ncount, Count2(0, 0), Count2(0, 0), ncount)));
      sortScan45Vector(vertices);
      std::cout << "scanning\n";
      scan45.scan(result, vertices.begin(), vertices.end());
      std::cout << "done scanning\n";
      // result size == 24
      // result == 0 8 -1 1
      // result == 0 8 1 -1
      // result == 4 0 1 1
      // result == 4 0 2 1
      // result == 4 4 2 -1
      // result == 4 4 -1 -1
      // result == 4 12 1 1
      // result == 4 12 2 1
      // result == 4 16 2 -1
      // result == 4 16 -1 -1
      // result == 6 2 1 -1
      // result == 6 14 -1 1
      // result == 6 2 -1 1
      // result == 6 14 1 -1
      // result == 8 0 -1 -1
      // result == 8 0 2 -1
      // result == 8 4 2 1
      // result == 8 4 1 1
      // result == 8 12 -1 -1
      // result == 8 12 2 -1
      // result == 8 16 2 1
      // result == 8 16 1 1
      // result == 12 8 1 -1
      // result == 12 8 -1 1
      if(result.size() != 24) {
        //std::cout << "result size == " << result.size() << std::endl;
        //std::cout << "reference size == " << 24 << std::endl;
        return false;
      }
      std::cout << "done testing Scan45Star1\n";
      return true;
    }

    static inline bool testScan45Star2() {
      std::cout << "testing Scan45Star2\n";
      Scan45 scan45(0);
      std::vector<Vertex45 > result;
      std::vector<Scan45Vertex> vertices;
      //is a Rectnagle(0, 0, 10, 10);
      Count2 count(1, 0);
      Count2 ncount(-1, 0);
      vertices.push_back(Scan45Vertex(Point(0,4), Scan45Count(Count2(0, 0), count, ncount, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(16,4), Scan45Count(count, ncount, Count2(0, 0), Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(8,12), Scan45Count(ncount, Count2(0, 0), count, Count2(0, 0))));
      count = Count2(0, 1);
      ncount = count.invert();
      vertices.push_back(Scan45Vertex(Point(0,8), Scan45Count(count, ncount, Count2(0, 0), Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(16,8), Scan45Count(Count2(0, 0), count, ncount, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(8,0), Scan45Count(ncount, Count2(0, 0), count, Count2(0, 0))));
      sortScan45Vector(vertices);
      std::cout << "scanning\n";
      scan45.scan(result, vertices.begin(), vertices.end());
      std::cout << "done scanning\n";
      // result size == 24
      // result == 0 4 0 1
      // result == 0 4 1 -1
      // result == 0 8 -1 1
      // result == 0 8 0 -1
      // result == 2 6 1 1
      // result == 2 6 -1 -1
      // result == 4 4 0 -1
      // result == 4 8 0 1
      // result == 4 4 -1 1
      // result == 4 8 1 -1
      // result == 8 0 -1 -1
      // result == 8 0 1 1
      // result == 8 12 1 1
      // result == 8 12 -1 -1
      // result == 12 4 1 -1
      // result == 12 8 -1 1
      // result == 12 4 0 1
      // result == 12 8 0 -1
      // result == 14 6 -1 -1
      // result == 14 6 1 1
      // result == 16 4 0 -1
      // result == 16 4 -1 1
      // result == 16 8 1 -1
      // result == 16 8 0 1
      if(result.size() != 24) {
        //std::cout << "result size == " << result.size() << std::endl;
        //std::cout << "reference size == " << 24 << std::endl;
        return false;
      }
      std::cout << "done testing Scan45Star2\n";
      return true;
    }

    static inline bool testScan45Star3() {
      std::cout << "testing Scan45Star3\n";
      Scan45 scan45(0);
      std::vector<Vertex45 > result;
      std::vector<Scan45Vertex> vertices;
      //is a Rectnagle(0, 0, 10, 10);
      Count2 count(1, 0);
      Count2 ncount(-1, 0);
      vertices.push_back(Scan45Vertex(Point(0,8), Scan45Count(count, Count2(0, 0), ncount, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(8,0), Scan45Count(ncount, Count2(0, 0), Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(8,16), Scan45Count(Count2(0, 0), Count2(0, 0), count, count)));

      vertices.push_back(Scan45Vertex(Point(6,0), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      vertices.push_back(Scan45Vertex(Point(6,14), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(12,0), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(12,14), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      count = Count2(0, 1);
      ncount = count.invert();
      vertices.push_back(Scan45Vertex(Point(12,8), Scan45Count(count, Count2(0, 0), ncount, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(4,0), Scan45Count(Count2(0, 0), Count2(0, 0), count, count)));
      vertices.push_back(Scan45Vertex(Point(4,16), Scan45Count(ncount, Count2(0, 0), Count2(0, 0), ncount)));
      sortScan45Vector(vertices);
      std::cout << "scanning\n";
      scan45.scan(result, vertices.begin(), vertices.end());
      std::cout << "done scanning\n";
      // result size == 28
      // result == 0 8 -1 1
      // result == 0 8 1 -1
      // result == 4 0 1 1
      // result == 4 0 2 1
      // result == 4 4 2 -1
      // result == 4 4 -1 -1
      // result == 4 12 1 1
      // result == 4 12 2 1
      // result == 4 16 2 -1
      // result == 4 16 -1 -1
      // result == 6 2 1 -1
      // result == 6 14 -1 1
      // result == 6 0 0 1
      // result == 6 0 2 1
      // result == 6 2 2 -1
      // result == 6 14 1 -1
      // result == 8 0 0 -1
      // result == 8 0 0 1
      // result == 8 14 0 -1
      // result == 8 14 2 -1
      // result == 8 16 2 1
      // result == 8 16 1 1
      // result == 12 0 0 -1
      // result == 12 0 2 -1
      // result == 12 8 2 1
      // result == 12 8 2 -1
      // result == 12 14 2 1
      // result == 12 14 0 1
      if(result.size() != 28) {
        //std::cout << "result size == " << result.size() << std::endl;
        //std::cout << "reference size == " << 28 << std::endl;
        return false;
      }

      std::cout << "done testing Scan45Star3\n";
      return true;
    }

    static inline bool testScan45Star4() {
      std::cout << "testing Scan45Star4\n";
      Scan45 scan45(0);
      std::vector<Vertex45 > result;
      std::vector<Scan45Vertex> vertices;
      //is a Rectnagle(0, 0, 10, 10);
      Count2 count(1, 0);
      Count2 ncount(-1, 0);
      vertices.push_back(Scan45Vertex(Point(0,4), Scan45Count(Count2(0, 0), count, ncount, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(16,4), Scan45Count(count, ncount, Count2(0, 0), Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(8,12), Scan45Count(ncount, Count2(0, 0), count, Count2(0, 0))));

      vertices.push_back(Scan45Vertex(Point(0,6), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      vertices.push_back(Scan45Vertex(Point(0,12), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(16,6), Scan45Count(Count2(0, 0), ncount, Count2(0, 0), ncount)));
      vertices.push_back(Scan45Vertex(Point(16,12), Scan45Count(Count2(0, 0), count, Count2(0, 0), count)));
      count = Count2(0, 1);
      ncount = count.invert();
      vertices.push_back(Scan45Vertex(Point(0,8), Scan45Count(count, ncount, Count2(0, 0), Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(16,8), Scan45Count(Count2(0, 0), count, ncount, Count2(0, 0))));
      vertices.push_back(Scan45Vertex(Point(8,0), Scan45Count(ncount, Count2(0, 0), count, Count2(0, 0))));
      sortScan45Vector(vertices);
      std::cout << "scanning\n";
      scan45.scan(result, vertices.begin(), vertices.end());
      std::cout << "done scanning\n";
      // result size == 28
      // result == 0 4 0 1
      // result == 0 4 1 -1
      // result == 0 6 0 1
      // result == 0 6 2 1
      // result == 0 8 2 -1
      // result == 0 8 2 1
      // result == 0 12 2 -1
      // result == 0 12 0 -1
      // result == 2 6 1 1
      // result == 2 6 0 -1
      // result == 4 4 0 -1
      // result == 4 4 -1 1
      // result == 8 12 0 1
      // result == 8 0 -1 -1
      // result == 8 0 1 1
      // result == 8 12 0 -1
      // result == 12 4 1 -1
      // result == 12 4 0 1
      // result == 14 6 -1 -1
      // result == 14 6 0 1
      // result == 16 4 0 -1
      // result == 16 4 -1 1
      // result == 16 6 0 -1
      // result == 16 6 2 -1
      // result == 16 8 2 1
      // result == 16 8 2 -1
      // result == 16 12 2 1
      // result == 16 12 0 1
      if(result.size() != 28) {
        //std::cout << "result size == " << result.size() << std::endl;
        //std::cout << "reference size == " << 28 << std::endl;
        return false;
      }

      std::cout << "done testing Scan45Star4\n";
      return true;
    }

    static inline bool testScan45() {
      if(!testScan45Rect()) return false;
      if(!testScan45P1()) return false;
      if(!testScan45P2()) return false;
      if(!testScan45And()) return false;
      if(!testScan45Star1()) return false;
      if(!testScan45Star2()) return false;
      if(!testScan45Star3()) return false;
      if(!testScan45Star4()) return false;
      return true;
    }

  };

}
#endif
