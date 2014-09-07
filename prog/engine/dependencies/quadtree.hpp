#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <algorithm>
#include <glm/glm.hpp>

struct QuadAABB
{
    //QuadAABB(float x_min, float x_max, float z_min, float z_max)
    float x_min, x_max, z_min, z_max;

    static bool overlapping(const QuadAABB &aabb_1, const QuadAABB &aabb_2)
    {
        return !((aabb_1.x_min>=aabb_2.x_max || aabb_2.x_min>=aabb_1.x_max) ||
                 (aabb_1.z_min>=aabb_2.z_max || aabb_2.z_min>=aabb_1.z_max));
    }

    bool intersectsAABB(const QuadAABB &aabb_2)
    {
        return overlapping(*this, aabb_2);
    }

    bool containsPoint(float x, float z)
    {
        return (x<=x_max && x>=x_min && z>=z_min && z<=z_max);
    }

    bool operator + (const QuadAABB &aabb_2)
    {
        return overlapping(*this, aabb_2);
    }

    float x_len() {return x_max - x_min;}
    float z_len() {return z_max - z_min;}
};

struct QuadFrustum
{
    glm::vec4 p0;
    glm::vec4 p1;
    glm::vec4 p2;
    glm::vec4 p3;

    bool containsPoint(float x, float z)
    {
        bool test01 = ((x - p0.x) * (p1.z - p0.z) - (p1.x - p0.x) * (z - p0.z)) >= 0;
        bool test12 = ((x - p1.x) * (p2.z - p1.z) - (p2.x - p1.x) * (z - p1.z)) >= 0;
        bool test23 = ((x - p2.x) * (p3.z - p2.z) - (p3.x - p2.x) * (z - p2.z)) >= 0;
        bool test30 = ((x - p3.x) * (p0.z - p3.z) - (p0.x - p3.x) * (z - p3.z)) >= 0;


        return (test01 == test12 && test12 == test23 && test23 == test30);
    }

    bool intersectsAABB(QuadAABB aabb)
    {
        return (containsPoint(aabb.x_min, aabb.z_min) || containsPoint(aabb.x_min, aabb.z_max) ||
                containsPoint(aabb.x_max, aabb.z_min) || containsPoint(aabb.x_max, aabb.z_max)) ||
                (aabb.containsPoint(p0.x, p0.z) || aabb.containsPoint(p1.x, p1.z) ||
                 aabb.containsPoint(p2.x, p2.z) || aabb.containsPoint(p3.x, p3.z));
    }

};

template <class XZPosType, unsigned int max_contained> // must support .x, and .z members
class QuadTree // non-owning but mutating data structure
{
    public:
        QuadTree(QuadAABB boundary_in) : children({nullptr, nullptr, nullptr, nullptr}),
                     boundary({0, 0, 0, 0})
        {
            boundary = boundary_in;
            contained.reserve(max_contained);
        };

        virtual ~QuadTree()
        {
            delete children.nw; delete children.ne; delete children.sw; delete children.se;
        };

        bool insert(XZPosType in)
        {
        // Ignore objects that do not belong in this quad tree
            if (!boundary.containsPoint(in.x, in.z))
              return false; // object cannot be added

            // If there is space in this quad tree, add the object here
            if (contained.size() < max_contained)
            {
              contained.push_back(in);
              return true;
            }

            // Otherwise, subdivide and then add the point to whichever node will accept it
            if (!children.nw) // it is not initialized
              subdivide();

            if (children.nw->insert(in)) return true;
            if (children.ne->insert(in)) return true;
            if (children.sw->insert(in)) return true;
            if (children.se->insert(in)) return true;

            std::cout << "aabb " << boundary.x_min << ", " << boundary.x_max << ", " << boundary.z_min << ", " << boundary.z_max << "\n";
            return false;
        }

        void subdivide()
        {
            //std::cout << "subdividing " << boundary.x_min << ", " << boundary.x_max << ", " << boundary.z_min << ", " << boundary.z_max << "\n";
            //  x------------>
            // z  NW  |  NE  |
            // |      |      |
            // |______|______|
            // |  SW  |  SE  |
            // |      |      |
            // |______|______|
            // V
            children.nw = new QuadTree(QuadAABB({boundary.x_min, boundary.x_min+0.5*boundary.x_len(),
                                                 boundary.z_min, boundary.z_min+0.5*boundary.z_len()}));

            children.ne = new QuadTree(QuadAABB({boundary.x_min+0.5*boundary.x_len(), boundary.x_max,
                                                 boundary.z_min, boundary.z_min+0.5*boundary.z_len()}));

            children.sw = new QuadTree(QuadAABB({boundary.x_min, boundary.x_min+0.5*boundary.x_len(),
                                                 boundary.z_min+0.5*boundary.z_len(), boundary.z_max}));

            children.se = new QuadTree(QuadAABB({boundary.x_min+0.5*boundary.x_len(), boundary.x_max,
                                                 boundary.z_min+0.5*boundary.z_len(), boundary.z_max}));

            // spread this node's contained among its children
//            for (auto it : contained)
//            {
//                insert(it);
//            }
//
//            contained.clear();
        }

        std::vector<XZPosType> queryRange(QuadAABB range)
        {
            // Prepare an array of results
            std::vector<XZPosType> pointsInRange; // how big will this be?

            // Automatically abort if the range does not intersect this quad
            if (!boundary.intersectsAABB(range))
              return pointsInRange; // empty list

            // Check objects at this quad level
            for (auto point : contained)
            {
              if (range.containsPoint(point.x, point.z))
                 pointsInRange.push_back(point);
            }

            // Terminate here, if there are no children
            if (!children.nw)
              return pointsInRange;

            // Otherwise, add the points from the children
            std::vector<XZPosType> nw_result = children.nw->queryRange(range);
            pointsInRange.insert(pointsInRange.end(), nw_result.begin(), nw_result.end());

            std::vector<XZPosType> ne_result = children.ne->queryRange(range);
            pointsInRange.insert(pointsInRange.end(), ne_result.begin(), ne_result.end());

            std::vector<XZPosType> sw_result = children.sw->queryRange(range);
            pointsInRange.insert(pointsInRange.end(), sw_result.begin(), sw_result.end());

            std::vector<XZPosType> se_result = children.se->queryRange(range);
            pointsInRange.insert(pointsInRange.end(), se_result.begin(), se_result.end());

            return pointsInRange;
        }

        template <typename ShapeType, typename Func>
        void for_all_in(ShapeType range, Func do_this)
        {
            // Automatically abort if the range does not intersect this quad
            if (!range.intersectsAABB(boundary))
              return; // do nothing

            // Check objects at this quad level
            for (auto point : contained)
            {
              if (range.containsPoint(point.x, point.z))
                 do_this(point);
            }

            // Terminate here, if there are no children
            if (!children.nw)
              return;

            // Otherwise, add the points from the children
            children.nw->for_all_in(range, do_this);

            children.ne->for_all_in(range, do_this);

            children.sw->for_all_in(range, do_this);

            children.se->for_all_in(range, do_this);

            //return pointsInRange; // implicit
        }

        template <typename Condition>
        bool erase_if(Condition is_true)
        {
            // Check objects at this quad level
            contained.erase(std::remove_if(contained.begin(), contained.end(),
                                           is_true), // erase if this function returns true
                                           contained.end());

            // Terminate here, if there are no children
            if (!children.nw)
            {
                return (!contained.empty());
            }


            // Otherwise, add the points from the children
            bool nw_empty = children.nw->erase_if(is_true);

            bool ne_empty = children.ne->erase_if(is_true);

            bool sw_empty = children.sw->erase_if(is_true);

            bool se_empty = children.se->erase_if(is_true);

            // if they are all emtpy then delete them
            if (nw_empty && ne_empty && sw_empty && se_empty)
            {
                delete children.nw; delete children.ne; delete children.sw; delete children.se;
                children.nw = nullptr;
                children.ne = nullptr;
                children.sw = nullptr;
                children.se = nullptr;

                return (!contained.empty());
            }
            else
            {
                return false;
            }
        }

        template <typename ShapeType>
        bool erase_range(ShapeType range)
        {
            if (!range.intersectsAABB(boundary))
              return false; // do nothing, assume not empty

            // Check objects at this quad level
            contained.erase(std::remove_if(contained.begin(), contained.end(),
                                           [&] (XZPosType &in) {return range.containsPoint(in.x, in.z); }), // erase if this function returns true
                                           contained.end());

            // Terminate here, if there are no children
            if (!children.nw)
            {
                return (!contained.empty());
            }


            // Otherwise, add the points from the children
            bool nw_empty = children.nw->erase_range(range);

            bool ne_empty = children.ne->erase_range(range);

            bool sw_empty = children.sw->erase_range(range);

            bool se_empty = children.se->erase_range(range);

            // if they are all emtpy then delete them
            if (nw_empty && ne_empty && sw_empty && se_empty)
            {
                delete children.nw; delete children.ne; delete children.sw; delete children.se;
                children.nw = nullptr;
                children.ne = nullptr;
                children.sw = nullptr;
                children.se = nullptr;

                return (!contained.empty());
            }
            else
            {
                return false;
            }
        }

    protected:
    private:
        // data


        struct Children
        {
            QuadTree* nw;
            QuadTree* ne; // clockwise
            QuadTree* sw;
            QuadTree* se;
        } children;


        QuadAABB boundary; // x_min, x_max, z_min, z_max;

        std::vector<XZPosType> contained;
};

#endif // QUADTREE_H
