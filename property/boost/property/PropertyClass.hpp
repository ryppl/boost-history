#pragma once
#include "Property.h"

class PropertyClass
{
  public:
  
	PropertyClass();
	virtual ~PropertyClass();
	
	private:
  
  int pi;
  
  public:
  
  int ReturnPi();
  void SetPiValue(int val);
  
  public:
  
  properties::PropertyMemberFunction<int,PropertyClass,&PropertyClass::ReturnPi,&PropertyClass::SetPiValue> IntMem;
  properties::PropertyReadMemberFunction<int,PropertyClass,&PropertyClass::ReturnPi> IntMemNoWrite;
	
};
