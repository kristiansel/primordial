#include "prop.h"

Prop::Prop()
{
    //ctor
}

Prop::~Prop()
{
    //dtor
}

void Prop::attachMesh(weak_ptr<Mesh> mesh_ptr_in)
{
//    mesh_ptrs.push_back(mesh_ptr_in);
    render_batches.push_back( {mesh_ptr_in} );
}
