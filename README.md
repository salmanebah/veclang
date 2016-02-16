# Veclang
A toy transpiler for vectorial drawing using the cairo library.
# Building
From the ```src``` directory:
```sh
make
```
# Usage
Assuming the file ```example.vl``` contains the file following:
```
scal x; 
scal y;
x := 100;
y := 100;
pict pict1;
path square;
path triangle;
square := (x, y)--(x + 100, y)--(x + 100, y + 100)--(x, y + 100)--cycle;
triangle := (100, 100)--(150, 50)--(200, 100);
pict1 := image {
     draw square;
     draw triangle;
};

draw pict1;
line_color 40 50 20;
line_width 15;
draw translate(pict1, (200,200));
draw rotate(pict1, (200, 200), 90);
```
It can be transpiled to ```C``` by using:

```./vlcc example.vl```

The transpilation will write the following ```C``` code to ```example.c```

```c
/* Automatically generated code */
/* PLEASE DON'T MODIFY THIS , UNLESS YOU KNOW WHAT YOU ARE DOING :) */

#include <stdlib.h>
#include  <cairo.h>
#include <cairo-pdf.h>

int main(int argc , char** argv )
{
	cairo_surface_t *surface;
	cairo_t *cr;
	cairo_surface_t* pdf_surface = cairo_pdf_surface_create("output.pdf" , 600 ,600);
	cr=cairo_create(pdf_surface);
	cairo_move_to(cr , 100 , 100);
	cairo_line_to(cr , 200 , 100);
	cairo_line_to(cr , 200 , 200);
	cairo_line_to(cr , 100 , 200);
	cairo_line_to(cr , 100 , 100);
	cairo_stroke(cr);
	cairo_move_to(cr , 100 , 100);
	cairo_line_to(cr , 150 , 50);
	cairo_line_to(cr , 200 , 100);
	cairo_stroke(cr);
	cairo_set_source_rgb (cr, 0.400000, 0.500000, 0.200000);
	cairo_set_line_width (cr, 15);
	cairo_move_to(cr , 300 , 300);
	cairo_line_to(cr , 400 , 300);
	cairo_line_to(cr , 400 , 400);
	cairo_line_to(cr , 300 , 400);
	cairo_line_to(cr , 300 , 300);
	cairo_stroke(cr);
	cairo_move_to(cr , 300 , 300);
	cairo_line_to(cr , 350 , 250);
	cairo_line_to(cr , 400 , 300);
	cairo_stroke(cr);
	cairo_move_to(cr , 301 , 101);
	cairo_line_to(cr , 300 , 201);
	cairo_line_to(cr , 200 , 200);
	cairo_line_to(cr , 201 , 101);
	cairo_line_to(cr , 301 , 101);
	cairo_stroke(cr);
	cairo_move_to(cr , 301 , 101);
	cairo_line_to(cr , 351 , 151);
	cairo_line_to(cr , 300 , 201);
	cairo_stroke(cr);
	cairo_destroy(cr);
	cairo_surface_destroy(pdf_surface);
	return EXIT_SUCCESS;
}
```
The file ```example.c``` can further be compiled to machine code:
```sh
gcc example.c `pkg-config --cflags --libs cairo` -o example
```
