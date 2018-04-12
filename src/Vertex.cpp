#include "Vertex.h"


std::ostream &operator<< (std::ostream &stream, const Vertex &vertex)
{
	stream
		<< vertex.position.x << " "
		<< vertex.position.y << " "
		<< vertex.position.z << " "
		<< vertex.position.w << ", "
		<< vertex.uv.s << " "
		<< vertex.uv.t << ", "
		<< vertex.color.r << " "
		<< vertex.color.g << " "
		<< vertex.color.b << " "
		<< vertex.color.a;
	return (stream);
}
