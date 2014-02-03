#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>

template <class ResType> class ResourceManager
{
public:
    struct ResCounter
    {
        ResCounter() : counter(0) {};
        explicit ResCounter(std::shared_ptr<ResType> res_ptr_in) : counter(0) { res_ptr = res_ptr_in; };
        std::shared_ptr<ResType> res_ptr;
        int counter;
    };

public:
    ResourceManager();
    virtual ~ResourceManager();

    std::weak_ptr<ResType> getResptrFromKey(  std::string res_key_in  );

protected:
private:
    std::unordered_map<std::string, ResCounter> resources;
};

/// SEE DEFINITIONS FURTHER DOWN
///                          |
///                          |
///                          |
///                          |
///                          |
///                          |
///                          |
///                          |
///                          |
///                          |
///                          |
///                          |
///                          |
///                          V




































/// DEFINITIONS

template <class ResType> ResourceManager<ResType>::ResourceManager()
{

}

template <class ResType> ResourceManager<ResType>::~ResourceManager()
{

}

template <class ResType> std::weak_ptr<ResType> ResourceManager<ResType>::getResptrFromKey(std::string res_key_in)      /// MESH
{

    std::shared_ptr<ResType> new_resource(new ResType);

    auto emplace_result = resources.emplace(res_key_in, ResCounter(new_resource));
    /// Type = pair<unordered_map<string, ResCounter<ResType>>::iterator, bool>

    auto pair_it = emplace_result.first; /// unordered_map<string, ResCounter<ResType>>::iterator

    bool inserted   = emplace_result.second;
    auto resource_counter = &(pair_it->second);
    auto resource_ptr   = resource_counter->res_ptr;

    if (inserted)
    {
        /// construct the resourc filepath from the key
        //string filepath = basefolder + "/" + modelfolder + "/" + res_key_in + ".obj";

        resource_ptr->fromFile(res_key_in);

//        cout << "inserted for " << resourc_key_in << "\n";
    }

    /// Important: Count another reference to the resource
    resource_counter->counter++;

//    cout << "resourc count for " << resourc_key_in << ": " << resourc_counter->counter << "\n";

    return std::weak_ptr<ResType>(resource_ptr);
}




#endif // RESOURCEMANAGER_H

