//
// C++ Interface: 
//
// Description: 
//
//
// Author: Stephan Diederich <S.Diederich@gmx.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <string>
#include <limits>
#include <list>

#include <qbitmap.h>
#include <qimage.h>

#include "segmentationbase.h"
#include <graphcut.h>

class Segmentation: public SegmentationBase 
{
  Q_OBJECT

public:
	Segmentation(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~Segmentation();
	/*$PUBLIC_FUNCTIONS$*/
	void loadImages(std::string fcr_filename,std::string fcr_maskFilename);
		  
public slots:
	
protected:
	virtual void fileOpenImage();
	virtual void fileOpenMask();
	virtual void fileNewMask();

	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);
	
protected slots:
  /*$PROTECTED_SLOTS$*/
	void segment();

	void hueSpinBoxChanged(int value);
	void similarityWeightSpinBoxChanged(int value);
	void neighborhoodWeightSpinBoxChanged(int value);
	
private:
	
	virtual void wheelEvent ( QWheelEvent * e );
	
	void visualizeResults();
	
	unsigned int m_huePenalty;
	GraphCut::tPrecision m_similarityWeight;
	GraphCut::tPrecision m_neighborhoodWeight;	
	
	QImage m_origImage;
	QPixmap m_origPixmap;
	QImage m_trimapImage;
	QPixmap m_trimapPixmap;
	
	QImage m_segmentedImage;
	QPixmap m_segmentedPixmap;
	
	
	enum eShownPixmap{
		OrigPixmap,
		TrimapPixmap,
	};
	eShownPixmap m_shownOrigPixmap;
	
	GraphCut m_graphCut;
	
	bool m_backgroundDrawingEnabled;
	bool m_foregroundDrawingEnabled;
};

#endif

