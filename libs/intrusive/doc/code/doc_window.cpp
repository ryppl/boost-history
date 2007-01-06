//[doc_window_code
#include <boost/intrusive/ilist.hpp>

//An abstract class that can be
//inserted in an intrusive list
class Window
   :  public boost::intrusive::ilist_base_hook<0, true>
{
   public:
   Window();
   virtual ~Window();
   virtual void Paint(){/**/}
};

//Some Window derived classes
class FrameWindow :  public Window
{/**/};

class EditWindow :  public Window
{/**/};

class CanvasWindow :  public Window
{/**/}; 

//This is a container of abstract values:
//you can't do this with std::list.
typedef boost::intrusive::ilist
   < Window::value_traits<Window> > win_list;

//A global intrusive list
win_list all_windows;

//Constructor. Includes this window in the list
Window::Window()
{  all_windows.push_back(*this);  }

//Destructor. Removes this node from the list
Window::~Window()
{  all_windows.erase(win_list::current(*this));  }

//A function that prints all windows stored
//in the intrusive list
void paint_all_windows()
{
   win_list::iterator i(all_windows.begin()), e(all_windows.end());
   for(/**/; i != e; ++i)
      i->Paint();
}

//...

//A class derived from Window
class MainWindow  :  public Window
{
   FrameWindow   frame_;  //these are derived from Window too
   EditWindow    edit_;
   CanvasWindow  canvas_;
   //...
};

//Main function
int main()
{
   //When each Window class is created, is
   //automatically registered in the global list
   MainWindow window;

   //Paint all the windows, sub-windows and so on
   paint_all_windows(); 

   //All the windows are automatically unregistered
   //in their destructors.
   return 0;
}
//]
