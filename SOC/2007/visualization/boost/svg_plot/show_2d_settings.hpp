// show_2d_settings.cpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// -----------------------------------------------------------------

// This shows settings and options for 2-D Plot.

#ifndef BOOST_SVG_SHOW_2D_SETTINGS_HPP
#define BOOST_SVG_SHOW_2D_SETTINGS_HPP

#include <iostream> // for testing only.
// using std::cerr and std::endl

namespace boost
{
namespace svg
{

  // Declaration, defined below.
  void show_plot_settings(svg_2d_plot& plot);

const char* fmtFlagWords[16] =
{ // Descriptions of each bit.
	"skipws", "unitbuf", "uppercase","showbase","showpoint","showpos","left","right",
		"internal","dec","oct","hex","scientific","fixed","boolalpha", "?"
		/*
		enum _Fmtflags
		{ // Show action when bit set == 1
		skipws = 0x0001,  //   Skip white space on input.
		unitbuf = 0x0002, // Output each char - unbuffered.
		uppercase = 0x0004,// Display uppercase A through F for hexadecimal values and E for scientific values
		showbase = 0x0008, // Display numeric constants in a format that can be read by the C++ compiler.
		showpoint = 0x0010, // Show decimal point and trailing zeros for floating-point values.
		showpos = 0x0020, // Show plus signs (+) for positive values.
		left = 0x0040, // Left-align values; pad on the right with the fill character.
		right = 0x0080,// Right-align values; pad on the left with the fill character (default alignment).
		internal = 0x0100, // Add fill characters after any leading sign or base indication, but before the value.
		dec = 0x0200, // Format numeric values as base 10 (decimal) (default radix).
		oct = 0x0400, // Format numeric values as base 8 (octal).
		hex = 0x0800, // Format numeric values as base 16 (hexadecimal).
		scientific = 0x1000,// Display floating-point numbers in scientific format.
		fixed = 0x2000, // Display floating-point numbers in fixed format.
		boolalpha = 0x4000, // Show bool as word true or false.
		// 0x8000 un-used. // show as ?
		adjustfield = 0x01c0,
		basefield = 0x0e00,
		floatfield = 0x3000,
		_Fmtmask = 0x7fff,
		_Fmtzero = 0
		};
		*/
}; // const char* fmtFlagWords

void outFmtFlags(std::ios_base::fmtflags fmtFlags, std::ostream& os, const char* term)
{// Usage: 	outFmtFlags(flags, cerr);
	// For example, logs to cerr "FormatFlags: skipws showbase right dec"
	// Defaults in xiostream.hpp:
	// void outFmtFlags(fmtflags fmtFlags = cout.flags(), ostream& os = cerr, const char* term = ".\n");
	const int up = 16; // Words across page.
	const int count = 16;  // cos using unsigned short int.
	int const flags = os.flags(); // save to restore.
	fmtFlags &= 0x7FFF;  // _Fmtmask // clear un-used bits.
  os << "IOS format flags (" << std::showbase << std::hex << fmtFlags << std::dec << ")" ; // hex value.
	if (fmtFlags != 0)
	{
		for(int i = 0, j = 1, hit = 0; i < count; ++i)
		{
			if ((fmtFlags & j) != 0)
			{
				if (hit == 0)
				{ // First word no separator.
					os << ' ';
				}
				else
				{
					os << (( (hit % up) == 0) ? ",\n" : ", ");
				}
				os	<< fmtFlagWords[i];
				++hit; // Count to add space or newline as appropriate.
			}
			j <<= 1;
		}
	}
	os << term; // eg "\n" or ". "
	os.flags(flags);  // Restore.
}  // outFmtFlags

  namespace detail
  {

    //std::ostream& operator<< (std::ostream&, const std::pair<double, double>&);
    //template<class T1, class T2> std::ostream& operator<< (std::ostream&, std::pair<T1, T1>&);

    template<class T1, class T2>
    std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)
    { // Output a pair of values.
           os << p.first << ", " << p.second;
        // Outputs:  1.2, 3.4
        return os;
    } // std::ostream& operator<< 

    std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& p)
    { // Output a pair of values.
        int precision = os.precision(3); // Save & use rather than default precision(6)
        os << p.first << ", " << p.second;
        // Outputs:  1.2, 3.4
        os.precision(precision); // Restore.
        return os;
    } // std::ostream& operator<< 

    // Maybe better as:
    //template<typename charT, typename traits, typename T1, typename T2>
    //inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<T1, T2>& p)
    //{
    //  return os << p.first << " " << p.second;
    //}
    //
    //// Explicit double, double.
    //template<typename charT, typename traits>
    //inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<double, double>& p)
    //{
    //  return os << p.first << " " << p.second;
    //}
    // but OK for this purpose.
  } // namespace detail

  const std::string l_or_r(int i)
  {
   return ((i < 0) ? "left" : ((i == 0) ? "none" : "right"));
  }

  const std::string t_or_b(int i)
  {
   return ((i < 0) ? "bottom" : ((i == 0) ? "none" : "top"));
  }

void show_plot_settings(svg_2d_plot& plot)
{
  using std::cout;
  using std::endl;
  using std::hex;
  using std::dec;
  using std::boolalpha;
  using std::fixed;

  using boost::svg::detail::operator<<;
  // std::ostream& operator<< (std::ostream&, const std::pair<double, double>&);
  // defined above.

  int iostate = cout.flags(); // Save to restore one exit.
  cout << dec << std::boolalpha << endl;
  cout  << endl;
  //cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << "image_filename() " << plot.image.image_filename() << endl; 
  // doesn't work - no filename is shown.
  
  cout << "axes_on " << plot.axes_on() << endl;
  cout << "background_border_width " << plot.background_border_width() << endl;
  cout << "background_border_color " << plot.background_border_color() << endl;
  cout << "background_color " << plot.background_color() << endl;
  cout << "image_border_margin() " << plot.image_border_margin() << endl;
  cout << "image_border_width() " << plot.image_border_width() << endl;
  cout << "coord_precision " << plot.coord_precision() << endl;
  cout << "copyright_date  " << plot.copyright_date() << endl;
  cout << "copyright_holder " << plot.copyright_holder() << endl;
  cout << "description "<< plot.description() << endl;
  cout << "document_title " << plot.document_title() << endl;
  //cout << plot.draw_bezier_lines() << endl;
  cout << "image_x_size " << plot.image_x_size() << endl;
  cout << "image_y_size " << plot.image_y_size() << endl;
  cout << "legend_on " << plot.legend_on() << endl;
  std::pair<double, double> lt = plot.legend_top_left();
  std::pair<double, double> rb = plot.legend_bottom_right();
  cout << "legend_place " << plot.legend_place() << endl;
  cout << "legend_top_left " << lt << ", legend_bottom_right " << rb << endl;
  cout << "legend_background_color " << plot.legend_background_color() << endl;
  cout << "legend_border_color " << plot.legend_border_color() << endl;
  cout << "legend_color " << plot.legend_color() << endl;
  cout << "legend_title " << plot.legend_title() << endl;
  cout << "legend_title_font_size " << plot.legend_title_font_size() << endl;
  cout << "legend_font_weight " << plot.legend_font_weight() << endl;
  // Not useful at present, so not longer implemented.
  // cout << "legend_font_width " << plot.legend_font_width() << endl;
  cout << "legend_width " << plot.legend_width() << endl;
  cout << "legend_lines " << plot.legend_lines() << endl;
  cout << "license_on " << plot.license_on() << endl;
  cout << "license_reproduction " << plot.license_reproduction() << endl;
  cout << "license_distribution " << plot.license_distribution() << endl;
  cout << "license_attribution " << plot.license_attribution() << endl;
  cout << "license_commercialuse " << plot.license_commercialuse() << endl;
  cout << "plot_background_color " << plot.plot_background_color() << endl;
  cout << "plot_border_color " << plot.plot_border_color() << endl;
  cout << "plot_border_width " << plot.plot_border_width() << endl;
  cout << "plot_window_on " << plot.plot_window_on() << endl;
  cout << "plot_window_x " << plot.plot_window_x() << endl;
  cout << "plot_window_x_left " << plot.plot_window_x_left() << endl;
  cout << "plot_window_x_right " << plot.plot_window_x_right() << endl;
  cout << "plot_window_y " << plot.plot_window_y() << endl;
  cout << "plot_window_y_top " << plot.plot_window_y_top() << endl;
  cout << "plot_window_y_bottom " << plot.plot_window_y_bottom() << endl;
  cout << "title_on " << plot.title_on() << endl;
  cout << "title \"" << plot.title() << "\""<< endl;
  cout << "title_color " << plot.title_color() << endl;
  cout << "title_font_alignment " << plot.title_font_alignment() << endl;
  cout << "title_font_decoration " << plot.title_font_decoration() << endl;
  cout << "title_font_family " << plot.title_font_family() << endl;
  cout << "title_font_rotation " << plot.title_font_rotation() << endl;
  cout << "title_font_size " << plot.title_font_size() << endl;
  cout << "title_font_stretch " << plot.title_font_stretch() << endl;
  cout << "title_font_style " << plot.title_font_style() << endl;
  cout << "title_font_weight " << plot.title_font_weight() << endl;
  //cout << "title_font_width " << plot.title_font_width() << endl;
  // Not useful at present, so not longer implemented.
  cout << "x_value_precision " << plot.x_value_precision() << endl;
  cout << "x_value_ioflags " << hex << plot.x_value_ioflags() << dec << ' ';
  outFmtFlags(plot.x_value_ioflags(), cout,  ".\n");
  cout << "y_value_precision " << plot.y_value_precision() << endl;
  cout << "y_value_ioflags " << hex << plot.y_value_ioflags() << dec << ' ';
  outFmtFlags(plot.y_value_ioflags(), cout,  ".\n");

  cout << "x_max " << plot.x_max() << endl;
  cout << "x_min " << plot.x_min() << endl;
  cout << "x_autoscale " << plot.x_autoscale() << endl;
  cout << "y_autoscale " << plot.y_autoscale() << endl;
  cout << "xy_autoscale " << plot.xy_autoscale() << endl;
  cout << "x_autoscale_check_limits " << plot.autoscale_check_limits() << endl;

  cout << "x_axis_on " << plot.x_axis_on() << endl;
  cout << "x_axis_color() " << plot.x_axis_color() << endl;
  cout << "x_axis_label_color " << plot.x_axis_label_color() << endl;
  cout << "x_axis_value_color " << plot.x_axis_value_color() << endl;
  cout << "x_axis_width " << plot.x_axis_width() << endl;
  cout << "x_label_on " << plot.x_label_on() << endl;
  cout << "x_label " << plot.x_label() << endl;
  cout << "x_label_color " << plot.x_label_color() << endl;
  cout << "x_label_font_family " << plot.x_label_font_family() << endl;
  cout << "x_label_font_size " << plot.x_label_font_size() << endl;
  cout << "x_label_units " << plot.x_label_units() << endl;
  cout << "x_label_units_on " << plot.x_label_units_on() << endl;
  //cout << "x_label_width " << plot.x_label_width() << endl;
  // Not useful at present, so not longer implemented.
  cout << "x_major_value_labels_side " << l_or_r(plot.x_major_value_labels_side()) << endl;
  cout << "x_major_label_rotation " << plot.x_major_label_rotation() << endl;
  cout << "x_major_grid_color " << plot.x_major_grid_color() << endl;
  cout << "x_major_grid_on " << plot.x_major_grid_on() << endl;
  cout << "x_major_grid_width " << plot.x_major_grid_width() << endl;
  cout << "x_major_interval " << plot.x_major_interval() << endl;
  cout << "x_major_tick " << plot.x_major_tick() << endl;
  cout << "x_major_tick_color " << plot.x_major_tick_color() << endl;
  cout << "x_major_tick_length " << plot.x_major_tick_length() << endl;
  cout << "x_major_tick_width " << plot.x_major_tick_width() << endl;
  cout << "x_minor_interval " << plot.x_minor_interval() << endl;
  cout << "x_minor_tick_color " << plot.x_minor_tick_color() << endl;
  cout << "x_minor_tick_length " << plot.x_minor_tick_length() << endl;
  cout << "x_minor_tick_width " << plot.x_minor_tick_width() << endl;
  cout << "x_minor_grid_on " << plot.x_minor_grid_on() << endl;
  cout << "x_minor_grid_color " << plot.x_minor_grid_color()<< endl;
  cout << "x_minor_grid_width " << plot.x_minor_grid_width() << endl;
  cout << "x_range() " << plot.x_range() << endl;
  cout << "x_num_minor_ticks " << plot.x_num_minor_ticks() << endl;
  cout << "x_ticks_down_on " << plot.x_ticks_down_on() << endl;
  cout << "x_ticks_up_on " << plot.x_ticks_up_on() << endl;
  cout << "x_ticks_on_window_or_axis " << t_or_b(plot.x_ticks_on_window_or_axis()) << endl;
  cout << "y_axis_position " << plot.y_axis_position() << endl;
  cout << "x_axis_position " << plot.x_axis_position() << endl;
  cout << "y_label_on " << plot.y_label_on() << endl;
  cout << "y_label_axis " << plot.y_label_axis() << endl;
  cout << "y_axis_color " << plot.y_axis_color() << endl;
  cout << "y_axis_label_color " << plot.y_axis_label_color() << endl;
  cout << "y_axis_on " << plot.y_axis_on() << endl;
  cout << "axes_on " << plot.axes_on() << endl; // Both x and y axes on
  cout << "y_axis_value_color " << plot.y_axis_value_color() << endl;
  cout << "y_axis_width " << plot.y_axis_width() << endl;
  cout << "y_label " << plot.y_label() << endl;
  cout << "y_label_color " << plot.y_label_color() << endl;//
  cout << "y_label_font_family " << plot.y_label_font_family() << endl;
  cout << "y_label_font_size " << plot.y_label_font_size() << endl;
  cout << "y_label_on " << plot.y_label_on() << endl;
  cout << "y_label_units " << plot.y_label_units() << endl;
  cout << "y_label_units_on " << plot.y_label_units_on() << endl;
  cout << "y_label_width " << plot.y_label_width() << endl;
  cout << "y_major_grid_on "  << plot.y_major_grid_on() << endl;
  cout << "y_major_grid_color " << plot.y_major_grid_color() << endl;
  cout << "y_major_grid_width " << plot.y_major_grid_width() << endl;
  cout << "y_major_interval " << plot.y_major_interval() << endl;
  cout << "y_major_value_labels_side " << t_or_b(plot.y_major_value_labels_side()) << endl;
  cout << "y_major_label_rotation " << plot.y_major_label_rotation() << endl;
  cout << "y_major_tick_color " << plot.y_major_tick_color() << endl;
  cout << "y_major_tick_length  " << plot.y_major_tick_length() << endl;
  cout << "y_major_tick_width  " << plot.y_major_tick_width() << endl;
  cout << "y_minor_grid_on " << plot.y_minor_grid_on() << endl;
  cout << "y_minor_grid_color  " << plot.y_minor_grid_color() << endl;
  cout << "y_minor_grid_width " << plot.y_minor_grid_width() << endl;
  cout << "y_minor_interval " << plot.y_minor_interval() << endl;
  //cout << "y_minor_labels_on " << plot.y_minor_labels_on() << endl; // Not implemented yet.
  cout << "y_minor_tick_color " << plot.y_minor_tick_color() << endl;
  cout << "y_minor_tick_length " << plot.y_minor_tick_length() << endl;
  cout << "y_minor_tick_width " << plot.y_minor_tick_width() << endl;
  cout << "y_range() " << plot.y_range() << endl;
  cout << "y_num_minor_ticks " << endl;
  cout << "y_ticks_left_on " << plot.y_ticks_left_on() << endl;
  cout << "y_ticks_right_on " << plot.y_ticks_right_on() << endl;
  cout << "y_ticks_on_window_or_axis " << l_or_r(plot.y_ticks_on_window_or_axis()) << endl;
  cout << "y_max " << plot.y_max() << endl;
  cout << "y_min " << plot.y_min() << endl;
  cout << "data lines width " << plot.data_lines_width() << endl;


 cout.flags(iostate); // Restore.
} // void show_plot_settings(svg_2d_plot& plot)

} // svg
} // boost

#endif // BOOST_SVG_SHOW_2D_SETTINGS_HPP
