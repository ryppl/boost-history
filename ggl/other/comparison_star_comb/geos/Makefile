# Modify these paths for other locations

SRC = /c/svn/ggl/other/comparison_star_comb
SRC_GEOS = ../../geos-3.1.1/source


CXX = gcc -O3 -w
CXXFLAGS = -I$(SRC_GEOS)/headers -I../common

GEOS_CXXFLAGS = -I$(SRC_GEOS)/headers 

RELEASE = ../release

all: $(RELEASE)/geos_starcomb 

OBJS = inlines.o \
	AbstractNode.o \
	AbstractSTRtree.o \
	Assert.o \
	Bintree.o \
	BufferBuilder.o \
	BufferOp.o \
	BufferSubgraph.o \
	ByteOrderDataInStream.o \
	ByteOrderValues.o \
	CentroidArea.o \
	CentroidLine.o \
	CentroidPoint.o \
	CGAlgorithms.o \
	CommonBits.o \
	CommonBitsOp.o \
	CommonBitsRemover.o \
	ConnectedElementLocationFilter.o \
	ConnectedElementPointFilter.o \
	ConnectedInteriorTester.o \
	ConnectedSubgraphFinder.o \
	ConsistentAreaTester.o \
	ConvexHull.o \
	Coordinate.o \
	CoordinateArraySequence.o \
	CoordinateArraySequenceFactory.o \
	CoordinateOperation.o \
	CoordinateSequence.o \
	Depth.o \
	Dimension.o \
	DirectedEdge.o DirectedEdge2.o \
	DirectedEdgeStar.o DirectedEdgeStar2.o \
	DistanceOp.o \
	DoubleBits.o \
	DouglasPeuckerLineSimplifier.o \
	DouglasPeuckerSimplifier.o \
	Edge.o Edge2.o \
	EdgeEnd.o \
	EdgeEndBuilder.o \
	EdgeEndBundle.o \
	EdgeEndBundleStar.o \
	EdgeEndStar.o \
	EdgeIntersection.o \
	EdgeIntersectionList.o \
	EdgeList.o \
	EdgeNodingValidator.o \
	EdgeRing.o EdgeRing2.o \
	EdgeSetNoder.o \
	EdgeString.o \
	ElevationMatrix.o \
	ElevationMatrixCell.o \
	EnhancedPrecisionOp.o \
	Envelope.o \
	FastNodingValidator.o \
	FuzzyPointLocator.o \
	GeometricShapeFactory.o \
	Geometry.o \
	GeometryCollection.o \
	GeometryComponentFilter.o \
	GeometryEditor.o \
	GeometryFactory.o \
	GeometryGraph.o \
	GeometryGraphOperation.o \
	GeometryLocation.o \
	GeometrySnapper.o \
	GeometryTransformer.o \
	GraphComponent.o \
	HCoordinate.o \
	HotPixel.o \
	InteriorPointArea.o \
	InteriorPointLine.o \
	InteriorPointPoint.o \
	IntersectionAdder.o \
	IntersectionFinderAdder.o \
	IntersectionMatrix.o \
	Interval.o Interval2.o \
	IntervalSize.o \
	IsSimpleOp.o \
	IsValidOp.o \
	ItemBoundable.o \
	IteratedNoder.o \
	Key.o Key2.o \
	Label.o \
	LinearRing.o \
	LineBuilder.o \
	LineIntersector.o \
	LineMergeDirectedEdge.o \
	LineMergeEdge.o \
	LineMergeGraph.o \
	LineMerger.o \
	LineSegment.o \
	LineSegmentIndex.o \
	LineSequencer.o \
	LineString.o \
	LineStringSnapper.o \
	Location.o \
	math.o \
	MaximalEdgeRing.o \
	MCIndexNoder.o \
	MCIndexPointSnapper.o \
	MCIndexSnapRounder.o \
	MCPointInRing.o \
	MinimalEdgeRing.o \
	MinimumDiameter.o \
	MonotoneChain.o \
	MonotoneChainBuilder.o \
	MonotoneChainEdge.o \
	MonotoneChainIndexer.o \
	MonotoneChainOverlapAction.o \
	MonotoneChainSelectAction.o \
	MultiLineString.o \
	MultiPoint.o \
	MultiPolygon.o \
	Node.o Node1.o Node2.o Node3.o \
	NodeBase.o NodeBase2.o \
	NodeFactory.o \
	NodeMap.o NodeMap2.o \
	NodingValidator.o \
	NotRepresentableException.o \
	Octant.o \
	OffsetCurveBuilder.o \
	OffsetCurveSetBuilder.o \
	OffsetPointGenerator.o \
	OverlayNodeFactory.o \
	OverlayOp.o \
	OverlayResultValidator.o \
	ParseException.o \
	PlanarGraph.o PlanarGraph2.o \
	Point.o \
	PointBuilder.o \
	PointLocator.o \
	Polygon.o \
	PolygonBuilder.o \
	PolygonizeDirectedEdge.o \
	PolygonizeEdge.o \
	PolygonizeGraph.o \
	Polygonizer.o \
	Position.o \
	PrecisionModel.o \
	Profiler.o \
	Quadrant.o \
	Quadtree.o \
	QuadtreeNestedRingTester.o \
	RectangleContains.o \
	RectangleIntersects.o \
	RelateComputer.o \
	RelateNode.o \
	RelateNodeFactory.o \
	RelateNodeGraph.o \
	RelateOp.o \
	RepeatedPointTester.o \
	RightmostEdgeFinder.o \
	RobustDeterminant.o \
	Root.o Root2.o \
	ScaledNoder.o \
	SegmentIntersectionTester.o \
	SegmentIntersector.o \
	SegmentNode.o \
	SegmentNodeList.o \
	SegmentString.o \
	ShortCircuitedGeometryVisitor.o \
	SimpleEdgeSetIntersector.o \
	SimpleGeometryPrecisionReducer.o \
	SimpleMCSweepLineIntersector.o \
	SimpleNestedRingTester.o \
	SimpleNoder.o \
	SimplePointInAreaLocator.o \
	SimplePointInRing.o \
	SimpleSnapRounder.o \
	SimpleSweepLineIntersector.o \
	SingleInteriorIntersectionFinder.o \
	SIRtree.o \
	SIRtreePointInRing.o \
	StringTokenizer.o \
	STRtree.o \
	SubGraph.o \
	SubgraphDepthLocater.o \
	SweepLineEvent.o SweepLineEvent2.o \
	SweepLineIndex.o \
	SweepLineInterval.o \
	SweeplineNestedRingTester.o \
	SweepLineSegment.o \
	TaggedLineSegment.o \
	TaggedLinesSimplifier.o \
	TaggedLineString.o \
	TaggedLineStringSimplifier.o \
	TopologyLocation.o \
	TopologyPreservingSimplifier.o \
	TopologyValidationError.o \
	Triangle.o \
	Unload.o \
	WKBReader.o \
	WKBWriter.o \
	WKTReader.o \
	WKTWriter.o \
	Writer.o
	
	
	
	
$(RELEASE)/geos_starcomb: $(OBJS) geos_starcomb.o 
	$(CXX) $(CXXFLAGS) $(OBJS) geos_starcomb.o  -o $(RELEASE)/geos_starcomb -lstdc++
	
geos_starcomb.o: $(SRC)/geos/geos_starcomb.cpp $(SRC)/common/starcomb.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/geos/geos_starcomb.cpp


inlines.o: $(SRC_GEOS)/inlines.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/inlines.cpp
	
DirectedEdge.o: $(SRC_GEOS)/planargraph/DirectedEdge.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/planargraph/DirectedEdge.cpp
	
DirectedEdgeStar.o: $(SRC_GEOS)/planargraph/DirectedEdgeStar.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/planargraph/DirectedEdgeStar.cpp
	
Edge.o: $(SRC_GEOS)/planargraph/Edge.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/planargraph/Edge.cpp
	
Node.o: $(SRC_GEOS)/planargraph/Node.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/planargraph/Node.cpp
	
NodeMap.o: $(SRC_GEOS)/planargraph/NodeMap.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/planargraph/NodeMap.cpp
	
PlanarGraph.o: $(SRC_GEOS)/planargraph/PlanarGraph.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/planargraph/PlanarGraph.cpp
	
SubGraph.o: $(SRC_GEOS)/planargraph/SubGraph.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/planargraph/SubGraph.cpp
	
DoubleBits.o: $(SRC_GEOS)/index/quadtree/DoubleBits.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/quadtree/DoubleBits.cpp
	
IntervalSize.o: $(SRC_GEOS)/index/quadtree/IntervalSize.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/quadtree/IntervalSize.cpp
	
Key.o: $(SRC_GEOS)/index/quadtree/Key.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/quadtree/Key.cpp
	
Node1.o: $(SRC_GEOS)/index/quadtree/Node.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/quadtree/Node.cpp -o Node1.o
	
NodeBase.o: $(SRC_GEOS)/index/quadtree/NodeBase.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/quadtree/NodeBase.cpp
	
Quadtree.o: $(SRC_GEOS)/index/quadtree/Quadtree.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/quadtree/Quadtree.cpp
	
QuadtreeNestedRingTester.o: $(SRC_GEOS)/operation/valid/QuadtreeNestedRingTester.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/valid/QuadtreeNestedRingTester.cpp
	
Root.o: $(SRC_GEOS)/index/quadtree/Root.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/quadtree/Root.cpp
	
Bintree.o: $(SRC_GEOS)/index/bintree/Bintree.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/bintree/Bintree.cpp
	
Interval.o: $(SRC_GEOS)/index/bintree/Interval.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/bintree/Interval.cpp
	
Key2.o: $(SRC_GEOS)/index/bintree/Key.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/bintree/Key.cpp -o Key2.o
	
Node2.o: $(SRC_GEOS)/index/bintree/Node.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/bintree/Node.cpp -o Node2.o
	
NodeBase2.o: $(SRC_GEOS)/index/bintree/NodeBase.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/bintree/NodeBase.cpp -o NodeBase2.o
	
Root2.o: $(SRC_GEOS)/index/bintree/Root.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/bintree/Root.cpp -o Root2.o
	
MonotoneChain.o: $(SRC_GEOS)/index/chain/MonotoneChain.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/chain/MonotoneChain.cpp
	
MonotoneChainBuilder.o: $(SRC_GEOS)/index/chain/MonotoneChainBuilder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/chain/MonotoneChainBuilder.cpp
	
MonotoneChainOverlapAction.o: $(SRC_GEOS)/index/chain/MonotoneChainOverlapAction.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/chain/MonotoneChainOverlapAction.cpp
	
MonotoneChainSelectAction.o: $(SRC_GEOS)/index/chain/MonotoneChainSelectAction.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/chain/MonotoneChainSelectAction.cpp
	
AbstractNode.o: $(SRC_GEOS)/index/strtree/AbstractNode.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/strtree/AbstractNode.cpp
	
AbstractSTRtree.o: $(SRC_GEOS)/index/strtree/AbstractSTRtree.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/strtree/AbstractSTRtree.cpp
	
Interval2.o: $(SRC_GEOS)/index/strtree/Interval.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/strtree/Interval.cpp -o Interval2.o
	
ItemBoundable.o: $(SRC_GEOS)/index/strtree/ItemBoundable.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/strtree/ItemBoundable.cpp
	
SIRtree.o: $(SRC_GEOS)/index/strtree/SIRtree.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/strtree/SIRtree.cpp
	
STRtree.o: $(SRC_GEOS)/index/strtree/STRtree.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/strtree/STRtree.cpp
	
SweepLineEvent.o: $(SRC_GEOS)/index/sweepline/SweepLineEvent.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/sweepline/SweepLineEvent.cpp
	
SweepLineIndex.o: $(SRC_GEOS)/index/sweepline/SweepLineIndex.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/sweepline/SweepLineIndex.cpp
	
SweepLineInterval.o: $(SRC_GEOS)/index/sweepline/SweepLineInterval.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/index/sweepline/SweepLineInterval.cpp
	
CentroidArea.o: $(SRC_GEOS)/algorithm/CentroidArea.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/CentroidArea.cpp
	
CentroidLine.o: $(SRC_GEOS)/algorithm/CentroidLine.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/CentroidLine.cpp
	
CentroidPoint.o: $(SRC_GEOS)/algorithm/CentroidPoint.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/CentroidPoint.cpp
	
CGAlgorithms.o: $(SRC_GEOS)/algorithm/CGAlgorithms.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/CGAlgorithms.cpp
	
ConvexHull.o: $(SRC_GEOS)/algorithm/ConvexHull.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/ConvexHull.cpp
	
HCoordinate.o: $(SRC_GEOS)/algorithm/HCoordinate.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/HCoordinate.cpp
	
InteriorPointArea.o: $(SRC_GEOS)/algorithm/InteriorPointArea.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/InteriorPointArea.cpp
	
InteriorPointLine.o: $(SRC_GEOS)/algorithm/InteriorPointLine.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/InteriorPointLine.cpp
	
InteriorPointPoint.o: $(SRC_GEOS)/algorithm/InteriorPointPoint.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/InteriorPointPoint.cpp
	
LineIntersector.o: $(SRC_GEOS)/algorithm/LineIntersector.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/LineIntersector.cpp
	
MCPointInRing.o: $(SRC_GEOS)/algorithm/MCPointInRing.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/MCPointInRing.cpp
	
MinimumDiameter.o: $(SRC_GEOS)/algorithm/MinimumDiameter.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/MinimumDiameter.cpp
	
NotRepresentableException.o: $(SRC_GEOS)/algorithm/NotRepresentableException.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/NotRepresentableException.cpp
	
PointLocator.o: $(SRC_GEOS)/algorithm/PointLocator.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/PointLocator.cpp
	
RobustDeterminant.o: $(SRC_GEOS)/algorithm/RobustDeterminant.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/RobustDeterminant.cpp
	
SimplePointInAreaLocator.o: $(SRC_GEOS)/algorithm/locate/SimplePointInAreaLocator.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/locate/SimplePointInAreaLocator.cpp
	
SimplePointInRing.o: $(SRC_GEOS)/algorithm/SimplePointInRing.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/SimplePointInRing.cpp
	
SIRtreePointInRing.o: $(SRC_GEOS)/algorithm/SIRtreePointInRing.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/algorithm/SIRtreePointInRing.cpp
	
Coordinate.o: $(SRC_GEOS)/geom/Coordinate.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/Coordinate.cpp
	
CoordinateArraySequence.o: $(SRC_GEOS)/geom/CoordinateArraySequence.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/CoordinateArraySequence.cpp
	
CoordinateArraySequenceFactory.o: $(SRC_GEOS)/geom/CoordinateArraySequenceFactory.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/CoordinateArraySequenceFactory.cpp
	
CoordinateOperation.o: $(SRC_GEOS)/geom/util/CoordinateOperation.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/util/CoordinateOperation.cpp
	
CoordinateSequence.o: $(SRC_GEOS)/geom/CoordinateSequence.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/CoordinateSequence.cpp
	
Dimension.o: $(SRC_GEOS)/geom/Dimension.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/Dimension.cpp
	
Envelope.o: $(SRC_GEOS)/geom/Envelope.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/Envelope.cpp
	
Geometry.o: $(SRC_GEOS)/geom/Geometry.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/Geometry.cpp
	
GeometryCollection.o: $(SRC_GEOS)/geom/GeometryCollection.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/GeometryCollection.cpp
	
GeometryComponentFilter.o: $(SRC_GEOS)/geom/GeometryComponentFilter.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/GeometryComponentFilter.cpp
	
GeometryEditor.o: $(SRC_GEOS)/geom/util/GeometryEditor.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/util/GeometryEditor.cpp
	
GeometryFactory.o: $(SRC_GEOS)/geom/GeometryFactory.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/GeometryFactory.cpp
	
GeometryTransformer.o: $(SRC_GEOS)/geom/util/GeometryTransformer.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/util/GeometryTransformer.cpp
	
IntersectionMatrix.o: $(SRC_GEOS)/geom/IntersectionMatrix.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/IntersectionMatrix.cpp
	
LinearRing.o: $(SRC_GEOS)/geom/LinearRing.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/LinearRing.cpp
	
LineSegment.o: $(SRC_GEOS)/geom/LineSegment.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/LineSegment.cpp
	
LineString.o: $(SRC_GEOS)/geom/LineString.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/LineString.cpp
	
Location.o: $(SRC_GEOS)/geom/Location.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/Location.cpp
	
MultiLineString.o: $(SRC_GEOS)/geom/MultiLineString.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/MultiLineString.cpp
	
MultiPoint.o: $(SRC_GEOS)/geom/MultiPoint.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/MultiPoint.cpp
	
MultiPolygon.o: $(SRC_GEOS)/geom/MultiPolygon.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/MultiPolygon.cpp
	
Point.o: $(SRC_GEOS)/geom/Point.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/Point.cpp
	
Polygon.o: $(SRC_GEOS)/geom/Polygon.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/Polygon.cpp
	
PrecisionModel.o: $(SRC_GEOS)/geom/PrecisionModel.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/PrecisionModel.cpp
	
ShortCircuitedGeometryVisitor.o: $(SRC_GEOS)/geom/util/ShortCircuitedGeometryVisitor.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/util/ShortCircuitedGeometryVisitor.cpp
	
Triangle.o: $(SRC_GEOS)/geom/Triangle.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geom/Triangle.cpp
	
ByteOrderDataInStream.o: $(SRC_GEOS)/io/ByteOrderDataInStream.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/ByteOrderDataInStream.cpp
	
ByteOrderValues.o: $(SRC_GEOS)/io/ByteOrderValues.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/ByteOrderValues.cpp
	
ParseException.o: $(SRC_GEOS)/io/ParseException.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/ParseException.cpp
	
StringTokenizer.o: $(SRC_GEOS)/io/StringTokenizer.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/StringTokenizer.cpp
	
Unload.o: $(SRC_GEOS)/io/Unload.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/Unload.cpp
	
WKBReader.o: $(SRC_GEOS)/io/WKBReader.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/WKBReader.cpp
	
WKBWriter.o: $(SRC_GEOS)/io/WKBWriter.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/WKBWriter.cpp
	
WKTReader.o: $(SRC_GEOS)/io/WKTReader.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/WKTReader.cpp
	
WKTWriter.o: $(SRC_GEOS)/io/WKTWriter.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/WKTWriter.cpp
	
Writer.o: $(SRC_GEOS)/io/Writer.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/io/Writer.cpp
	
FastNodingValidator.o: $(SRC_GEOS)/noding/FastNodingValidator.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/FastNodingValidator.cpp
	
HotPixel.o: $(SRC_GEOS)/noding/snapround/HotPixel.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/snapround/HotPixel.cpp
	
IntersectionAdder.o: $(SRC_GEOS)/noding/IntersectionAdder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/IntersectionAdder.cpp
	
IntersectionFinderAdder.o: $(SRC_GEOS)/noding/IntersectionFinderAdder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/IntersectionFinderAdder.cpp
	
IteratedNoder.o: $(SRC_GEOS)/noding/IteratedNoder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/IteratedNoder.cpp
	
MCIndexNoder.o: $(SRC_GEOS)/noding/MCIndexNoder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/MCIndexNoder.cpp
	
MCIndexPointSnapper.o: $(SRC_GEOS)/noding/snapround/MCIndexPointSnapper.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/snapround/MCIndexPointSnapper.cpp
	
MCIndexSnapRounder.o: $(SRC_GEOS)/noding/snapround/MCIndexSnapRounder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/snapround/MCIndexSnapRounder.cpp
	
NodingValidator.o: $(SRC_GEOS)/noding/NodingValidator.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/NodingValidator.cpp
	
Octant.o: $(SRC_GEOS)/noding/Octant.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/Octant.cpp
	
ScaledNoder.o: $(SRC_GEOS)/noding/ScaledNoder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/ScaledNoder.cpp
	
SegmentNode.o: $(SRC_GEOS)/noding/SegmentNode.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/SegmentNode.cpp
	
SegmentNodeList.o: $(SRC_GEOS)/noding/SegmentNodeList.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/SegmentNodeList.cpp
	
SegmentString.o: $(SRC_GEOS)/noding/SegmentString.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/SegmentString.cpp
	
SimpleNoder.o: $(SRC_GEOS)/noding/SimpleNoder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/SimpleNoder.cpp
	
SimpleSnapRounder.o: $(SRC_GEOS)/noding/snapround/SimpleSnapRounder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/snapround/SimpleSnapRounder.cpp
	
SingleInteriorIntersectionFinder.o: $(SRC_GEOS)/noding/SingleInteriorIntersectionFinder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/noding/SingleInteriorIntersectionFinder.cpp
	
BufferBuilder.o: $(SRC_GEOS)/operation/buffer/BufferBuilder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/buffer/BufferBuilder.cpp
	
BufferOp.o: $(SRC_GEOS)/operation/buffer/BufferOp.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/buffer/BufferOp.cpp
	
BufferSubgraph.o: $(SRC_GEOS)/operation/buffer/BufferSubgraph.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/buffer/BufferSubgraph.cpp
	
ConnectedElementLocationFilter.o: $(SRC_GEOS)/operation/distance/ConnectedElementLocationFilter.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/distance/ConnectedElementLocationFilter.cpp
	
ConnectedElementPointFilter.o: $(SRC_GEOS)/operation/distance/ConnectedElementPointFilter.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/distance/ConnectedElementPointFilter.cpp
	
ConnectedInteriorTester.o: $(SRC_GEOS)/operation/valid/ConnectedInteriorTester.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/valid/ConnectedInteriorTester.cpp
	
ConsistentAreaTester.o: $(SRC_GEOS)/operation/valid/ConsistentAreaTester.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/valid/ConsistentAreaTester.cpp
	
DistanceOp.o: $(SRC_GEOS)/operation/distance/DistanceOp.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/distance/DistanceOp.cpp
	
EdgeEndBuilder.o: $(SRC_GEOS)/operation/relate/EdgeEndBuilder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/relate/EdgeEndBuilder.cpp
	
EdgeEndBundle.o: $(SRC_GEOS)/operation/relate/EdgeEndBundle.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/relate/EdgeEndBundle.cpp
	
EdgeEndBundleStar.o: $(SRC_GEOS)/operation/relate/EdgeEndBundleStar.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/relate/EdgeEndBundleStar.cpp
	
EdgeRing.o: $(SRC_GEOS)/operation/polygonize/EdgeRing.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/polygonize/EdgeRing.cpp
	
EdgeSetNoder.o: $(SRC_GEOS)/operation/overlay/EdgeSetNoder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/EdgeSetNoder.cpp
	
EdgeString.o: $(SRC_GEOS)/operation/linemerge/EdgeString.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/linemerge/EdgeString.cpp
	
ElevationMatrix.o: $(SRC_GEOS)/operation/overlay/ElevationMatrix.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/ElevationMatrix.cpp
	
ElevationMatrixCell.o: $(SRC_GEOS)/operation/overlay/ElevationMatrixCell.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/ElevationMatrixCell.cpp
	
FuzzyPointLocator.o: $(SRC_GEOS)/operation/overlay/FuzzyPointLocator.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/FuzzyPointLocator.cpp
	
GeometryGraphOperation.o: $(SRC_GEOS)/operation/GeometryGraphOperation.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/GeometryGraphOperation.cpp
	
GeometryLocation.o: $(SRC_GEOS)/operation/distance/GeometryLocation.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/distance/GeometryLocation.cpp
	
IsSimpleOp.o: $(SRC_GEOS)/operation/IsSimpleOp.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/IsSimpleOp.cpp
	
IsValidOp.o: $(SRC_GEOS)/operation/valid/IsValidOp.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/valid/IsValidOp.cpp
	
LineBuilder.o: $(SRC_GEOS)/operation/overlay/LineBuilder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/LineBuilder.cpp
	
LineMergeDirectedEdge.o: $(SRC_GEOS)/operation/linemerge/LineMergeDirectedEdge.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/linemerge/LineMergeDirectedEdge.cpp
	
LineMergeEdge.o: $(SRC_GEOS)/operation/linemerge/LineMergeEdge.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/linemerge/LineMergeEdge.cpp
	
LineMergeGraph.o: $(SRC_GEOS)/operation/linemerge/LineMergeGraph.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/linemerge/LineMergeGraph.cpp
	
LineMerger.o: $(SRC_GEOS)/operation/linemerge/LineMerger.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/linemerge/LineMerger.cpp
	
LineSequencer.o: $(SRC_GEOS)/operation/linemerge/LineSequencer.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/linemerge/LineSequencer.cpp
	
MaximalEdgeRing.o: $(SRC_GEOS)/operation/overlay/MaximalEdgeRing.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/MaximalEdgeRing.cpp
	
MinimalEdgeRing.o: $(SRC_GEOS)/operation/overlay/MinimalEdgeRing.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/MinimalEdgeRing.cpp
	
OffsetCurveBuilder.o: $(SRC_GEOS)/operation/buffer/OffsetCurveBuilder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/buffer/OffsetCurveBuilder.cpp
	
OffsetCurveSetBuilder.o: $(SRC_GEOS)/operation/buffer/OffsetCurveSetBuilder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/buffer/OffsetCurveSetBuilder.cpp
	
OffsetPointGenerator.o: $(SRC_GEOS)/operation/overlay/OffsetPointGenerator.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/OffsetPointGenerator.cpp
	
OverlayNodeFactory.o: $(SRC_GEOS)/operation/overlay/OverlayNodeFactory.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/OverlayNodeFactory.cpp
	
OverlayOp.o: $(SRC_GEOS)/operation/overlay/OverlayOp.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/OverlayOp.cpp
	
OverlayResultValidator.o: $(SRC_GEOS)/operation/overlay/OverlayResultValidator.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/OverlayResultValidator.cpp
	
PointBuilder.o: $(SRC_GEOS)/operation/overlay/PointBuilder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/PointBuilder.cpp
	
PolygonBuilder.o: $(SRC_GEOS)/operation/overlay/PolygonBuilder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/overlay/PolygonBuilder.cpp
	
PolygonizeDirectedEdge.o: $(SRC_GEOS)/operation/polygonize/PolygonizeDirectedEdge.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/polygonize/PolygonizeDirectedEdge.cpp
	
PolygonizeEdge.o: $(SRC_GEOS)/operation/polygonize/PolygonizeEdge.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/polygonize/PolygonizeEdge.cpp
	
PolygonizeGraph.o: $(SRC_GEOS)/operation/polygonize/PolygonizeGraph.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/polygonize/PolygonizeGraph.cpp
	
Polygonizer.o: $(SRC_GEOS)/operation/polygonize/Polygonizer.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/polygonize/Polygonizer.cpp
	
RectangleContains.o: $(SRC_GEOS)/operation/predicate/RectangleContains.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/predicate/RectangleContains.cpp
	
RectangleIntersects.o: $(SRC_GEOS)/operation/predicate/RectangleIntersects.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/predicate/RectangleIntersects.cpp
	
RelateComputer.o: $(SRC_GEOS)/operation/relate/RelateComputer.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/relate/RelateComputer.cpp
	
RelateNode.o: $(SRC_GEOS)/operation/relate/RelateNode.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/relate/RelateNode.cpp
	
RelateNodeFactory.o: $(SRC_GEOS)/operation/relate/RelateNodeFactory.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/relate/RelateNodeFactory.cpp
	
RelateNodeGraph.o: $(SRC_GEOS)/operation/relate/RelateNodeGraph.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/relate/RelateNodeGraph.cpp
	
RelateOp.o: $(SRC_GEOS)/operation/relate/RelateOp.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/relate/RelateOp.cpp
	
RepeatedPointTester.o: $(SRC_GEOS)/operation/valid/RepeatedPointTester.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/valid/RepeatedPointTester.cpp
	
RightmostEdgeFinder.o: $(SRC_GEOS)/operation/buffer/RightmostEdgeFinder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/buffer/RightmostEdgeFinder.cpp
	
SegmentIntersectionTester.o: $(SRC_GEOS)/operation/predicate/SegmentIntersectionTester.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/predicate/SegmentIntersectionTester.cpp
	
SimpleNestedRingTester.o: $(SRC_GEOS)/operation/valid/SimpleNestedRingTester.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/valid/SimpleNestedRingTester.cpp
	
SubgraphDepthLocater.o: $(SRC_GEOS)/operation/buffer/SubgraphDepthLocater.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/buffer/SubgraphDepthLocater.cpp
	
SweeplineNestedRingTester.o: $(SRC_GEOS)/operation/valid/SweeplineNestedRingTester.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/valid/SweeplineNestedRingTester.cpp
	
TopologyValidationError.o: $(SRC_GEOS)/operation/valid/TopologyValidationError.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/operation/valid/TopologyValidationError.cpp
	
CommonBits.o: $(SRC_GEOS)/precision/CommonBits.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/precision/CommonBits.cpp
	
CommonBitsOp.o: $(SRC_GEOS)/precision/CommonBitsOp.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/precision/CommonBitsOp.cpp
	
CommonBitsRemover.o: $(SRC_GEOS)/precision/CommonBitsRemover.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/precision/CommonBitsRemover.cpp
	
EnhancedPrecisionOp.o: $(SRC_GEOS)/precision/EnhancedPrecisionOp.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/precision/EnhancedPrecisionOp.cpp
	
GeometrySnapper.o: $(SRC_GEOS)/precision/GeometrySnapper.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/precision/GeometrySnapper.cpp
	
LineStringSnapper.o: $(SRC_GEOS)/precision/LineStringSnapper.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/precision/LineStringSnapper.cpp
	
SimpleGeometryPrecisionReducer.o: $(SRC_GEOS)/precision/SimpleGeometryPrecisionReducer.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/precision/SimpleGeometryPrecisionReducer.cpp
	
DouglasPeuckerLineSimplifier.o: $(SRC_GEOS)/simplify/DouglasPeuckerLineSimplifier.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/simplify/DouglasPeuckerLineSimplifier.cpp
	
DouglasPeuckerSimplifier.o: $(SRC_GEOS)/simplify/DouglasPeuckerSimplifier.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/simplify/DouglasPeuckerSimplifier.cpp
	
LineSegmentIndex.o: $(SRC_GEOS)/simplify/LineSegmentIndex.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/simplify/LineSegmentIndex.cpp
	
TaggedLineSegment.o: $(SRC_GEOS)/simplify/TaggedLineSegment.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/simplify/TaggedLineSegment.cpp
	
TaggedLinesSimplifier.o: $(SRC_GEOS)/simplify/TaggedLinesSimplifier.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/simplify/TaggedLinesSimplifier.cpp
	
TaggedLineString.o: $(SRC_GEOS)/simplify/TaggedLineString.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/simplify/TaggedLineString.cpp
	
TaggedLineStringSimplifier.o: $(SRC_GEOS)/simplify/TaggedLineStringSimplifier.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/simplify/TaggedLineStringSimplifier.cpp
	
TopologyPreservingSimplifier.o: $(SRC_GEOS)/simplify/TopologyPreservingSimplifier.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/simplify/TopologyPreservingSimplifier.cpp
	
Assert.o: $(SRC_GEOS)/util/Assert.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/util/Assert.cpp
	
ConnectedSubgraphFinder.o: $(SRC_GEOS)/planargraph/algorithm/ConnectedSubgraphFinder.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/planargraph/algorithm/ConnectedSubgraphFinder.cpp
	
GeometricShapeFactory.o: $(SRC_GEOS)/util/GeometricShapeFactory.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/util/GeometricShapeFactory.cpp
	
math.o: $(SRC_GEOS)/util/math.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/util/math.cpp
	
Profiler.o: $(SRC_GEOS)/util/Profiler.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/util/Profiler.cpp
	
Depth.o: $(SRC_GEOS)/geomgraph/Depth.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/Depth.cpp
	
DirectedEdge2.o: $(SRC_GEOS)/geomgraph/DirectedEdge.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/DirectedEdge.cpp -o DirectedEdge2.o
	
DirectedEdgeStar2.o: $(SRC_GEOS)/geomgraph/DirectedEdgeStar.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/DirectedEdgeStar.cpp -o DirectedEdgeStar2.o
	
Edge2.o: $(SRC_GEOS)/geomgraph/Edge.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/Edge.cpp -o Edge2.o
	
EdgeEnd.o: $(SRC_GEOS)/geomgraph/EdgeEnd.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/EdgeEnd.cpp
	
EdgeEndStar.o: $(SRC_GEOS)/geomgraph/EdgeEndStar.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/EdgeEndStar.cpp
	
EdgeIntersection.o: $(SRC_GEOS)/geomgraph/EdgeIntersection.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/EdgeIntersection.cpp
	
EdgeIntersectionList.o: $(SRC_GEOS)/geomgraph/EdgeIntersectionList.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/EdgeIntersectionList.cpp
	
EdgeList.o: $(SRC_GEOS)/geomgraph/EdgeList.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/EdgeList.cpp
	
EdgeNodingValidator.o: $(SRC_GEOS)/geomgraph/EdgeNodingValidator.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/EdgeNodingValidator.cpp
	
EdgeRing2.o: $(SRC_GEOS)/geomgraph/EdgeRing.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/EdgeRing.cpp -o EdgeRing2.o
	
GeometryGraph.o: $(SRC_GEOS)/geomgraph/GeometryGraph.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/GeometryGraph.cpp
	
GraphComponent.o: $(SRC_GEOS)/geomgraph/GraphComponent.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/GraphComponent.cpp
	
Label.o: $(SRC_GEOS)/geomgraph/Label.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/Label.cpp
	
MonotoneChainEdge.o: $(SRC_GEOS)/geomgraph/index/MonotoneChainEdge.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/index/MonotoneChainEdge.cpp
	
MonotoneChainIndexer.o: $(SRC_GEOS)/geomgraph/index/MonotoneChainIndexer.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/index/MonotoneChainIndexer.cpp
	
Node3.o: $(SRC_GEOS)/geomgraph/Node.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/Node.cpp -o Node3.o
	
NodeFactory.o: $(SRC_GEOS)/geomgraph/NodeFactory.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/NodeFactory.cpp
	
NodeMap2.o: $(SRC_GEOS)/geomgraph/NodeMap.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/NodeMap.cpp -o NodeMap2.o
	
PlanarGraph2.o: $(SRC_GEOS)/geomgraph/PlanarGraph.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/PlanarGraph.cpp -o PlanarGraph2.o
	
Position.o: $(SRC_GEOS)/geomgraph/Position.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/Position.cpp
	
Quadrant.o: $(SRC_GEOS)/geomgraph/Quadrant.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/Quadrant.cpp
	
SegmentIntersector.o: $(SRC_GEOS)/geomgraph/index/SegmentIntersector.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/index/SegmentIntersector.cpp
	
SimpleEdgeSetIntersector.o: $(SRC_GEOS)/geomgraph/index/SimpleEdgeSetIntersector.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/index/SimpleEdgeSetIntersector.cpp
	
SimpleMCSweepLineIntersector.o: $(SRC_GEOS)/geomgraph/index/SimpleMCSweepLineIntersector.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/index/SimpleMCSweepLineIntersector.cpp
	
SimpleSweepLineIntersector.o: $(SRC_GEOS)/geomgraph/index/SimpleSweepLineIntersector.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/index/SimpleSweepLineIntersector.cpp
	
SweepLineEvent2.o: $(SRC_GEOS)/geomgraph/index/SweepLineEvent.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/index/SweepLineEvent.cpp -o SweepLineEvent2.o
	
SweepLineSegment.o: $(SRC_GEOS)/geomgraph/index/SweepLineSegment.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/index/SweepLineSegment.cpp
	
TopologyLocation.o: $(SRC_GEOS)/geomgraph/TopologyLocation.cpp
	$(CXX) -c $(GEOS_CXXFLAGS) $(SRC_GEOS)/geomgraph/TopologyLocation.cpp
	
	
