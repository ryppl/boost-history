#ifndef MODELS_H
#define MODELS_H

#include <algorithm>
#include <vector>
#include <math.h>
#include <cassert>
#include <iostream>

#ifndef EULER_NUMBER
#define EULER_NUMBER 2.71828182845904523536028747135
#endif
typedef unsigned char BYTE;
template<class T> inline T SQUARE(T value){return value*value;};
template<class T> inline T EXP(T f_exponent) { return pow((T)EULER_NUMBER, (T)f_exponent); }
template <unsigned int IntensityByteIndex>
class IntensityNeighborHoodFunctor
{
public:
	IntensityNeighborHoodFunctor():m_intensityThreshold(10){
	}
	//Functor
	inline double operator()(const BYTE* fcp_firstPixel,const BYTE* fcp_secondPixel){
		const int intensityDifference(
				abs((*(fcp_firstPixel+IntensityByteIndex)-*(fcp_secondPixel+IntensityByteIndex)))
				);
		return std::max(0.,EXP(-.5*SQUARE(intensityDifference/m_intensityThreshold)));
	}

	inline unsigned int getThreshold() const {
		return m_intensityThreshold;
	}
	
	inline void setThreshold(unsigned int f_intesityThreshold){
		m_intensityThreshold=f_intesityThreshold;
	}

private:
	unsigned int m_intensityThreshold;
};


template <unsigned int IntensityByteIndex>
class IntensityHistogram
{
public:

	IntensityHistogram():
		MC_NUMBER_OF_BUCKETS(256),
		m_values(MC_NUMBER_OF_BUCKETS,0),
		m_histogram(MC_NUMBER_OF_BUCKETS),
		m_numberOfPixels(0),
		m_histogramCalculated(false){
	}
	
	void reset(){
		m_histogramCalculated=false;
		m_numberOfPixels=0;
 		std::fill(m_values.begin(),m_values.end(),0);
	}
	
	inline void addPixel(const BYTE* fcp_pixel){
		m_histogramCalculated=false;
		++m_values[*(fcp_pixel+IntensityByteIndex)];
		++m_numberOfPixels;
	}
	
	inline void calculateHistogram(){
		assert(m_numberOfPixels);
		for(unsigned int i(0);i<MC_NUMBER_OF_BUCKETS;++i)
		{
			//if this value was zero, assign a very small number, not zero, so we can calculate the log(x,e) later from it
			if(m_values[i]==0)
			{
				m_histogram[i]=1./(2.*m_numberOfPixels);
			}
			else
			{
				m_histogram[i]=m_values[i]/static_cast<double>(m_numberOfPixels);
			}
			assert(m_histogram[i]>0);
			assert(m_histogram[i]<=1);
			m_histogram[i]=-log(m_histogram[i]);
		}
		m_histogramCalculated=true;
/*		for(unsigned int i(0);i<MC_NUMBER_OF_BUCKETS;++i)
			std::cout << m_histogram[i]<<std::endl;
		std::cout << "============================="<<std::endl;*/		
	}

	
	inline double getOppositeOfSimilarity(const BYTE* fcp_pixel) const  {
		assert(m_histogramCalculated);
		return m_histogram[*(fcp_pixel+IntensityByteIndex)];
	}
	
	private:
		const unsigned int MC_NUMBER_OF_BUCKETS;
		std::vector<int> m_values;	
		std::vector<double> m_histogram;
		unsigned int m_numberOfPixels;	
		bool m_histogramCalculated;
};
#endif
