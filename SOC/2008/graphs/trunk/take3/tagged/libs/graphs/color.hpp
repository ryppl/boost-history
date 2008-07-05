
#if BOOST_HAS_CONCEPTS
/**
 * The Color concept defines the requirements for colors in this library.
 */
concept Color<typename C>
{
    // The color type must provide an underlying color representation that is
    // a regular type. This is the return type of all color functions.
    Regular color_type;

    // Color functions.
    color_type white();
    color_type gray();
    color_type black();
    color_type red();
    color_type green();
    color_type blue();
};

/**
 * Adapt the color enum to the Color concept.
 */
concept_map<color>
{
    typedef color color_type;
    color white()   { return white_color; }
    color gray()    { return gray_color; }
    color black()   { return black_color; }
    color red()     { return red_color; }
    color green()   { return green_color; }
    color blue()    { return blue_color; }
};
#endif