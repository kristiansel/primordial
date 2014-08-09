#include "terrain.h"

Terrain::Terrain() :
    m_dimension(256),
    m_height_map(256)
{
    //ctor
}

Terrain::~Terrain()
{
    //dtor
}

void Terrain::init()
{
    std::cout << "initializing terrain...\n";

    generateHeightMap();

    float patch_length = 32.0; // LOD1 16, LOD2 8, LOD3 4

    for (int i = 0; i<3; i++)
    {
        for (int j = 0; j<3; j++)
        {
            float center_x = -1*patch_length + j*patch_length;
            float center_z = -1*patch_length + i*patch_length;

            Vertex* verts;
            Triangle* tris;
            int num_verts;
            int num_tris;

            std::cout << "num_verts: " << num_verts << "\n";
            std::cout << "num_tris: " << num_tris << "\n";

            subdividedQuads(verts, num_verts, tris, num_tris, patch_length/2, 3, center_x, center_z);

             // assing the quad to the mesh;
            terrain_meshes.push_back(std::shared_ptr<Mesh> (new Mesh));

            std::shared_ptr<Mesh> terrain_quad = terrain_meshes.back();
            terrain_quad->fromMemory(verts, num_verts, tris, num_tris);

            delete [] verts;
            delete [] tris;

            // declare our patch as a prop
            std::shared_ptr<Prop> terrain_patch = std::shared_ptr<Prop>(new Prop);

            terrain_patch->pos = glm::vec3(center_x, 0.0, center_z);

            // get the placeholder terrain texture
            //std::weak_ptr<Mesh>      mesh_ptr    = global::mesh_manager.getResptrFromKey ("simple_plane");
            std::weak_ptr<Mesh>      mesh_ptr    = std::weak_ptr<Mesh>(terrain_quad);
            std::weak_ptr<Texture>   tex_ptr     = global::tex_manager.getResptrFromKey  ("grass_equal");

            // attach the generated geometry to the patch-prop
            terrain_patch->attachBatch(mesh_ptr, tex_ptr);

            // add the terrain patch to the vector of terrain patches
            terrain_patches.push_back(terrain_patch);
        }
    }

    std::cout << "length of terrain_patches: " << terrain_patches.size() << "\n";

    std::cout << "SAMPLE" << sampleHeightMap(0.3, 0.3, 32, 0, 0) << "\n";
}

std::vector<std::shared_ptr<Prop>>* Terrain::getPatches()
{
    return &terrain_patches;
}

void Terrain::subdividedQuads(Vertex* &vertices,
                              int &num_verts,
                              Triangle* &triangles,
                              int &num_tris,
                              float half_length,
                              int n_sub,
                              float x_corner,
                              float z_corner)
{

    int n_side = pow(2, n_sub);
    int n_quads = n_side*n_side;

    int verts_per_quad = 4;
    int tris_per_quad = 2;

    num_verts = verts_per_quad * n_quads;
    num_tris = tris_per_quad * n_quads;

    vertices = new Vertex [num_verts];
    triangles = new Triangle [num_tris];

    float quad_length = 2*half_length/n_side;

    for (int i = 0; i<n_side; i++)
    {
        for (int j = 0; j<n_side; j++)
        {
            //     1-------2    A
            //     | _0___/|    | -z
            //     |/   1  |    |
            //     0-------3    y----> X
            int first_vert_index = verts_per_quad*(i*n_side + j);
            int first_tri_index = tris_per_quad*(i*n_side + j);

            Vertex* verts = &vertices[first_vert_index];
            Triangle* tris = &triangles[first_tri_index];

            float x = -half_length + j*quad_length;
            float z = -half_length + i*quad_length;

            float center_x = 0;
            float center_z = 0;
            float map_l = 128.0;

            float sample0 = 0.002*sampleHeightMap(x+x_corner, z+quad_length+z_corner, map_l, center_x, center_z);
            float sample1 = 0.002*sampleHeightMap(x+x_corner, z+z_corner, map_l, center_x, center_z);
            float sample2 = 0.002*sampleHeightMap(x+quad_length+x_corner, z+z_corner, map_l, center_x, center_z);
            float sample3 = 0.002*sampleHeightMap(x+quad_length+x_corner, z+quad_length+z_corner, map_l, center_x, center_z);

            std::cout << "samples " << sample0 << ", " << sample1 << ", " << sample2 << ", " << sample3 << "\n";

            verts[0].position = glm::vec4(x, sample0, z+quad_length, 1.0);
            verts[1].position = glm::vec4(x, sample1, z, 1.0);
            verts[2].position = glm::vec4(x+quad_length, sample2, z, 1.0);
            verts[3].position = glm::vec4(x+quad_length, sample3, z+quad_length, 1.0);

            verts[0].normal = glm::vec3(0.0, 1.0, 0.0);
            verts[1].normal = glm::vec3(0.0, 1.0, 0.0);
            verts[2].normal = glm::vec3(0.0, 1.0, 0.0);
            verts[3].normal = glm::vec3(0.0, 1.0, 0.0);

            verts[0].tex_coords[0] = 0.0; verts[0].tex_coords[1] = 0.0;
            verts[1].tex_coords[0] = 0.0; verts[1].tex_coords[1] = 1.0;
            verts[2].tex_coords[0] = 1.0; verts[2].tex_coords[1] = 1.0;
            verts[3].tex_coords[0] = 1.0; verts[3].tex_coords[1] = 0.0;

            for (int i = 0; i<4; i++)
            {
                for (int n_bon = 0; n_bon<MAX_BONE_INFLUENCES; n_bon++)
                {
                    verts[i].bone_indices[n_bon] = 0;
                    if (n_bon == 0)
                    {
                        verts[i].bone_weights[n_bon] = 1;
                    }
                    else
                    {
                        verts[i].bone_weights[n_bon] = 0;
                    }

                }
            }


            tris[0].indices[0] = first_vert_index + 0;
            tris[0].indices[1] = first_vert_index + 2;
            tris[0].indices[2] = first_vert_index + 1;

            tris[1].indices[0] = first_vert_index + 0;
            tris[1].indices[1] = first_vert_index + 3;
            tris[1].indices[2] = first_vert_index + 2;
        }
    }

    // finished
}

//int Terrain::lin_i(int i, int j)
//{
//    return i*map_dim+j;
//};

double rand_range(double a, double b)
{
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

void Terrain::generateHeightMap()
{
    double range = 8000.0; // m

    // find the first (middle point) m_dimension/2
    m_height_map(m_dimension/2, m_dimension/2) = rand_range(-range, range);

//        std::cout << "first point: " << m_height_map(m_dimension/2, m_dimension/2) << "\n";

    // find the first corner
    m_height_map(0,0) = m_height_map(m_dimension/2, m_dimension/2) + rand_range(-range/2.0, range/2.0);

//        std::cout << "corner point: " << m_height_map(m_dimension, m_dimension) << "\n";
//        std::cout << "corner point: " << m_height_map(0, 0) << "\n";

    // find the first edge points
    m_height_map(m_dimension/2, 0) = m_height_map(0, 0) + rand_range(-range/2.0, range/2.0);
    m_height_map(0, m_dimension/2) = m_height_map(0, 0) + rand_range(-range/2.0, range/2.0);

//        std::cout << "edge point: " << m_height_map(m_dimension, m_dimension) << "\n";

    unsigned int max_size = m_dimension+1;
    unsigned int depth = 0;
    unsigned int length_scale = (max_size-1)/pow(2, depth);

    while (length_scale > 2) // each layer
    {
//            std::cout << "level===================================\n";

        length_scale = (max_size-1)/pow(2, depth);
//            std::cout << "length_scale = " << length_scale << "\n";
        unsigned int num_quads = (max_size-1)/length_scale;
//            std::cout << "num_quads = " << num_quads << "\n";

        int scale_factor = pow(2, depth);

        for (int j = 0; j<num_quads; j++)
        {
//                std::cout << "yrow========================\n";
            // update y
            unsigned int low_y = j*length_scale;
            unsigned int high_y = (j+1)*length_scale;
            for (int i = 0; i<num_quads; i++)
            {
//                    std::cout << "xrow============\n";
                unsigned int low_x = i*length_scale;
                unsigned int high_x = (i+1)*length_scale;

                // fill in edges
//                    if (length_scale==2 && low_x == 32)
//                    {
//                        std::cout << "low_x = " << low_x << "\n";
//                        std::cout << "high_x = " << high_x << "\n";
//                        std::cout << "low_y = " << low_y << "\n";
//                        std::cout << "high_y = " << high_y << "\n";
//                    }


                m_height_map((low_x+high_x)/2, low_y) = (m_height_map(low_x, low_y)+m_height_map(high_x, low_y))/2.0 +
                                             rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                m_height_map((low_x+high_x)/2, high_y) = (m_height_map(low_x, high_y)+m_height_map(high_x, high_y))/2.0 +
                                             rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                m_height_map(low_x, (low_y+high_y)/2) = (m_height_map(low_x, low_y)+m_height_map(low_x, high_y))/2.0 +
                                             rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                m_height_map(high_x, (low_y+high_y)/2) = (m_height_map(high_x, low_y)+m_height_map(high_x, high_y))/2.0 +
                                             rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);

                // middle
//                    m_height_map((low_x+high_x)/2, (low_y+high_y)/2) = (m_height_map(low_x, low_y)+m_height_map(low_x, high_y)+m_height_map(high_x, low_y)+m_height_map(high_x, high_y))/4.0 +
//                                                 rand_range(-range/2.0/scale_factor/2.0, 0.0+range/2.0/scale_factor/2.0);

                // Need to change this to make smoother middle (take all 9 surrounding into account with different weights)
                m_height_map((low_x+high_x)/2, (low_y+high_y)/2) = 0.414213562*(m_height_map(low_x, low_y)+m_height_map(low_x, high_y)+m_height_map(high_x, low_y)+m_height_map(high_x, high_y))/4.0
                                             + 0.585786438*(m_height_map((low_x+high_x)/2, low_y) + m_height_map((low_x+high_x)/2, high_y) + m_height_map(low_x, (low_y+high_y)/2) + m_height_map(high_x, (low_y+high_y)/2))/4.0
                                             + rand_range(-range/2.0/scale_factor/2.0, 0.0+range/2.0/scale_factor/2.0);
            }

        }
    depth++;
    }

    // adjust water lvl
    float av_height;
    float num_cells = m_dimension*m_dimension;

    for (int i = 0; i<m_dimension; i++)
    {
        for (int j = 0; j<m_dimension; j++)
        {
            av_height+=m_height_map(i,j)/num_cells;
        }
    }

    for (int i = 0; i<m_dimension; i++)
    {
        for (int j = 0; j<m_dimension; j++)
        {
            m_height_map(i,j)=m_height_map(i,j)-av_height;
        }
    }


    // debug height
    unsigned int x_side = 79 < m_dimension? 79 : m_dimension;
    unsigned int y_side = 46 < m_dimension? 46 : m_dimension;

//        std::cout << "seed = " << m_seed << ":\n";
    for (int j = 0; j<y_side; j++)
    {
        for (int i = 0; i<x_side; i++)
        {
            unsigned int x_ind = int((float)(i)/(float)(x_side)*m_dimension);
            unsigned int y_ind = int((float)(j)/(float)(y_side)*m_dimension);

            float height = m_height_map(y_ind, x_ind);
            if (height < 0.0)
            {
                std::cout << "-";
//                std::printf("\x1b[34m"     "-"     "\x1b[0m" "\n");
            }
            else if (height < 500.0)
            {
                std::cout << "=";
            }
            else if (height < 1500.0)
            {
                std::cout << "o";
            }
            else if (height < 3000.0)
            {
                std::cout << "U";
            }
            else
            {
                std::cout << "A";
            }
        }
        std::cout << "\n";
    } // finished printing debug map
}

float Terrain::sampleHeightMap(float x, float z, float length, float center_x, float center_z)
{
    // find the x and z of the top left corner
    float top_left_x = center_x-length/2.0;
    float top_left_z = center_z-length/2.0;

    float rel_x = x-top_left_x;
    float rel_z = z-top_left_z;

    float frac_x = rel_x/length;
    float frac_z = rel_z/length;

    float dec_ind_x = frac_x*m_dimension;
    float dec_ind_z = frac_z*m_dimension;

    int upper_x = ceil(dec_ind_x);
    int upper_z = ceil(dec_ind_z);
    int lower_x = floor(dec_ind_x);
    int lower_z = floor(dec_ind_z);

    if (upper_x == lower_x) upper_x++;
    if (upper_z == lower_z) upper_z++;

    float wt11 = (upper_x-dec_ind_x)*(upper_z-dec_ind_z)/((upper_x-lower_x)*(upper_z-lower_z));
    float wt12 = (dec_ind_x-lower_x)*(upper_z-dec_ind_z)/((upper_x-lower_x)*(upper_z-lower_z));
    float wt21 = (upper_x-dec_ind_x)*(dec_ind_z-lower_z)/((upper_x-lower_x)*(upper_z-lower_z));
    float wt22 = (dec_ind_x-lower_x)*(dec_ind_z-lower_z)/((upper_x-lower_x)*(upper_z-lower_z));

    std::cout << "======================================================\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!d " << dec_ind_x << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!u " << upper_x << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << lower_x << "\n";

    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!d " << wt11 << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!u " << wt12 << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << wt21 << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << wt22 << "\n";

    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!d " << m_height_map(lower_x, lower_z) << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!u " << m_height_map(lower_x, upper_z) << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << m_height_map(upper_x, lower_z) << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << m_height_map(upper_x, upper_z) << "\n";
    float out = wt11*m_height_map(lower_x, lower_z) +
                wt12*m_height_map(lower_x, upper_z) +
                wt21*m_height_map(upper_x, lower_z) +
                wt22*m_height_map(upper_x, upper_z);

    return out;
}



