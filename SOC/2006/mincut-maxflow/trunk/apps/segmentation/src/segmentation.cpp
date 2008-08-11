//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Stephan Diederich <S.Diederich@gmx.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

//locals
#include "segmentation.h"

#include <iostream>
#include <string>

//qt
#include <qwidget.h>
#include <qlabel.h>

#include <qtooltip.h>
#include <qpainter.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <q3filedialog.h>
#include <q3textedit.h>
#include <qtabwidget.h>
#include <qmenubar.h>
#include <qcombobox.h>
#include <qdatetime.h> 
#include <qevent.h>
//Added by qt3to4:
#include <QWheelEvent>
#include <QMouseEvent>
#include "ui_segmentationbase.h"
using namespace std;
// using namespace boost;

Segmentation::Segmentation(QWidget* parent, const char* name, Qt::WFlags fl)
  : Q3MainWindow(), mp_ui(new Ui::SegmentationBase), m_huePenalty(15),m_similarityWeight(5),m_neighborhoodWeight(500),m_backgroundDrawingEnabled(false),m_foregroundDrawingEnabled(false)
{
	mp_ui->setupUi(this);

	//connect the menubar
	connect(mp_ui->fileOpenImageAction,SIGNAL(activated()),this,SLOT(fileOpenImage()));
	connect(mp_ui->fileOpenMaskAction,SIGNAL(activated()),this,SLOT(fileOpenMask()));
	connect(mp_ui->fileNewMaskAction,SIGNAL(activated()),this,SLOT(fileNewMask()));
	connect(mp_ui->fileSaveAsActionss,SIGNAL(activated()),this,SLOT(fileSaveAs()));
	connect(mp_ui->optionsStore_problem_as_DIMACSAction,SIGNAL(toggled(bool)),this,SLOT(storeProblemAsDimacsFile(bool)));

	
	connect(mp_ui->mp_buttonSegment,SIGNAL(clicked()),this,SLOT(segment()));
	mp_ui->mp_spinHuePenalty->setValue(m_huePenalty);
	connect(mp_ui->mp_spinHuePenalty,SIGNAL(valueChanged(int)),this,SLOT(hueSpinBoxChanged(int)));
	
	mp_ui->mp_spinNeighborhoodWeight->setValue((int)m_neighborhoodWeight);
	connect(mp_ui->mp_spinNeighborhoodWeight,SIGNAL(valueChanged(int)),this,SLOT(neighborhoodWeightSpinBoxChanged(int)));
	
	mp_ui->mp_spinSimilarityWeight->setValue((int)m_similarityWeight);
	connect(mp_ui->mp_spinSimilarityWeight,SIGNAL(valueChanged(int)),this,SLOT(similarityWeightSpinBoxChanged(int)));
	
	//init the 2 image-showing qlabes with empty pixmaps
	mp_ui->mp_labelOrig->setPixmap(m_origPixmap);
	mp_ui->mp_labelSegmented->setPixmap(m_segmentedPixmap);
	
	update();
}

Segmentation::~Segmentation()
{

}

void Segmentation::fileOpenImage()
{
    QString s = Q3FileDialog::getOpenFileName(
		    "data/images",
                    "Images (*.png)",
                    this,
                    "open file dialog",
                    "Choose a file to open" );
	if(s.isNull())
		return;
	else
	{
		m_origImage.load(s);
		m_origPixmap.convertFromImage(m_origImage);
		mp_ui->mp_labelOrig->setPixmap(m_origPixmap);
		m_shownOrigPixmap=OrigPixmap;
	}
}

void Segmentation::fileOpenMask()
{
    QString s = Q3FileDialog::getOpenFileName(
		    "data/trimaps",
                    "Images (*.png)",
                    this,
                    "open file dialog",
                    "Choose a file to open" );
	if(s.isNull())
		return;
	else{
		m_trimapImage.load(s);
		m_trimapPixmap.convertFromImage(m_trimapImage);
		mp_ui->mp_labelOrig->setPixmap(m_trimapPixmap);
		m_shownOrigPixmap=TrimapPixmap;
	}
}

void Segmentation::fileSaveAs()
{
  if(!m_segmentedImage.isNull()){  
    QString s = Q3FileDialog::getSaveFileName (
        "data/segmented",
    "Images (*.png)",
    this,
    "save file dialog",
    "Where to save?" );
    if(s.isNull())
      return;
    else{
      if(s.right(4).lower().compare(".png")){ 
        //there is no png extension, so add one
        s.append(".png");
      }
      if(!m_segmentedImage.save(s,"PNG")){
        mp_ui->mp_logEdit->append(QString("ERROR saving segmented image to ")+s);		
      }
      else{
        mp_ui->mp_logEdit->append(QString("Saved segmented image to ")+s);		
      }
    }
  }
  else
    mp_ui->mp_logEdit->append(QString("ERROR. No segmented image to save yet!"));
}

void Segmentation::loadImages( std::string fcr_imageFileName, std::string fcr_maskFileName )
{
	mp_ui->mp_logEdit->append("Loading images...");
	
	if(!m_origImage.load(QString::fromStdString(fcr_imageFileName)))
		cout << "Could not load image from " << fcr_imageFileName <<endl;
	m_origPixmap.convertFromImage(m_origImage);
	
	if(!m_trimapImage.load(QString::fromStdString(fcr_maskFileName)))
		cout << "Could not load trimap from " << fcr_imageFileName <<endl;
	m_trimapPixmap.convertFromImage(m_trimapImage); 	
	
	mp_ui->mp_labelOrig->setPixmap(m_origPixmap);
	m_shownOrigPixmap=OrigPixmap;
	
	update();
	mp_ui->mp_logEdit->append("...done");
}


void Segmentation::wheelEvent( QWheelEvent *  )
{
	if(mp_ui->mp_labelOrig->hasMouse())
	{
		if(m_shownOrigPixmap==OrigPixmap){
			mp_ui->mp_labelOrig->setPixmap(m_trimapPixmap);
			m_shownOrigPixmap=TrimapPixmap;
		}
		else{
			mp_ui->mp_labelOrig->setPixmap(m_origPixmap);
			m_shownOrigPixmap=OrigPixmap;
		}
	}	
	update();
}



void Segmentation::segment()
{
	
  	m_graphCut.setSimilarityWeight(m_similarityWeight);
	m_graphCut.setNeighborhoodWeight(m_neighborhoodWeight);
	m_graphCut.setNeighborhoodThreshold(m_huePenalty);
 	mp_ui->mp_logEdit->append("Starting segmentation...");
	QTime time;
	time.start();
	m_segmentedImage=m_graphCut.segment(m_origImage,m_trimapImage,static_cast<GraphCut::eMode>(mp_ui->mp_comboAlgo->currentItem()));
	int neededTime=time.elapsed();
	m_segmentedPixmap.convertFromImage(m_segmentedImage);
	mp_ui->mp_labelSegmented->setPixmap(m_segmentedPixmap);
	mp_ui->mp_logEdit->append(QString("Done! Segmentation lasted: %1 ms (maxflow:%2)").arg(neededTime).arg(m_graphCut.maxflowTime()));		

// 	visualizeResults();
}

void Segmentation::visualizeResults()
{
/*	
	if(m_origImage.getColorFormat()==vip::GRAY_8_BYTE)
	{
		BYTE* pDataSeg(m_segmentedImage.getrImageBuffer().pData());
		const BYTE* pDataOrig(m_origImage.getcrImageBuffer().pConstData());
		for(unsigned int y(m_segmentedImage.getcrImageSize().getHeight());y>0;--y)
		{
			for(unsigned int x(m_segmentedImage.getcrImageSize().getWidth());x>0;--x)
			{
				if(*pDataSeg == 255)
				{
					*pDataSeg=*pDataOrig;
				}
				++pDataSeg;
				++pDataOrig;
			}
		}	
	}
	else 
	{
		VipImage temp(m_origImage);
		temp.separate();
		const BYTE* pDataSeg(m_segmentedImage.getcrImageBuffer().pConstData());
		BYTE* pDataNew(temp.getrImageBuffer().pData());
		const unsigned int DataNewSkip(temp.getSkip());
		const unsigned int DataSegSkip(m_segmentedImage.getSkip());
			
		vector<int> segmentationHisto(256);
		double numberOfPixels(0);
			
		for(int y(0);y<m_segmentedImage.getcrImageSize().getHeight();++y)
		{
			for(int x(0);x<m_segmentedImage.getcrImageSize().getWidth();++x)
			{
				if(*pDataSeg++ != 255)
				{
					*pDataNew++=0;*pDataNew++=0;*pDataNew++=0;*pDataNew++=0;
				}
					//TODO maybe use alpha channel of old image
				else
				{
					++segmentationHisto[m_origImageHLS.getcrImageBuffer()[m_origImageHLS.getPitch()*y+x*m_origImageHLS.getcrFormat().getPixelDepth()]];
					++numberOfPixels;
					pDataNew+=4; //have argb
	// 					*pDataNew++=255;*pDataNew++=255;*pDataNew++=255;*pDataNew++=255;
				}
			}
			pDataNew+=DataNewSkip;
			pDataSeg+=DataSegSkip;
		}
		m_segmentedImage=temp;
		
		if(m_useRGBGraphCut)
		{
			std::vector< VmVector3<double> > vecs;
			for(unsigned int r(0);r<256;++r)
				for(unsigned int g(0);g<256;++g)
					for(unsigned int b(0);b<256;++b)
					{
						if((static_cast<tRGBGraphCut*>(mp_graphCut))->getrSimilarityModel(tRGBGraphCut::SimilarityModelFG).getProbability(r,g,b)>0.00000001)
						{
							vecs.push_back(VmVector3<double>(r/255.,g/255.,b/255.));

						}
					}
					mp_glViewer->getrObjectList().add(new VgfxPointCloud3d(vecs));
		}
		else //no gray image, use alpha channel
		{
			tHueGraphCut* lpCut(static_cast<tHueGraphCut*>(mp_graphCut));
			{
				QPixmap pixmap(QSize(256,mp_labelHistoFGSegmented->size().height()));//mp_labelHistoFG->size());
				double histoHeight (pixmap.size().height());
				QPainter painter(&pixmap);		
				painter.fillRect(pixmap.rect(), black);
				for(unsigned int i(0);i<256;++i)
				{
					painter.setPen ( QColor(static_cast<int>(i/255.*359.),255,255,QColor::Hsv));
					painter.drawLine(i,static_cast<int>(histoHeight),i,
							static_cast<int>(histoHeight-segmentationHisto[i]/numberOfPixels*histoHeight)
							);
				}	
				painter.end();
				mp_labelHistoFGSegmented->setPixmap(pixmap);
			}
			//draw the foreground and background similarity model histograms					
			{
				QPixmap pixmap(QSize(256,mp_labelHistoFG->size().height()));//mp_labelHistoFG->size());
				int histoHeight (pixmap.size().height());
				QPainter painter(&pixmap);		
				painter.fillRect(pixmap.rect(), black);
				for(unsigned int i(0);i<256;++i)
				{
					painter.setPen ( QColor(static_cast<int>(i/255.*360.),255,255,QColor::Hsv));
					painter.drawLine(i,static_cast<int>(histoHeight),i,
							static_cast<int>(histoHeight-				lpCut->getrSimilarityModel(tHueGraphCut::SimilarityModelFG).m_values[i]* histoHeight)
							);
				}	
				painter.end();
				mp_labelHistoFG->setPixmap(pixmap);
			}
			
			{
				QPixmap pixmap(QSize(256,mp_labelHistoBG->size().height()));
				double histoHeight (pixmap.size().height());
				QPainter painter(&pixmap);		
				painter.fillRect(pixmap.rect(), black);
				for(unsigned int i(0);i<256;++i)
				{
					painter.setPen ( QColor(static_cast<int>(i/255.*360.),255,255,QColor::Hsv));				
				
					painter.drawLine(i,static_cast<int>(histoHeight),i,
							static_cast<int>(histoHeight-
									lpCut->getrSimilarityModel(tHueGraphCut::SimilarityModelBG).m_values[i] * histoHeight)
							);
				}	
				painter.end();
				mp_labelHistoBG->setPixmap(pixmap);		
			}
		}
	}
	m_segmentedPixmap.setBaseImage(m_segmentedImage);
	m_segmentedPixmap.doResize(mp_labelSegmented->size());
	mp_labelSegmented->setPixmap(m_segmentedPixmap);*/
}

void Segmentation::hueSpinBoxChanged( int value )
{
	m_huePenalty=value;
}

void Segmentation::similarityWeightSpinBoxChanged( int value )
{
	m_similarityWeight=value;
}
void Segmentation::neighborhoodWeightSpinBoxChanged( int value )
{
	m_neighborhoodWeight=value;
}

void Segmentation::fileNewMask( )
{
	std::cout << "hello" <<std::endl;
}

void Segmentation::mousePressEvent(QMouseEvent* e){
// 	if(!mp_labelOrig->hasMouse())
// 		return;
// 	if(e->button()==Qt::LeftButton)
// 		m_foregroundDrawingEnabled=true;
// 	if(e->button()==Qt::RightButton)
// 		m_backgroundDrawingEnabled=true;	
}

void Segmentation::mouseReleaseEvent(QMouseEvent* e){
// 	if(e->button()==Qt::LeftButton)
// 		m_foregroundDrawingEnabled=false;
// 	if(e->button()==Qt::RightButton)
// 		m_backgroundDrawingEnabled=false;		
}

void Segmentation::mouseMoveEvent(QMouseEvent* e){
// // 	if(!mp_labelOrig->hasMouse())
// // 		return;
// 
// 	
// 	QPoint mouse_pos_relative_to_origlabel=e->pos() - (mp_labelOrig->geometry().topLeft()+QPoint(0,menubar->height())); //this is shit. how do i get the relative pos?!
// 	cout << mouse_pos_relative_to_origlabel.x() << " " << mouse_pos_relative_to_origlabel.y() <<endl;
// 	//as the pixmap is maybe resized, we need some conversion factor
// 	double resized_to_normal_x=m_origImage.width()/(double)mp_labelOrig->width();
// 	double resized_to_normal_y=m_origImage.height()/(double)mp_labelOrig->height();
// 	QPoint imageCoordinates=QPoint(
// 		static_cast<int>(resized_to_normal_x*mouse_pos_relative_to_origlabel.x()),
// 		sstatic_cast<int>(resized_to_normal_y*mouse_pos_relative_to_origlabel.y()));
// 	if(imageCoordinates.x()>=m_origImage.width() ||imageCoordinates.x()<0)
// 		return;
// 	if(imageCoordinates.y()>=m_origImage.heigth() || imageCoordinates.y()<0)
// 		return;
// 	//ok, fine, start recording pixels
// 	if(m_backgroundDrawingEnabled){
// 		m_trimapImage.pixel
// 	}
// 	else if(m_foregroundDrawingEnabled){
// 	}
// 	
}

void Segmentation::storeProblemAsDimacsFile( bool toggled )
{
  if (toggled){
    m_graphCut.setDumpProblemAsDimacsFile( true, "image.dat");
  }else{
    m_graphCut.setDumpProblemAsDimacsFile( false,"");
  }
}


