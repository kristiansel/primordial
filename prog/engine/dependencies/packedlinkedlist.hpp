#ifndef CCPOOL_H
#define CCPOOL_H

#include <cstring>
#include <cstdlib>
#include <iostream>

template <class T>
struct DataFlag
{
    DataFlag() :
        active(false)
    {};

    T data;
    bool active;
};

template <class T>
class PackedFlagList
{
    public:
        PackedFlagList() :
            m_end(nullptr),
            m_maxSize(0),
            m_start(nullptr),
            m_links(nullptr),
            m_insert_stack(nullptr),
            m_stack_current(nullptr) {};

        ~PackedFlagList()
        {
            free(m_links);
            delete [] m_insert_stack;
        };

        void reserve(unsigned int maxSize)
        {
            free(m_links);
            delete[] m_insert_stack;

            // This is lightning fast
//            std::allocator<DataFlag<T>> al;
//            m_links = al.allocate(maxSize);

            // This is lightning fast
            m_links = (DataFlag<T>*)malloc(maxSize * sizeof(DataFlag<T>));

            // This is slow as *****
            //m_links = (DataFlag<T>*)(new unsigned char[maxSize * sizeof(DataFlag<T>)]);

            // This is slow as well.....
            //m_links = new DataFlag<T> [maxSize];


            m_insert_stack = new DataFlag<T>* [maxSize];

            m_maxSize = maxSize;

            m_start = &m_links[0];
            m_end = m_start;

            m_stack_current = &m_insert_stack[0];
            *m_stack_current = m_end;
        }

        void setSize(unsigned int size_in)
        {
            if (size_in <= m_maxSize)
            {
                m_end = m_start + size_in;
                m_insert_stack[0] = m_end;
                for_all_priv(
                                [] (DataFlag<T> &t) {t.active = true;}
                             );
            }
            else
            {
                std::cerr << "tried to resize bigger than max\n";
            }
        }

    private:
        template<class Func>
        void for_all_priv(Func func)
        {
            DataFlag<T>* it = &m_links[0];
            while (it<&m_links[m_maxSize])
            {
                func(*it);
                it++;
            }
        }

    public:


        template<class Func>
        void forall(Func func)
        {
            DataFlag<T>* it = m_start;
            while (it<m_end)
            {
                if (it->active)
                    func(it->data);

                it++;
            }
        }

        template<class Func>
        void forall_i(Func func)
        {
            DataFlag<T>* it = m_start;
            while (it<m_end)
            {
                unsigned int i = (unsigned int)(it-m_start);
                if (it->active)
                    func(it->data, i);

                it++;
            }
        }

        DataFlag<T>* begin() {return m_start;};
        DataFlag<T>* end() {return m_end;};


        void add(const T &t_in)
        {
            DataFlag<T>* to_add = *m_stack_current;

            // check if in bounds
            if (to_add >=m_start && to_add < m_start+m_maxSize)
            { // proceed
                to_add->data = t_in;

                // update m_stack_current
                if (to_add == m_end)
                { // just update the end
                    //std::cout << "adding to end\n";
                    m_end++;
                    m_insert_stack[0] = m_end;
                }
                else
                {
                    m_stack_current--;
                    //std::cout << "adding not to end\n";
                }

                to_add->active = true;
            }
            else
            {
                std::cerr << "attempted to add to a full container\n";
            }

        }

        unsigned int size()
        {
            return ((m_end-m_start)-(m_stack_current-&m_insert_stack[0]));
        }

        void add_blank(unsigned int num)
        {
            for (int i = 0; i<num; i++)
            {
                add(T());
            }
        }

        void erase(DataFlag<T>* dft_in)
        {
            erase(&(dft_in->data));
        }

        void erase(T* t_in)
        {
            DataFlag<T>* to_del = static_cast<DataFlag<T>*>(static_cast<void*>(t_in));

            // deactivate
            to_del->active = false;

            // destruct
            to_del->data.~T();

            // reduce end if applicable
            if (to_del == (m_end-1))
                m_end--;

            // add this slot to stack
            m_stack_current++;
            *m_stack_current = to_del;
        }

        T& operator [] (unsigned int i)
        {
            if (&m_links[i] < m_end && i >= 0)
                return m_links[i].data;
            else
            {
                std::cerr << "error: PackedFlagList accessing element out of bounds" << i << "/" << size() << "\n";

                return m_links[i].data;
            }
        };

    private:
        DataFlag<T>* m_end;
        unsigned int m_maxSize;
        DataFlag<T>* m_links;
        DataFlag<T>* m_start;

        DataFlag<T>** m_insert_stack;
        DataFlag<T>** m_stack_current;



};

#endif // CCPOOL_H
