#include <cairo.h>

int
main (void) {
    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 200, 200);
    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, 200, 200);
    cairo_fill(cr);

    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 190, 100);
    cairo_line_to(cr, 100, 185);
    cairo_line_to(cr, 200, 200);
    cairo_line_to(cr, 30, 130);
    cairo_close_path(cr);
    cairo_set_source_rgb(cr, 0.8, 0.4, 1);
    cairo_fill(cr);

    cairo_move_to(cr, 180, 30);
    cairo_line_to(cr, 100, 20);
    cairo_line_to(cr, 80, 120);
    cairo_set_source_rgb(cr, 0.5, 0.7, 0.3);
    cairo_fill_preserve(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 3);
    cairo_stroke(cr);

    cairo_destroy(cr);
    cairo_surface_write_to_png(surface, "simple-example.png");

    cairo_surface_destroy(surface);
    return 0;
}

/* vi:set ts=4 sw=4 expandtab: */
