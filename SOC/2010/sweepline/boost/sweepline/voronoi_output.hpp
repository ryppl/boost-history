// Boost sweepline/voronoi_output.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// Point in 2D space data structure. Comparators defined in this
    // data structure actually define sweepline movement direction.

#ifndef BOOST_SWEEPLINE_VORONOI_OUTPUT
#define BOOST_SWEEPLINE_VORONOI_OUTPUT

#include "detail/voronoi_formation.hpp"

namespace boost {
namespace sweepline {

    template <typename Point2D>
    struct half_edge;

    template <typename T>
    struct point_2d {
    public:
        typedef T coordinate_type;
        typedef half_edge< point_2d<T> > Edge;

        point_2d() {}

        point_2d(T x, T y) {
            x_ = x;
            y_ = y;
        }

        bool operator==(const point_2d &point) const {
            return (this->x_ == point.x()) && (this->y_ == point.y());
        }

        bool operator!=(const point_2d &point) const {
            return (this->x_ != point.x()) || (this->y_ != point.y());
        }

        // This comparator actually defines sweepline movement direction.
        bool operator<(const point_2d &point) const {
            // Sweepline sweeps from left to right.
            if (this->x_ != point.x_)
                return this->x_ < point.x_;
            return this->y_ < point.y_;
        }

        bool operator<=(const point_2d &point) const {
            return !(point < (*this));
        }

        bool operator>(const point_2d &point) const {
            return point < (*this);
        }

        bool operator>=(const point_2d &point) const {
            return !((*this) < point);
        }

        coordinate_type x() const {
            return this->x_;
        }

        coordinate_type y() const {
            return this->y_;
        }

        void x(coordinate_type x) {
            x_ = x;
        }

        void y(coordinate_type y) {
            y_ = y;
        }

    private:
        coordinate_type x_;
        coordinate_type y_;
    };

    template <typename T>
    point_2d<T> make_point_2d(T x, T y) {
        return point_2d<T>(x,y);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // VORONOI OUTPUT TYPES ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Cell record data structure. Represents voronoi cell.
    // Contains site point pointer to any incident half-edge and number
    // of incident half-edges.
    template <typename Point2D>
    struct cell_record {
        half_edge<Point2D> *incident_edge;
        Point2D site_point;
        int num_incident_edges;

        cell_record(Point2D site, half_edge<Point2D>* edge) :
            incident_edge(edge),
            site_point(site),
            num_incident_edges(0) {}
    };

    // Voronoi vertex data structure. Represents voronoi vertex.
    // Contains vertex coordinates, pointers to all incident half-edges and
    // number of incident half-edges.
    template <typename Point2D>
    struct vertex_record {
        typedef typename std::list< half_edge<Point2D>* >::iterator vertex_incident_edges_it;

        std::list< half_edge<Point2D>* > incident_edges;
        Point2D vertex;
        int num_incident_edges;
        typename std::list< vertex_record<Point2D> >::iterator vertex_it;

        vertex_record(Point2D vertex) : vertex(vertex), num_incident_edges(3) {}

        vertex_incident_edges_it get_prev_incident_edge(vertex_incident_edges_it it) {
            if (it != incident_edges.begin()) {
                it--;
                return it;
            }
            it = incident_edges.end();
            it--;
            return it;
        }

        vertex_incident_edges_it get_next_incident_edge(vertex_incident_edges_it it) {
            it++;
            if (it != incident_edges.end())
                return it;            
            return incident_edges.begin();
        }
    };

    // Half-edge data structure. Represents voronoi edge.
    // Contains: 1) pointer to cell records;
    //           2) pointers to start/end vertices of half-edge;
    //           3) pointers to previous/next half-edges(CCW);
    //           4) pointer to twin half-edge;
    //           5) iterator in the vertex incident edges list.
    template <typename Point2D>
    struct half_edge {
        typedef cell_record<Point2D> cell_record_type;
        typedef vertex_record<Point2D> vertex_record_type;
        typedef half_edge<Point2D> half_edge_type;

        cell_record_type *cell;
        vertex_record_type *start_point;
        vertex_record_type *end_point;
        half_edge_type *prev;
        half_edge_type *next;
        half_edge_type *twin;
        typename std::list< half_edge<Point2D>* >::iterator incident_it;

        half_edge(int site_index) :
            cell(NULL),
            start_point(NULL),
            end_point(NULL),
            prev(NULL),
            next(NULL),
            twin(NULL) {}
    };

    // Voronoi output data structure based on the half-edges.
    // Contains vector of voronoi cells, doubly linked list of
    // voronoi vertices and voronoi edges.
    template <typename Point2D>
    class voronoi_output {
    public:
        typedef typename detail::site_event<Point2D> site_event_type;
        typedef typename detail::circle_event<Point2D> circle_event_type;
        typedef cell_record<Point2D> cell_record_type;
        typedef vertex_record<Point2D> vertex_record_type;
        typedef half_edge<Point2D> edge_type;
        typedef std::vector<cell_record_type> cell_records_type;
        typedef std::list<vertex_record_type> voronoi_vertices_type;
        typedef typename std::list<edge_type *>::iterator edge_pointer_iterator_type;
        typedef typename std::list<edge_type *>::const_iterator edge_pointer_const_iterator_type;
        typedef typename voronoi_vertices_type::iterator voronoi_vertices_iterator_type;
        typedef typename voronoi_vertices_type::const_iterator
            voronoi_vertices_const_iterator_type;

        voronoi_output() {
            num_cell_records_ = 0;
            num_edges_ = 0;
            num_vertex_records_ = 0;
        }

        // This preserves the validity of iterators.
        void init(int num_sites) {
            cell_records_.reserve(num_sites);
            num_cell_records_ = 0;
            num_edges_ = 0;
            num_vertex_records_ = 0;
        }

        void reset() {
            cell_records_.clear();
            vertex_records_.clear();
            edges_.clear();

            num_cell_records_ = 0;
            num_edges_ = 0;
            num_vertex_records_ = 0;
        }

        // Inserts new half-edge into the output data structure during site
        // event processing. Takes as input left and right sites of the new
        // beach line node and returns reference to the new half-edge. 
        // Second argument is new site. During this step we add two new
        // twin half-edges.
        edge_type *insert_new_edge(const site_event_type &site1,
                                   const site_event_type &site2) {
            num_cell_records_++;
            num_edges_++;

            // Get indexes of sites.                                           
            int site_index1 = site1.get_site_index();
            int site_index2 = site2.get_site_index();

            // Create new half-edge that belongs to the cell with the first site.
            edges_.push_back(edge_type(site_index1));
            edge_type &edge1 = edges_.back();

            // Create new half-edge that belongs to the cell with the second site.
            edges_.push_back(edge_type(site_index2));
            edge_type &edge2 = edges_.back();

            // Add initial cell during first edge insertion.
            if (cell_records_.empty()) {
                cell_records_.push_back(cell_record_type(site1.get_point(), &edge1));
                num_cell_records_++;
            }

            // Second site represents new site during site event processing.
            // Add new cell to the cell records vector.
            cell_records_.push_back(cell_record_type(site2.get_point(), &edge2));

            // Update pointers to cells.
            edge1.cell = &cell_records_[site_index1];
            edge2.cell = &cell_records_[site_index2];

            // Update incident edges counters.
            cell_records_[site_index1].num_incident_edges++;
            cell_records_[site_index2].num_incident_edges++;

            // Update twin pointers.
            edge1.twin = &edge2;
            edge2.twin = &edge1;

            return &edge1;
        }

        edge_type *insert_new_edge(const site_event_type &site1,
                                   const site_event_type &site2,
                                   const site_event_type &site3,
                                   const circle_event_type &circle,
                                   edge_type *edge12,
                                   edge_type *edge23) {
            num_vertex_records_++;
            num_edges_++;

            // Add new voronoi vertex as voronoi circle center.
            vertex_records_.push_back(vertex_record_type(circle.get_center()));
            vertex_record_type &new_vertex = vertex_records_.back();
            
            // Update vertex iterator.
            voronoi_vertices_iterator_type vertices_it = vertex_records_.end();
            vertices_it--;
            new_vertex.vertex_it = vertices_it;

            // Update two input bisectors and their twins half-edges with
            // new voronoi vertex.
            edge12->start_point = &new_vertex;
            edge12->twin->end_point = &new_vertex;
            edge23->start_point = &new_vertex;
            edge23->twin->end_point = &new_vertex;

            // Add new half-edge.
            edges_.push_back(edge_type(site1.get_site_index()));
            edge_type &new_edge1 = edges_.back();
            new_edge1.cell = &cell_records_[site1.get_site_index()];
            cell_records_[site1.get_site_index()].num_incident_edges++;
            new_edge1.end_point = &new_vertex;

            // Add new half-edge.
            edges_.push_back(edge_type(site3.get_site_index()));
            edge_type &new_edge2 = edges_.back();
            new_edge2.cell = &cell_records_[site3.get_site_index()];
            cell_records_[site3.get_site_index()].num_incident_edges++;
            new_edge2.start_point = &new_vertex;

            // Update twin pointers of the new half-edges.
            new_edge1.twin = &new_edge2;
            new_edge2.twin = &new_edge1;

            // Update voronoi prev/next pointers of all half-edges incident
            // to the new voronoi vertex.
            edge12->prev = &new_edge1;
            new_edge1.next = edge12;
            edge12->twin->next = edge23;
            edge23->prev = edge12->twin;
            edge23->twin->next = &new_edge2;
            new_edge2.prev = edge23->twin;

            // Update voronoi vertex incident edges pointers.
            new_vertex.incident_edges.push_back(edge12);
            typename std::list<edge_type*>::iterator temp_iter =new_vertex.incident_edges.begin();
            edge12->incident_it = temp_iter;
            
            new_vertex.incident_edges.push_back(edge23);
            temp_iter++;
            edge23->incident_it = temp_iter;
            
            new_vertex.incident_edges.push_back(&new_edge2);
            temp_iter++;
            new_edge2.incident_it = temp_iter;

            return &new_edge1;
        }

        const cell_records_type &get_cell_records() const {
            return cell_records_;
        }

        const voronoi_vertices_type &get_voronoi_vertices() const {
            return vertex_records_;
        }

        int get_num_voronoi_cells() const {
            return num_cell_records_;
        }

        int get_num_voronoi_vertices() const {
            return num_vertex_records_;
        }

        int get_num_voronoi_edges() const {
            return num_edges_;
        }

        void simplify() {
            typename std::list<edge_type>::iterator edge_it1;
            typename std::list<edge_type>::iterator edge_it = edges_.begin();

            // Iterate over all edges and remove degeneracies.
            while (edge_it != edges_.end()) {
                edge_it1 = edge_it;
                edge_it++;
                edge_it++;

                if (edge_it1->start_point && edge_it1->end_point &&
                    edge_it1->start_point->vertex == edge_it1->end_point->vertex) {
                    // Splice incident edges of the second voronoi vertex to the first
                    // one, if it contains less or equal number of them.

                    // Decrease number of cell incident edges.
                    edge_it1->cell->num_incident_edges--;
                    edge_it1->twin->cell->num_incident_edges--;

                    // To guarantee N*lon(N) time we merge vertex with
                    // less incident edges to the one with more.
                    if (edge_it1->start_point->num_incident_edges >=
                        edge_it1->end_point->num_incident_edges)
                            simplify_edge(*edge_it1);
                    else
                        simplify_edge(*edge_it1->twin);

                    // Remove zero length edges.
                    edges_.erase(edge_it1, edge_it);

                    num_vertex_records_--;
                    num_edges_--;
                }
            }
        }

        bool check() {
            return true;
        }

    private:
        void simplify_edge(edge_type &edge) {
            vertex_record_type *vertex1 = edge.start_point;
            vertex_record_type *vertex2 = edge.end_point;

            // Update number of vertex incident edges.
            vertex1->num_incident_edges += vertex2->num_incident_edges - 2;

            // Update second vertex incident edges start and end points.
            for (edge_pointer_iterator_type it = vertex2->incident_edges.begin();
                 it != vertex2->incident_edges.end(); it++) {
                (*it)->start_point = vertex1;
                (*it)->twin->end_point = vertex1;
            }

            // Update prev and next pointers of incident edges that
            // belongs to different voronoi vertices.
            edge_pointer_iterator_type edge1_it = edge.incident_it;
            edge_pointer_iterator_type edge2_it = edge.twin->incident_it;

            edge_pointer_iterator_type edge1_it_prev = vertex1->get_prev_incident_edge(edge1_it);
            edge_pointer_iterator_type edge1_it_next = vertex1->get_next_incident_edge(edge1_it);

            edge_pointer_iterator_type edge2_it_prev = vertex2->get_prev_incident_edge(edge2_it);
            edge_pointer_iterator_type edge2_it_next = vertex2->get_next_incident_edge(edge2_it);
            
            (*edge1_it_prev)->twin->next = *edge2_it_next;
            (*edge2_it_next)->prev = (*edge1_it_prev)->twin;

            (*edge1_it_next)->prev = (*edge2_it_prev)->twin;
            (*edge2_it_prev)->twin->next = (*edge1_it_next);

            // Splice incident edges of the second voronoi vertex to the first one.
            edge2_it++;
            for (; edge2_it != vertex2->incident_edges.end(); edge2_it++)
                (*edge2_it)->incident_it = vertex1->incident_edges.insert(edge1_it, *edge2_it);

            edge2_it = vertex2->incident_edges.begin();
            for (; edge2_it != edge.twin->incident_it; edge2_it++)
                (*edge2_it)->incident_it = vertex1->incident_edges.insert(edge1_it, *edge2_it);

            // Remove zero length edge from list of incident edges.
            vertex1->incident_edges.erase(edge1_it);

            // Remove second vertex from the vertex records.
            vertex_records_.erase(vertex2->vertex_it);
        }

        std::vector<cell_record_type> cell_records_;
        std::list<vertex_record_type> vertex_records_;
        std::list<edge_type> edges_;

        int num_cell_records_;
        int num_vertex_records_;
        int num_edges_;

        //Disallow copy constructor and operator=
        voronoi_output(const voronoi_output&);
        void operator=(const voronoi_output&);
    };

} // sweepline
} // boost

#endif
