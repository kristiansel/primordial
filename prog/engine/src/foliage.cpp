#include "foliage.h"

Foliage::Foliage(Terrain* in_terrain)
{
    //ctor
    terrain = in_terrain;
}

Foliage::~Foliage()
{
    //dtor
}


void Foliage::addSmallVisuals(string mesh_key,
                             string tex_key,
                             glm::vec4 wind_params,
                             glm::vec3 center,
                             float radius,
                             float density) // #/m²
{
    float r_sqrd = radius*radius;

    //calculate area
    float area = 3.141592 * r_sqrd; // m²

    int num = density*area;

    std::cout << "number of small visuals: " << num << "\n";

    small_visuals.push_back(SmallVisual());
    SmallVisual &small_visual = small_visuals.back();

    small_visual.init();

    small_visual.mesh = std::shared_ptr<Mesh>(global::mesh_manager.getResptrFromKey(mesh_key));
    small_visual.tex = std::shared_ptr<Texture>(global::tex_manager.getResptrFromKey(tex_key));
    small_visual.wind_params = wind_params;


    for (int i = 0; i<num;)
    {
        float x = center.x + (float)(rand()%10000)/(float)(10000)*radius*2-radius;
        float z = center.z + (float)(rand()%10000)/(float)(10000)*radius*2-radius;

        if (x*x + z*z > r_sqrd)
        {
            // discard
        }
        else
        {
            if (i<SmallVisual::MAX_NUM_SMVIS)
            {
                small_visual.sm_buffer[i] = glm::vec4(x, terrain->ySample(x, z), z, 3.141592*(float)(rand()%360)/(float)(180.0));
            }
            //small_visual.positions.push_back(glm::vec4(x, terrain->ySample(x, z), z, 3.141592*(float)(rand()%360)/(float)(180.0)));
            i++;
        }
    }

    small_visual.num_smvis = num;

    small_visual.updated = false;

}

std::vector<SmallVisual> const * const Foliage::getVisuals() const
{
    return &small_visuals;
}

