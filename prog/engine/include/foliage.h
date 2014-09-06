#ifndef FOLIAGE_H
#define FOLIAGE_H

#include "smallvisual.h"
#include "global.h"
#include "terrain.h"

class Foliage
{
    public:
        Foliage(Terrain* in_terrain);
        virtual ~Foliage();

        std::vector<SmallVisual> const * const getVisuals() const;

        void addSmallVisuals(string mesh_key,
                             string tex_key,
                             glm::vec4 wind_params,
                             glm::vec3 center,
                             float radius,
                             float density);

    protected:
    private:
        std::vector<SmallVisual> small_visuals;

        Terrain* terrain;
};

#endif // FOLIAGE_H
