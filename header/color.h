#ifndef COLOR_H
#define COLOR_H

class Color {
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
	Color(void) : R(0), G(0), B(0), A(0) {}
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : R(r), G(g), B(b), A(a) {}
};

#endif
