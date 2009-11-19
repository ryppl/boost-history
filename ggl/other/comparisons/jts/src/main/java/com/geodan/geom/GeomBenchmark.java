package com.geodan.geom;

import java.io.File;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.geotools.data.DataStore;
import org.geotools.data.DataStoreFinder;
import org.geotools.data.FeatureSource;
import org.geotools.feature.FeatureCollection;
import org.geotools.feature.FeatureIterator;
import org.opengis.feature.simple.SimpleFeature;
import org.opengis.feature.simple.SimpleFeatureType;

import com.vividsolutions.jts.geom.Coordinate;
import com.vividsolutions.jts.geom.CoordinateList;
import com.vividsolutions.jts.geom.Envelope;
import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.geom.GeometryFactory;
import com.vividsolutions.jts.geom.LinearRing;
import com.vividsolutions.jts.geom.MultiPolygon;
import com.vividsolutions.jts.geom.Point;
import com.vividsolutions.jts.geom.Polygon;
import com.vividsolutions.jts.geom.prep.PreparedPolygon;
import com.vividsolutions.jts.simplify.DouglasPeuckerSimplifier;

public class GeomBenchmark {

	private List<Polygon> polygons;
	private List<Envelope> boxes;

	private List<Envelope> clip_boxes; // for clips
	private List<Polygon> ellipses; // for intersections
	private List<Integer> ids; // for tracing

	private List<PreparedPolygon> ppolygons;

	private GeometryFactory geometryFactory;

	// intersections/unions

	public GeomBenchmark() {
		super();
		polygons = new ArrayList<Polygon>();
		boxes = new ArrayList<Envelope>();
		clip_boxes = new ArrayList<Envelope>(); // for clips
		ellipses = new ArrayList<Polygon>(); // for

		geometryFactory = new GeometryFactory();

		ids = new ArrayList<Integer>();

	}

	public void prepare(File shapefile) {
		polygons.clear();
		boxes.clear();
		clip_boxes.clear();
		ellipses.clear();
		ids.clear();

		geometryFactory = new GeometryFactory();

		try {
			// load all shapes from shapefile into polygons
			read_shapefile(shapefile, polygons, ids);

		} catch (Exception e) {
			System.out.println(e.getMessage());
			System.exit(1);
		}

		// ////////////////////////////////////////////////////////////////
		ppolygons = new ArrayList<PreparedPolygon>(polygons.size());

		for (Polygon polygon : polygons) {
			PreparedPolygon prepoly = new PreparedPolygon(polygon);
			ppolygons.add(prepoly);
			if (prepoly.contains(polygon.getCentroid())) {
				// needed to cache it
			}
		}
		// ////////////////////////////////////////////////////////////////

		// Create envelopes
		for (Polygon polygon : polygons) {
			boxes.add(polygon.getEnvelopeInternal());
		}
		// Create the star-ellipses for intersections later on
		if (Compare.MEASURE_OVERLAY || Compare.MEASURE_CLIP) {
			int k = 0;
			for (Envelope box : boxes) {
				k++;

				double cx = box.centre().x;
				double cy = box.centre().y;

				double dx = box.getWidth();
				double dy = box.getHeight();

				if (Compare.MEASURE_OVERLAY) {
					double a1 = Compare.OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dx;
					double b1 = Compare.OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dy;
					double a2 = Compare.OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dx;
					double b2 = Compare.OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dy;

					// We will use a coordinate list to build the linearring
					CoordinateList clist = new CoordinateList();
					// Compare.OVERLAY_ELLIPSE_COUNT);
					double angle = 0.0; // 45.0 * ggl::math::d2r; //0.0;
					for (int i = 0; i < Compare.OVERLAY_ELLIPSE_COUNT - 1; i++, angle += Compare.delta) {
						if (i % 2 == 0) {
							clist.add(new Coordinate(cx + a1 * Math.sin(angle),
									cy + b1 * Math.cos(angle)));
						} else {
							clist.add(new Coordinate(cx + a2 * Math.sin(angle),
									cy + b2 * Math.cos(angle)));
						}
					}

					clist.add(clist.get(0));
					LinearRing lr = geometryFactory.createLinearRing(clist
							.toCoordinateArray());
					Polygon ellipse = geometryFactory.createPolygon(lr, null);
					ellipses.add(ellipse);
				}

				if (Compare.MEASURE_CLIP) {

					// note : weird use of sin/cos for a constant angle
					// effectively this create a box . shrinkBy(
					// Compare.CLIP_FACTOR * 0.5*sqrt2)?
					double a = Compare.CLIP_FACTOR * 0.5 * dx;
					double b = Compare.CLIP_FACTOR * 0.5 * dy;

					double angle1 = Math.toRadians(225.0);
					double angle2 = Math.toRadians(45.0);

					double x0 = (cx + a * Math.sin(angle1));
					double y0 = (cy + b * Math.cos(angle1));

					double x1 = (cx + a * Math.sin(angle2));
					double y1 = (cy + b * Math.cos(angle2));

					Envelope clipbox = new Envelope(x0, x1, y0, y1);
					clip_boxes.add(clipbox);
				}
			}
		}
	}

	// sleep to allow buffers and malloc to settle
	// Compare.wait();

	public void runchecks() {
		if (Compare.MEASURE_AREA) {
			double area = 0;
			long t0 = System.nanoTime();
			for (int i = 0; i < Compare.AREA_COUNT; i++) {
				for (Polygon polygon : polygons) {
					area += polygon.getArea();
				}
			}
			long t1 = System.nanoTime();
			Compare.report_area(t1 - t0, polygons.size(), area);
		}

		if (Compare.MEASURE_CENTROID) {
			double sum_x = 0, sum_y = 0;
			long t0 = System.nanoTime();
			for (int i = 0; i < Compare.CENTROID_COUNT; i++) {
				for (Polygon polygon : polygons) {
					Point centroid = polygon.getCentroid();
					sum_x += centroid.getX();
					sum_y += centroid.getY();
				}
			}
			long t1 = System.nanoTime();
			Compare.report_centroid(t1 - t0, polygons.size(), sum_x, sum_y);
		}

		if (Compare.MEASURE_CONVEX_HULL) {
			double area = 0.0;
			long t0 = System.nanoTime();
			for (Polygon polygon : polygons) {
				Geometry hull = polygon.convexHull();
				if (Compare.HULL_AREA) {
					area += Math.abs(hull.getArea());
				}
			}
			long t1 = System.nanoTime();
			Compare.report_hull(t1 - t0, polygons.size(), area);
		}

		if (Compare.MEASURE_OVERLAY) {
			double area1 = 0.0, area2 = 0.0;
			long t0 = System.nanoTime();
			for (int i = 0; i < Compare.OVERLAY_COUNT; i++) {
				// TODO : weird loop C++ port??
				int k = 0;
				Iterator<Polygon> eit = ellipses.iterator();
				for (Iterator<Polygon> pit = polygons.iterator(); pit.hasNext()
						&& eit.hasNext(); k++) {
					Polygon poly = pit.next();
					Polygon ellipse = eit.next();
					if (Compare.OVERLAY_AREA) {
						area1 += poly.getArea();
					}
					Geometry v = ellipse.intersection(poly);
					if (Compare.OVERLAY_AREA) {
						area2 += v.getArea();
					}
				}
			}
			long t1 = System.nanoTime();
			Compare.report_overlay(t1 - t0, polygons.size(), area1, area2);
		}

		if (Compare.MEASURE_CLIP) {
			boolean first = true;
			double area1 = 0.0, area2 = 0.0;

			long t0 = System.nanoTime();

			for (int i = 0; i < Compare.CLIP_COUNT; i++) {
				Iterator<Envelope> bit = clip_boxes.iterator();
				Iterator<Polygon> pit = polygons.iterator();
				for (int k = 0; pit.hasNext() && bit.hasNext(); k++) {
					Polygon poly = pit.next();
					Envelope clipenv = bit.next();
					Geometry clipgeom = geometryFactory.toGeometry(clipenv);
					if (Compare.CLIP_AREA) {
						area1 += poly.getArea();
					}
					Geometry v = clipgeom.intersection(poly);
					if (Compare.CLIP_AREA) {
						area2 += v.getArea();
					}
				}
			}
			long t1 = System.nanoTime();
			Compare.report_clip(t1 - t0, polygons.size(), area1, area2);
		}

		if (Compare.MEASURE_SIMPLIFY) {
			int count1 = 0, count2 = 0;
			double length1 = 0.0, length2 = 0.0;
			long t0 = System.nanoTime();
			for (Polygon polygon : polygons) {
				Geometry simplegeom = DouglasPeuckerSimplifier.simplify(
						polygon, Compare.SIMPLIFY_DISTANCE);
				count1 += polygon.getNumPoints();
				count2 += simplegeom.getNumPoints();
				if (Compare.SIMPLIFY_LENGTH) {
					length1 += polygon.getLength();
					length2 += simplegeom.getLength();
				}

			}
			long t1 = System.nanoTime();
			Compare.report_simplify(t1 - t0, polygons.size(), length1, length2,
					count1, count2);
		}

		// nearly straight port from original
		if (Compare.MEASURE_WITHIN) {
			int count = 0;
			long t0 = System.nanoTime();
			for (int e = 0; e < boxes.size(); e++) {
				Envelope b = boxes.get(e);
				Coordinate c = b.centre();
				// todo: expensive call in jts ! should not be in measurements ;
				Point p = geometryFactory.createPoint(c);
				// TODO : port to java: what is intended with this loop
				// construct? it may be ported wrong
				Iterator<Envelope> bit = boxes.iterator();
				Iterator<Polygon> pit = polygons.iterator();
				for (int k = 0; pit.hasNext() && bit.hasNext(); k++) {
					Polygon poly = pit.next();
					Envelope box = bit.next();
					// todo: jts does not support coordinate.within(envelope);
					// it is checked by p.within anyway so should be removed
					// from the test??
					if (box.contains(c) && p.within(poly)) {
						count++;
					}
				}
			}
			long t1 = System.nanoTime();
			Compare.report_within(t1 - t0, polygons.size(), count, -1);
		}

		// alternative port : calc points first, outside timer
		if (Compare.MEASURE_WITHIN) {

			int count = 0;
			List<Point> points = new ArrayList<Point>(boxes.size());
			for (int e = 0; e < boxes.size(); e++) {
				Envelope b = boxes.get(e);
				Coordinate c = b.centre();
				Point p = geometryFactory.createPoint(c);
				points.add(p);
			}

			long t0 = System.nanoTime();
			Iterator<Point> pointIt = points.iterator();
			Iterator<Polygon> pit = polygons.iterator();
			Iterator<Envelope> bit = boxes.iterator();
			for (int k = 0; pit.hasNext() && bit.hasNext() && pointIt.hasNext(); k++) {
				Polygon poly = pit.next();
				Envelope box = bit.next();
				Point p = pointIt.next();
				// todo: jts does not support coordinate.within(envelope);
				// it is checked by p.within anyway so should be removed from
				// the test??
				if (box.contains(box.centre()) && p.within(poly)) {
					count++;
				}
			}
			long t1 = System.nanoTime();

			System.out
					.println("within alternative port : calc points first, outside timer");
			Compare.report_within(t1 - t0, polygons.size(), count, -1);
		}

		// alternative port : calc points first, outside timer, skip boundingbox
		if (Compare.MEASURE_WITHIN) {
			int count = 0;
			List<Point> points = new ArrayList<Point>(boxes.size());
			for (int e = 0; e < boxes.size(); e++) {
				Envelope b = boxes.get(e);
				Coordinate c = b.centre();
				Point p = geometryFactory.createPoint(c);
				points.add(p);
			}

			long t0 = System.nanoTime();
			Iterator<Point> pointIt = points.iterator();
			Iterator<Polygon> pit = polygons.iterator();
			for (int k = 0; pit.hasNext() && pointIt.hasNext(); k++) {
				Polygon poly = pit.next();
				Point p = pointIt.next();
				if (p.within(poly)) {
					count++;
				}
			}
			long t1 = System.nanoTime();
			System.out
					.println("within alternative port2 : calc points first, outside timer, skip bbox");
			Compare.report_within(t1 - t0, polygons.size(), count, -1);
		}



		// new not in c++ original
		if (Compare.MEASURE_CONTAINS) {
			int count = 0;
			List<Point> points = new ArrayList<Point>(boxes.size());
			for (int e = 0; e < boxes.size(); e++) {
				Envelope b = boxes.get(e);
				Coordinate c = b.centre();
				Point p = geometryFactory.createPoint(c);
				points.add(p);
			}
			long t0 = System.nanoTime();
			Iterator<Point> pointIt = points.iterator();
			Iterator<Polygon> pit = polygons.iterator();
			for (int k = 0; pit.hasNext() && pointIt.hasNext(); k++) {
				Polygon poly = pit.next();
				Point p = pointIt.next();
				if (poly.contains(p)) {
					count++;
				}
			}
			long t1 = System.nanoTime();
			Compare.report_contains(t1 - t0, polygons.size(), count, -1);
		}

		// new not in c++ original, prepared geometry
		if (Compare.MEASURE_CONTAINS) {
			int count = 0;
			List<Point> points = new ArrayList<Point>(boxes.size());
			for (int e = 0; e < boxes.size(); e++) {
				Envelope b = boxes.get(e);
				Coordinate c = b.centre();
				Point p = geometryFactory.createPoint(c);
				points.add(p);
			}

			// and run it again: should be fatser now ???
			long t0 = System.nanoTime();
			Iterator<Point> pointIt = points.iterator();
			Iterator<PreparedPolygon> pit = ppolygons.iterator();
			for (int k = 0; pit.hasNext() && pointIt.hasNext(); k++) {
				PreparedPolygon poly = pit.next();
				Point p = pointIt.next();
				if (poly.contains(p)) {
					count++;
				}
			}
			long t1 = System.nanoTime();
			System.out.println("rerun of contains using prepared polygons");
			Compare.report_contains(t1 - t0, polygons.size(), count, -1);

		}

		// // from geos c++ original , commented: too slow: full n*n comparison
		// if (Compare.MEASURE_TOUCH) {
		// int count = 0;
		// int count_non_disjoint = 0;
		// long t0 = System.nanoTime();
		//
		// int e1 = 0;
		//
		// for (Iterator<Polygon> it1 = polygons.iterator(); it1.hasNext();
		// e1++) {
		// Polygon polygon1 = it1.next();
		// Envelope env1 = polygon1.getEnvelopeInternal();
		// env1.expandBy(2.0);
		// int e2 = 0;
		// for (Iterator<Polygon> it2 = polygons.iterator(); it2.hasNext();
		// e2++) {
		// Polygon polygon2 = it2.next();
		// Envelope env2 = polygon2.getEnvelopeInternal();
		// env2.expandBy(2.0);
		// if (env1.intersects(env2)) {
		// count_non_disjoint++;
		// if (!(polygon1.disjoint(polygon2))) {
		// count++;
		// }
		// }
		// }
		// }
		// long t1 = System.nanoTime();
		// Compare.report_touch(t1 - t0, polygons.size(), count,
		// count_non_disjoint);
		// }

	}

	public static void main(String[] args) throws Exception {
		GeomBenchmark c = new GeomBenchmark();
		File file = new File("c:/data/spatial/shape/c100000jan2902.shp");
		c.prepare(file);
		// wait
		c.runchecks();
	}

	public static void read_shapefile(File file, List<Polygon> polygons,
			List<Integer> ids) {
		try {
			/*
			 * Attmpt to find a GeoTools DataStore that can handle the shapefile
			 */
			Map<String, Serializable> connectParameters = new HashMap<String, Serializable>();

			connectParameters.put("url", file.toURI().toURL());
			connectParameters.put("create spatial index", false);

			DataStore dataStore = DataStoreFinder
					.getDataStore(connectParameters);
			if (dataStore == null) {
				System.out.println("No DataStore found to handle"
						+ file.getPath());
				System.exit(1);
			}

			/*
			 * We are now connected to the shapefile. Get the type name of the
			 * features within it
			 */
			String[] typeNames = dataStore.getTypeNames();
			String typeName = typeNames[0];

			System.out.println("Reading content " + typeName);

			/*
			 * Iterate through the features, collecting some spatial data (line
			 * or boundary length) on each one
			 */
			FeatureSource<SimpleFeatureType, SimpleFeature> featureSource;
			FeatureCollection<SimpleFeatureType, SimpleFeature> collection;
			FeatureIterator<SimpleFeature> iterator;

			featureSource = dataStore.getFeatureSource(typeName);
			collection = featureSource.getFeatures();
			iterator = collection.features();

			int i = 0;
			double totalArea = 0.0;
			try {
				while (iterator.hasNext()) {
					SimpleFeature feature = iterator.next();

					i++;
					// System.out.println(i+"  :"+feature.getID());
					/*
					 * The spatial portion of the feature is represented by a
					 * Geometry object
					 */
					Geometry geometry = (Geometry) feature.getDefaultGeometry();
					// TODO validate polygon?

					// Process only polygons, and from them only single-polygons
					// without holes
					Polygon polygon = null;
					if (geometry instanceof Polygon) {
						polygon = (Polygon) geometry;
					} else if (geometry instanceof MultiPolygon) {
						MultiPolygon mp = (MultiPolygon) geometry;
						if (mp.getNumGeometries() == 1) {
							polygon = (Polygon) mp.getGeometryN(0);
						} else {
							/*System.out
									.println(i
											+ "skipped:  not a single polygon multipolygon");
							*/
						}
					} else {
						System.out.println(i
								+ " skipped: not a (multi)polygon:"
								+ geometry.getGeometryType());
					}

					if (polygon != null) {
						if (polygon.getNumInteriorRing() == 0) {
							totalArea += polygon.getArea();
							polygons.add(polygon);
							ids.add(i);
						} else {
							/*System.out.println(i
									+ "  not a single ring polygon:"
									+ geometry.getGeometryType());
							*/
						}
					}

				}
			} finally {
				/*
				 * You MUST explicitly close the feature iterator otherwise
				 * terrible things will happen !!!
				 */
				if (iterator != null) {
					iterator.close();
				}
			}

			System.out.println("Total Area" + totalArea);

		} catch (Exception ex) {
			ex.printStackTrace();
			System.exit(1);
		}
	}
}