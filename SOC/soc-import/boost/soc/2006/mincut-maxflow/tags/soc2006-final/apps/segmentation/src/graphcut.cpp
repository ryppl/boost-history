//
// C++ Implementation: graphcut
//
// Description: 
//
//
// Author: Stephan Diederich <S.Diederich@gmx.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "graphcut.h"
#include "graphkolmogorov.h"
#include "graphfreeimpl.h"
#include "graphboostpushrelabel.h"
#include "graphboostedmundkarp.h"
#include "graphboostkolmogorov.h"
#include <cassert>
#include <iostream>
#include <algorithm>

#include <qdatetime.h> 

GraphCut::GraphCut():m_similarity_weight(1),m_neighborhood_weight(1),m_max_sum_edge_links(0), mp_graph(0),m_dump_problem_as_dimacs_file(false),m_dimacs_file(){
}

GraphCut::~GraphCut(){
	if(mp_graph)
		delete mp_graph;
}

QImage GraphCut::segment( const QImage & fcr_source, const QImage & fcr_trimap, eMode f_mode )
{
	assert(fcr_source.size()==fcr_trimap.size());
	assert(fcr_source.depth()==8 && 	fcr_trimap.depth()==8); // check that both are greyscale images
	//reset the similaritymodel
	m_similarity_models[SimilarityModelFG].reset();
	m_similarity_models[SimilarityModelBG].reset();
	m_neighborhood_model.setThreshold( m_neighborhood_threshold);
	
	const int number_of_pixels=fcr_source.height()*fcr_source.width();
	
   if(m_dump_problem_as_dimacs_file){
     m_dimacs_file.open( m_dimacs_filename.c_str() );
     if( m_dimacs_file.is_open() ){
       m_dimacs_file << "c Dimacs file generated from an image segmentation problem" <<std::endl;
       m_dimacs_file << "c Image size: "<< fcr_source.width() << "x" << fcr_source.height() <<std::endl;
       long number_of_arcs = 
           number_of_pixels * 2 //source->node->sink connect
           + 4 * 2 // 4 edges
           + (fcr_source.width()-2) * 3 * 2 //top and bottom row
           + (fcr_source.height()-2) * 3 * 2 //right- and leftmost column
           + (fcr_source.height()-2) *(fcr_source.width()-2)*4; //everything in the middle
       m_dimacs_file << "p max " << number_of_pixels + 2 /* add 2 for source and sink */ << " " << number_of_arcs <<std::endl;
       m_dimacs_file << "n " << number_of_pixels +1 << " s"<<std::endl;
       m_dimacs_file << "n " << number_of_pixels +2 << " t"<<std::endl;
     }
     else{
       std::cerr << "Could not open output file "<< m_dimacs_filename<<". Disabling dimacs output."<<std::endl;
       m_dump_problem_as_dimacs_file = false;
     }
   }
   
	createGraph(number_of_pixels,f_mode);

	calculateNeighborhoud4(fcr_source);

	updateSimilarityModels(fcr_source,fcr_trimap);
	
	updateGraphWithSimilarityModels(fcr_source, fcr_trimap);
  
   if(m_dump_problem_as_dimacs_file){
     m_dimacs_file.close();
   }
	//everything set... ab die post
	QTime time;
	time.start();
	GraphBase::tPrecision flow = mp_graph->maxflow();
	m_maxflow_time=time.elapsed ();
	std::cout << "Computed max-flow " << flow << std::endl;
	return createOutputImage(fcr_source);
}



void GraphCut::createGraph( int f_numberOfPixels,eMode f_mode )
{
	if(mp_graph)
		delete mp_graph;
	switch(f_mode){
		case KOLMOGOROV:
			mp_graph=new GraphKolmogorov(f_numberOfPixels);
			break;
		case FREEIMPLEMENTATION:
			mp_graph=new GraphFreeImpl(f_numberOfPixels);
			break;
		case BOOST_PUSH_RELABEL:
			mp_graph=new GraphBoostPushRelabel(f_numberOfPixels);
			break;
		case BOOST_EDMUND_KARP:
			mp_graph=new GraphBoostEdmundKarp(f_numberOfPixels);
			break;
		case BOOST_KOLMOGOROV:
			mp_graph=new GraphBoostKolmogorov(f_numberOfPixels);
			break;			
		default:
			assert(0);
	}	
}

void GraphCut::calculateNeighborhoud4(const QImage& fcr_imageToSegment){

	m_max_sum_edge_links=0;

	//some image-infos
	const unsigned int imageHeight(fcr_imageToSegment.height());
	const unsigned int imageWidth(fcr_imageToSegment.width());
	const unsigned int pitch(fcr_imageToSegment.bytesPerLine ());

	//the last row/colum are seperatly processed so they dont have to be caught by if-clauses in the big 
	//loop below	
	//add edges for the last row
	const BYTE* pData = fcr_imageToSegment.scanLine(imageHeight-1);
	for(unsigned int x = 0; x < imageWidth - 1; ++x, ++pData){
		tPrecision cost=static_cast<tPrecision>(m_neighborhood_weight*m_neighborhood_model(pData,pData+1));
		mp_graph->add_edge(x+imageWidth*(imageHeight-1),x+1+imageWidth*(imageHeight-1),cost,cost);
      if(m_dump_problem_as_dimacs_file){
        assert(m_dimacs_file.is_open());
        m_dimacs_file << "a " << x+imageWidth*(imageHeight-1) +1<< " " << x+1+imageWidth*(imageHeight-1) +1<< " " << cost <<std::endl;
        m_dimacs_file << "a " << x+1+imageWidth*(imageHeight-1) +1<< " " << x+imageWidth*(imageHeight-1) +1<< " " << cost <<std::endl;
      }
	}
	//add edges for the last column
	pData = fcr_imageToSegment.bits()+(imageWidth-1);
	for(unsigned int y(0); y < imageHeight-1; ++y, pData += pitch){
		tPrecision cost=static_cast<tPrecision>(m_neighborhood_weight*m_neighborhood_model (pData,pData+pitch));
		mp_graph->add_edge(imageWidth*y+imageWidth-1,imageWidth*(y+1)+imageWidth-1,cost,cost);
      if(m_dump_problem_as_dimacs_file){
        assert(m_dimacs_file.is_open());
        m_dimacs_file << "a " << imageWidth*y+imageWidth-1 +1<< " " << imageWidth*(y+1)+imageWidth-1 +1<< " " << cost <<std::endl;
        m_dimacs_file << "a " << imageWidth*(y+1)+imageWidth-1 +1<< " " << imageWidth*y+imageWidth-1 +1<< " " << cost <<std::endl;
      }      
	}		

	//add 4 edges for each node (take each pixel and add one to the right and one to the bottom)
	//omit last row as well as last column (done above)
	for(unsigned int y(0);y<imageHeight-1;++y){
		for(unsigned int x(0);x<imageWidth-1;++x){
			unsigned int nodeId(y*imageWidth+x);
			const BYTE* currentPixel(fcr_imageToSegment.scanLine(y)+x);
			//one to the right *tamtam*
			tPrecision cost=static_cast<tPrecision>(m_neighborhood_weight*m_neighborhood_model(currentPixel,currentPixel+1));
			m_max_sum_edge_links=std::max(m_max_sum_edge_links,cost);
			mp_graph->add_edge(nodeId,nodeId+1,cost,cost);
         if(m_dump_problem_as_dimacs_file){
           assert(m_dimacs_file.is_open());
           m_dimacs_file << "a " << nodeId +1<< " " << nodeId +1 +1<< " " << cost <<std::endl;
           m_dimacs_file << "a " << nodeId + 1 +1<< " " << nodeId +1<< " " << cost <<std::endl;
         }                        
			//and one to the bottom *tamtam*
			cost=static_cast<tPrecision>(m_neighborhood_weight*m_neighborhood_model (currentPixel,currentPixel+pitch));
			m_max_sum_edge_links=std::max(m_max_sum_edge_links,cost);
			mp_graph->add_edge(nodeId, nodeId + imageWidth, cost, cost);
         if(m_dump_problem_as_dimacs_file){
           assert(m_dimacs_file.is_open());
           m_dimacs_file << "a " << nodeId +1<< " " << nodeId + imageWidth +1<< " " << cost << std::endl;
           m_dimacs_file << "a " << nodeId + imageWidth +1<< " " << nodeId +1<< " " << cost << std::endl;
         }                        
		}
	}
	m_max_sum_edge_links*=tPrecision(4); //we need only an upper boundary
// 	std::cout << "Max sum of edge-links: " << m_max_sum_edge_links << std::endl;	
}

void GraphCut::updateSimilarityModels( const QImage & fcr_imageToSegment, const QImage & fcr_trimap){
	//now we calculate the histo-data
	//that means we go though the trimap, check it for BG or FG label and give that pixel to the approciate SimilarityModel
	const unsigned int imageWidth=fcr_imageToSegment.width();
	const unsigned int imageHeigth=fcr_imageToSegment.height();
	const unsigned int skip=fcr_imageToSegment.bytesPerLine() - fcr_imageToSegment.width();
	const BYTE* pDataImage=fcr_imageToSegment.bits();
	const BYTE* pDataTrimap=fcr_trimap.bits();
	
	IntensityHistogram<0>& rBackgroundModel=m_similarity_models[SimilarityModelBG];
	IntensityHistogram<0>& rForegroundModel=m_similarity_models[SimilarityModelFG];
	for(unsigned int y(0);y<imageHeigth;++y){
		for(unsigned int x(0);x<imageWidth;++x){
			if(*pDataTrimap==TrimapLabelBG)
				rBackgroundModel.addPixel(pDataImage);
			else if(*pDataTrimap==TrimapLabelFG)
				rForegroundModel.addPixel(pDataImage);
			++pDataImage;
			++pDataTrimap;
		}
		pDataImage+=skip;
		pDataTrimap+=skip;
	}
	m_similarity_models[SimilarityModelFG].calculateHistogram();
	m_similarity_models[SimilarityModelBG].calculateHistogram();
}

void GraphCut::updateGraphWithSimilarityModels(const QImage& fcr_imageToSegment, const QImage& fcr_trimap){

	//now we can calculate each pixels similarity to one of the models
	//and set this value as the pixel-terminal-edge-cost
	const unsigned int imageWidth=fcr_imageToSegment.width();
	const unsigned int imageHeigth=fcr_imageToSegment.height();
	const unsigned int skip=fcr_imageToSegment.bytesPerLine()-fcr_imageToSegment.width();
	const BYTE* pDataImage=fcr_imageToSegment.bits();
	const BYTE* pDataTrimap=fcr_trimap.bits();
	
	tPrecision maxToSink(0),maxToSource(0);
	
	for(unsigned int y(0);y<imageHeigth;++y){
		for(unsigned int x(0);x<imageWidth;++x){
			tPrecision costToSource=0;
			tPrecision costToSink=0;
			if(*pDataTrimap==TrimapLabelFG) //set hard-constraint
			{
				costToSource=m_max_sum_edge_links+1;
				costToSink=0;
			}
			else if(*pDataTrimap==TrimapLabelBG) //set hard-constraint
			{
				costToSource=0;
				costToSink=m_max_sum_edge_links+1;
			}
			else
			{
				costToSource=static_cast<tPrecision>(m_similarity_weight*m_similarity_models[SimilarityModelBG].getOppositeOfSimilarity(pDataImage));
				costToSink=static_cast<tPrecision>(m_similarity_weight*m_similarity_models[SimilarityModelFG].getOppositeOfSimilarity(pDataImage));
				assert(costToSource >=0);
				assert(costToSink >= 0);
				maxToSource=std::max(maxToSource,costToSource);
				maxToSink=std::max(maxToSink,costToSink);
			}
			mp_graph->add_tedge(x+y*imageWidth,costToSource,costToSink);
         if(m_dump_problem_as_dimacs_file){
           long source = imageHeigth*imageWidth;
           long sink = source + 1 ;
           assert(m_dimacs_file.is_open());
           m_dimacs_file << "a " << source +1 << " " << x+y*imageWidth +1 << " " << costToSource << std::endl;
           m_dimacs_file << "a " << x+y*imageWidth +1 << " " << sink +1 << " " << costToSink << std::endl;
         }                        
         
			//advance to next pixels
			++pDataImage; 
			++pDataTrimap;
		}
		pDataImage+=skip;
		pDataTrimap+=skip;
	}
		std::cout << "Max cost to Sink/Source: " << maxToSink<< "/" << maxToSource<< std::endl;	
}

QImage GraphCut::createOutputImage(const QImage& fcr_imageToSegment) const{
	QImage returnImage=fcr_imageToSegment.copy();
	BYTE* pReturnImage=returnImage.bits();
	const unsigned int imageWidth=returnImage.width();
	const unsigned int imageHeigth=returnImage.height();
	const unsigned int skip=returnImage.bytesPerLine()-returnImage.width();
		
	//create a new QImage as a copy of the input
	for(unsigned int y(0);y<imageHeigth;++y){
		for(unsigned int x(0);x<imageWidth;++x){
			unsigned int currentNode=x+y*imageWidth;
			if(mp_graph->what_segment(currentNode) == GraphBase::SINK){
				*(pReturnImage)=TrimapLabelBG;
			}
			++pReturnImage;
		}
		pReturnImage+=skip;
	}
	return returnImage;
}

void GraphCut::setNeighborhoodWeight(const tPrecision& theValue){
        m_neighborhood_weight = theValue;
}
GraphCut::tPrecision GraphCut::neighborhoodWeight() const{
	return m_neighborhood_weight;
}

void GraphCut::setNeighborhoodThreshold(unsigned int theValue){
	m_neighborhood_threshold = theValue;
}

unsigned int GraphCut::neighborhoodThreshold() const{
	return m_neighborhood_threshold;
}

GraphCut::tPrecision GraphCut::similarityWeight() const{
        return m_similarity_weight;
}
void GraphCut::setSimilarityWeight(const tPrecision& theValue){
        m_similarity_weight = theValue;
}
int GraphCut::maxflowTime() const{
	return m_maxflow_time;
}

void GraphCut::setDumpProblemAsDimacsFile( bool enabled, const std::string & filename )
{
  m_dump_problem_as_dimacs_file = enabled;
  m_dimacs_filename=filename;
  
}
