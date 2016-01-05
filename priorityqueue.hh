#include <map>
#include <exception>
#include <set>
#include <utility>
#include <memory>

using std::map;
using std::set;
using std::pair;
using std::shared_ptr;
using std::multiset;
using std::move;

template <typename K, typename V>
class PriorityQueue {

private:
    struct K_ptr_cmp {
        bool operator() (const shared_ptr<K>& lhs, const shared_ptr<K>& rhs) const
        {
            return *lhs < *rhs;
        }
    };

    struct V_ptr_cmp {
        bool operator() (const shared_ptr<V>& lhs, const shared_ptr<V>& rhs) const
        {
            return *lhs < *rhs;
        }
    };

    typedef map<shared_ptr<K>, multiset<shared_ptr<V>, V_ptr_cmp>, K_ptr_cmp> kvmap;
    typedef map<shared_ptr<V>, multiset<shared_ptr<K>, K_ptr_cmp>, V_ptr_cmp> vkmap;

    map<K, shared_ptr<K>> key_pointer_map;
    map<V, shared_ptr<V>> value_pointer_map;
    kvmap key_value_map;
    vkmap value_key_map;
    size_t qsize;

public:
    using size_type = size_t;
    using key_type = K;
    using value_type = V;

    PriorityQueue()
    {
        try {
            key_pointer_map = map<K, shared_ptr<K>>();
            value_pointer_map = map<V, shared_ptr<V>>();
            key_value_map = kvmap();
            value_key_map = vkmap();
            qsize = 0;
        }
        catch (...) {
            throw;
        }
    }

    PriorityQueue(const PriorityQueue<K, V>& queue)
    {
        try {
            key_pointer_map =
                    map<const K, shared_ptr<const K>>( queue.key_pointer_map );
            value_pointer_map =
                    map<V, shared_ptr<V>>( queue.value_pointer_map );
            key_value_map =
                    kvmap( queue.key_value_map );
            value_key_map =
                    vkmap( queue.value_key_map );
            qsize = queue.qsize;
        }
        catch (...) {
            throw;
        }
    }

    PriorityQueue(PriorityQueue<K, V>&& queue)
    {
        try {
            key_pointer_map =
                    map<const K, shared_ptr<const K>>( move(queue.key_pointer_map) );
            value_pointer_map =
                    map<V, shared_ptr<V>>( move(queue.value_pointer_map) );
            key_value_map =
                    kvmap( move(queue.key_value_map) );
            value_key_map =
                    vkmap( move(queue.value_key_map) );
            qsize = queue.qsize;
        }
        catch (...) {
            throw;
        }
    }

    friend void swap(PriorityQueue<K, V>& lhs, PriorityQueue<K, V>& rhs) noexcept
    {
        std::swap(lhs.key_pointer_map, rhs.key_pointer_map);
        std::swap(lhs.value_pointer_map, rhs.value_pointer_map);
        std::swap(lhs.key_value_map, rhs.key_value_map);
        std::swap(lhs.value_key_map, rhs.value_key_map);
    }

    PriorityQueue<K, V>& operator=(const PriorityQueue<K, V>& queue)
    {
        try {
            PriorityQueue<K, V> temp( queue );
            swap(*this, temp);
            return *this;
        }
        catch (...) {
            throw;
        }
    }

    PriorityQueue<K, V>& operator=(PriorityQueue<K, V>&& queue)
    {
        try {
            PriorityQueue<K, V> temp( move(queue) );
            swap(*this, temp);
            return *this;
        }
        catch (...) {
            throw;
        }
    }

    bool empty() const noexcept { return key_value_map.size() == 0; }

    size_type size() const { return qsize; }

    // TODO - Watch out for invariants!
    void insert(const K& key, const V& value)
    {
        try {
        }
        catch (...) {
            throw;
        }
    }

    const V& minValue() const noexcept
    {
        return value_key_map.begin()->first;
    }

    const V& maxValue() const noexcept
    {
        return value_key_map.rbegin()->first;
    }

    const K& minKey() const noexcept
    {
        return value_key_map.begin()->second;
    }
    const K& maxKey() const noexcept
    {
        return value_key_map.rbegin()->second;
    }

    // TODO: All stuff below + global operations
    // ==, !=, <, >, <=, >=
    void deleteMin();
    void deleteMax();
    void changeValue(const K& key, const V& value);
    void merge(PriorityQueue<K, V>& queue);
};
