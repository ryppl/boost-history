#include <iostream>
#include <qapplication.h>
#include <segmentation.h>

using namespace std;

int main(int argc, char* argv[]){
 	QApplication qapp(argc,argv);
	Segmentation *mw = new Segmentation();
	qapp.setMainWidget(mw);
	mw->show();
	if (argc < 3)
	{
		cout << "No images given on commandline. Using defaults " <<endl;
 		mw->loadImages("data/images/lena.png","data/trimaps/lena.png");
//       mw->loadImages("data/images/testImage50.png","data/trimaps/testImage50.png");      
	}
	else
	{
		mw->loadImages(argv[1],argv[2]);
	}
	return qapp.exec();
	return 0;
}
