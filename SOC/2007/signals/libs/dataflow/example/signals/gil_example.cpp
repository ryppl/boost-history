// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ gil_example_preliminary

#include <boost/dataflow/signals/component/timed_generator.hpp>
#include <boost/dataflow/signals/component/multiplexer.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

using namespace boost;

//]


//[ gil_example_image_generator

class image_generator : public signals::filter<image_generator, void(const gil::gray8_image_t &)>
{
public:
    // constructed with the width, height, and initial grayscale value of the image
    image_generator(unsigned width, unsigned height, int value)
        : image(width, height), value(value) {}
    
    // Upon receiving a void() signal, build and output the image
    void operator()()
    {
        boost::gil::fill_pixels(view(image), value);
        out(image);
    }
    // Upon receiving a void(int) signal, set the grayscale value to what's received
    void operator()(int new_value)
    {
        value = new_value;
    }
private:
    gil::gray8_image_t image;
    int value;
};

//]

//[ gil_example_noise_adder

class noise_adder : public signals::filter<noise_adder, void(const gil::gray8_image_t &)>
{
public:
    noise_adder() : dist(0, 10), generator(mt, dist)
    {}
    // Upon receiving an image, generate the filtered version and output it.
    void operator()(const gil::gray8_image_t &image)
    {
        if (image.dimensions() != filtered_image.dimensions())
            filtered_image.recreate(image.dimensions());
        boost::gil::transform_pixels(
            const_view(image),
            view(filtered_image),
            transformer(generator));
        out(filtered_image);
    }
private:
    gil::gray8_image_t filtered_image;

    mt19937 mt;
    normal_distribution<> dist;
    variate_generator<mt19937&, boost::normal_distribution<> > generator;

    struct transformer
    {
        transformer(variate_generator<mt19937&, boost::normal_distribution<> > &generator)
            : generator(generator)
        {}
        
        gil::gray8_pixel_t operator()(const gil::gray8_pixel_t& p) const
        {
            return gil::gray8_pixel_t(p + gil::bits8(generator()));
        }
        
        variate_generator<mt19937&, boost::normal_distribution<> > &generator;
    };
};

//]

//[ gil_example_image_display

class image_display : public signals::consumer<image_display>
{
public:
    // Upon receiving an image, cout an ASCII approximation.
    void operator()(const gil::gray8_image_t &image)
    {
        gil::gray8_image_t::const_view_t view = const_view(image);
        for (int y=0; y<view.height(); y++)
        {
            gil::gray8_image_t::const_view_t::x_iterator it = view.row_begin(y);
            
            for (int x=0; x<view.width(); x++)
                std::cout << symbols[it[x] * (sizeof(symbols) / sizeof(char)) / 256];
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
private:
    static char symbols[8];
};

char image_display::symbols[8] = {' ', '.', ',', '-', '+', 'x', 'X', '#'};

//]

//[ gil_example_controller

class controller
{
public:
    controller(int value) : value(value) {}
    
    void run()
    {
        while (!std::cin.eof() && !std::cin.fail())
        {
            int key = std::cin.get();
            switch (key)
            {
            // Keys ',' and '.' adjust the grayscale value
            case ',':
                adjust(-32);
                break;
            case '.':
                adjust(32);
                break;
            // Keys '0' and '1' control the mux output
            case '0':
                select_signal(0);
                break;
            case '1':
                select_signal(1);
                break;
            // 'q' quits.
            case 'q':
                return;
            }
        }
    }

    boost::signal<void(int)> value_signal;
    boost::signal<void(int)> select_signal;
private:
    void adjust(int delta)
    {
        value += delta;
        if (value < 0) value = 0;
        if (value > 255) value = 255;
        value_signal(value);
    }
    int value;
};

//]

//[ gil_example_main

int main(int, char* [])
{
    // To drive the dataflow network, we will use timed_generator,
    // which creates its own thread and outputs a signal
    // at a specified time interval.  We'll store a value of 0 to be sent out.
    // The signature void() specifies that the signal is empty - we just
    // use it to drive the network.
    signals::timed_generator<void ()> timer;

    // Data processor and output:
    image_generator generator(10,10, 32);
    noise_adder filter;
    image_display display;

    controller control(32);
    signals::multiplexer<void (const gil::gray8_image_t &)> mux;

    // ---Connect the dataflow network -----------------------------------------
    //
    //                  ,---------.
    //                  | control | -----------------------+
    //                  `---------'                        |
    //                       |                             |
    //                       v                             v
    //   ,-------.     ,-----------.                   ,-------.
    //   | timer | --> | generator | -+--------------> 0       |    ,---------.  
    //   `-------'     `-----------'  |                |  mux  | -> | display |
    //                                |  ,--------.    |       |    `---------'
    //                                +->| filter |--> 1       |
    //                                   `--------'    `-------'
    //
    // -------------------------------------------------------------------------
    timer >>= generator | mux.slot<0>()
                        | (filter >>= mux.slot<1>());
    
    mux >>= display;
    
    control.value_signal >>= generator;
    control.select_signal >>= mux.select_slot();

    // Tell the timer to start producing signals, every 1.0s:
    timer.enable(1.0);

    // Run the controller
    std::cout
        << "Use keys ',' and '.' to control the image intensity, " << std::endl
        << "and '0' and '1' to switch the display between the unfiltered" << std::endl
        << "and filtered image. 'q' will exit." << std::endl << std::endl
        << "Since we are using cin for the input, REMEMBER TO PRESS ENTER" << std::endl
        << "after each command." << std::endl << std::endl;
    control.run();

    timer.join();
    return 0;
}

//]
