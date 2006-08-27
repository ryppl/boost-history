//
// C++ Interface: graphcut
//
// Description: 
//
//
// Author: Stephan Diederich <S.Diederich@gmx.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GRAPHCUT_H
#define GRAPHCUT_H

#include <qimage.h>
#include <graphbase.h>
#include <fstream>
#include "models.h"

/**
	@author Stephan Diederich <S.Diederich@gmx.de>
*/
	
class GraphCut{
public:
	typedef GraphBase::tPrecision tPrecision;
	
	enum eMode{
		KOLMOGOROV,
		FREEIMPLEMENTATION,
		BOOST_PUSH_RELABEL,
		BOOST_EDMUND_KARP,
		BOOST_KOLMOGOROV
	};
	enum eTrimapLabels{
		TrimapLabelFG=255,
		TrimapLabelBG=0
	};
	enum eSimilarityModel{
		SimilarityModelFG=0,
		SimilarityModelBG=1
	};
			
   GraphCut();
   ~GraphCut();
	 
	///starts segmentation of fcr_source with specified trimap fcr_trimap
	QImage segment(const QImage& fcr_source,const QImage& fcr_trimap,eMode f_mode);

	//
	// Member access
	//
	void setSimilarityWeight(const tPrecision& theValue);
	tPrecision similarityWeight() const;	

	void setNeighborhoodWeight(const tPrecision& theValue);
	tPrecision neighborhoodWeight() const;
	
	void setNeighborhoodThreshold(unsigned int theValue);
	unsigned int neighborhoodThreshold() const;
	
	int maxflowTime() const;
	
   void setDumpProblemAsDimacsFile( bool enabled, const std::string& filename);
   
private:
	void createGraph(int f_numberOfPixels,eMode f_mode);
	
	///updates the graph's edges with values got from a neighborhoodmodel
	void calculateNeighborhoud4(const QImage& fcr_imageToSegment);
	
	///updates similarity models with the images pixels specified by trimap
	void updateSimilarityModels(const QImage& fcr_imageToSegment, const QImage& fcr_trimap);
	
	///updates the graph's edges with the values got from the similarity models
	void updateGraphWithSimilarityModels(const QImage& fcr_imageToSegment,const QImage& fcr_trimap);

	QImage createOutputImage(const QImage& fcr_imageToSegment) const;
	
	tPrecision m_similarity_weight;
	tPrecision m_neighborhood_weight;
	unsigned int m_neighborhood_threshold;
	tPrecision m_max_sum_edge_links;
	
	GraphBase* mp_graph;
	IntensityNeighborHoodFunctor<0> m_neighborhood_model;
	IntensityHistogram<0> m_similarity_models[2];
	int m_maxflow_time;
   bool m_dump_problem_as_dimacs_file;
   std::string m_dimacs_filename;
   std::ofstream m_dimacs_file;
};

#endif
