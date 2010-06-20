// Boost sweepline/voronoi_output.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_OUTPUT
#define BOOST_SWEEPLINE_VORONOI_OUTPUT

#include <list>
#include <vector>

namespace boost {
namespace sweepline {

    template <typename Point2D>
    struct half_edge;

    // Cell record data structure. Represents voronoi cell.
    // Contains site point and pointer to any incident half-edge.
    template <typename Point2D>
    struct cell_record {
        half_edge<Point2D> *incident_edge;
        Point2D site_point;

        cell_record(Point2D site, half_edge<Point2D>* edge) : incident_edge(edge), site_point(site) {}
    };

    // Voronoi vertex data structure. Represents voronoi vertex.
    // Contains vertex coordinates and pointers to all incident half-edges.
    template <typename Point2D>
    struct vertex_record {
        std::list< half_edge<Point2D>* > incident_edges;
        Point2D vertex;

        vertex_record(Point2D vertex) : vertex(vertex) {}
    };

    // Half-edge data structure. Represents voronoi edge.
    // Contains: 1) pointer to cell records;
    //           2) pointers to start/end vertices of half-edge;
    //           3) pointers to previous/next half-edges(CCW);
    //           4) pointer to twin half-edge.
    template <typename Point2D>
    struct half_edge {
        typedef typename cell_record<Point2D> cell_record_type;
        typedef typename vertex_record<Point2D> vertex_record_type;
        typedef typename half_edge<Point2D> half_edge_type;

        cell_record_type *cell_record;
        vertex_record_type *start_point;
        vertex_record_type *end_point;
        half_edge_type *prev;
        half_edge_type *next;
        half_edge_type *twin;

        half_edge(int site_index) :
            cell_record(NULL),
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
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;
        typedef typename cell_record<Point2D> cell_record_type;
        typedef typename vertex_record<Point2D> vertex_record_type;
        typedef typename half_edge<Point2D> edge_type;
        typedef typename std::vector<cell_record_type> cell_records;
        typedef typename std::list<vertex_record_type> voronoi_vertices;
        typedef typename std::list<edge_type *>::const_iterator edge_iterator;
        typedef typename voronoi_vertices::const_iterator voronoi_vertices_iterator;

        voronoi_output() {}

        // This preserves the validity of iterators.
        void init(int num_sites) {
            cell_records_.reserve(num_sites);
        }

        void reset() {
            cell_records_.clear();
            vertex_records_.clear();
            edges_.clear();
        }

        // Inserts new half-edge into the output data structure during site
        // event processing. Takes as input left and right sites of the new
        // beach line node and returns reference to the new half-edge. 
        // Second argument is new site. During this step we add two new
        // twin half-edges.
        edge_type *insert_new_edge(const site_event_type &site1,
                                   const site_event_type &site2) {
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
            if (cell_records_.empty())
                cell_records_.push_back(cell_record_type(site1.get_point(), &edge1));

            // Second site represents new site during site event processing.
            // Add new cell to the cell records vector.
            cell_records_.push_back(cell_record_type(site2.get_point(), &edge2));

            // Update pointers to cells.
            edge1.cell_record = &cell_records_[site_index1];
            edge2.cell_record = &cell_records_[site_index2];

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
            // Add new voronoi vertex as voronoi circle center.
            vertex_records_.push_back(vertex_record_type(circle.get_center()));
            vertex_record_type &new_vertex = vertex_records_.back();

            // Update two input bisectors and their twins half-edges with
            // new voronoi vertex.
            edge12->start_point = &new_vertex;
            edge12->twin->end_point = &new_vertex;
            edge23->start_point = &new_vertex;
            edge23->twin->end_point = &new_vertex;

            // Add new half-edge.
            edges_.push_back(edge_type(site1.get_site_index()));
            edge_type &new_edge1 = edges_.back();
            new_edge1.cell_record = &cell_records_[site1.get_site_index()];
            new_edge1.end_point = &new_vertex;

            // Add new half-edge.
            edges_.push_back(edge_type(site3.get_site_index()));
            edge_type &new_edge2 = edges_.back();
            new_edge2.cell_record = &cell_records_[site3.get_site_index()];
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
            new_vertex.incident_edges.push_back(edge23);
            new_vertex.incident_edges.push_back(&new_edge2);
            return &new_edge1;
        }

        const cell_records &get_cell_records() const {
            return cell_records_;
        }

        const voronoi_vertices &get_voronoi_vertices() const {
            return vertex_records_;
        }

    private:
        std::vector<cell_record_type> cell_records_;
        std::list<vertex_record_type> vertex_records_;
        std::list<edge_type> edges_;

        //Disallow copy constructor and operator=
        voronoi_output(const voronoi_output&);
        void operator=(const voronoi_output&);
    };

} // sweepline
} // boost

#endif