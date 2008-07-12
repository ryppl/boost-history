#ifndef INC_GLV_UTIL_H
#define INC_GLV_UTIL_H

/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

namespace glv {

/// Smart pointer functionality to avoid deleting references.

///	When a class contains pointers to other objects, sometimes it is not clear
///	if it is responsible for freeing the memory associated with those objects.
class SmartPointer{
public:

	/// @param[in] del	whether the object is deleted with smartDelete()
	SmartPointer(bool del=true): mDeletable(del){}

	/// Deletes object only if it is deletable, i.e. not a reference.
	void smartDelete(){
		//printf("%p %c\n", this, mDeletable ? 'y' : 'n');
		if(true == mDeletable) delete this;
	}
	
	/// Set whether the object can be deleted.
	void deletable(bool v){ mDeletable = v; }
	
	/// Returns whether the object can be deleted.
	bool deletable(){ return mDeletable; }
	
//	void operator delete (void * p){
//		
//		SmartPointer * pc = static_cast<SmartPointer *>(p);
//		if(pc && pc->mIsDeletable){
//			free(pc);
//			printf("freed\n");
//		}
//		else printf("denied\n");
//	}
	
private:
	bool mDeletable;
};

}	// end namespace glv

#endif

