#include <map>
#include <exception>
#include <set>
#include <utility>
#include <memory>

using std::map;
using std::set;
using std::make_pair;
using std::make_shared;
using std::shared_ptr;
using std::multiset;
using std::move;


class PriorityQueueNotFoundException : public std::exception
{
public:
    const char* what() const noexcept
    {
        return "Element not found";
    }
};

class PriorityQueueEmptyException : public std::exception
{
public:
    const char* what() const noexcept
    {
        return "PriorityQueue is empty";
    }
};

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

    // TODO: jeszcze potrzeba  void swap(PriorityQueue<K,V>& queue);
    friend void swap(PriorityQueue<K, V>& lhs, PriorityQueue<K, V>& rhs) noexcept
    {
        std::swap(lhs.key_value_map, rhs.key_value_map);
        std::swap(lhs.value_key_map, rhs.value_key_map);
        std::swap(lhs.qsize, rhs.qsize);
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

    bool empty() const noexcept { return qsize == 0; }

    size_type size() const { return qsize; }

    // TODO - Watch out for invariants!
    void insert(const K& key, const V& value)
    {
        try {
            auto new_key = make_shared<K>(key);
            auto new_val = make_shared<V>(value);

            auto kv_elem = key_value_map.find(new_key);
            auto kv_key = kv_elem == key_value_map.end() ? make_shared<K>(key) : kv_elem->first;

            auto vk_elem = value_key_map.find(new_val);
            auto vk_key = vk_elem == value_key_map.end() ? make_shared<V>(value) : vk_elem->first;

            key_value_map[kv_key].insert(vk_key);
            value_key_map[vk_key].insert(kv_key);

            qsize++;
        }
        catch (...) {
            throw;
        }
    }

    const V& minValue() const
    {
        if (empty())
            throw PriorityQueueEmptyException();
        return *(value_key_map.begin()->first);
    }

    const V& maxValue() const
    {
        if (empty())
            throw PriorityQueueEmptyException();
        return *(value_key_map.rbegin()->first);
    }

    const K& minKey() const
    {
        if (empty())
            throw PriorityQueueEmptyException();
        return **(value_key_map.begin()->second.begin());
    }
    const K& maxKey() const
    {
        if (empty())
            throw PriorityQueueEmptyException();
        return **(value_key_map.rbegin()->second.rbegin());
    }


    void deleteMin()
    {
        if (!qsize) return;

        try {
            auto min_val_elem = value_key_map.begin(); // O(1)
            auto min_key_elem = key_value_map.find(*(min_val_elem->second.begin())); // O(log(size))

            min_key_elem->second.erase(min_key_elem->second.begin()); // O(1) (amortized)
            min_val_elem->second.erase(min_val_elem->second.begin()); // O(1) (amortized)

            qsize--;

            if (min_key_elem->second.empty()) // O(1)
                key_value_map.erase(min_key_elem); // O(1) (amortized)
            if (min_val_elem->second.empty()) // O(1)
                value_key_map.erase(min_val_elem); // O(1) (amortized)
        }
        catch (...) {
            throw;
        }
    }

    void deleteMax()
    {
        if (!qsize) return;

        try {
            auto max_val_elem = std::prev(value_key_map.end());
            auto max_key_elem = key_value_map.find(*(std::prev(max_val_elem->second.end())));

            max_key_elem->second.erase(std::prev(max_key_elem->second.end()));
            max_val_elem->second.erase(std::prev(max_val_elem->second.end()));

            qsize--;

            if (max_key_elem->second.empty())
                key_value_map.erase(max_key_elem);
            if (max_val_elem->second.empty())
                value_key_map.erase(max_val_elem);
            }
        catch (...) {
            throw;
        }
    }

    void changeValue(const K& key, const V& value)
    {
        try {
            auto key_elem = key_value_map.find(make_shared<K>(key));
            if (key_elem == key_value_map.end())
                throw PriorityQueueNotFoundException();

            // usunięcie pierwszej wartości w multisecie o kluczu key
            auto val = *(key_elem->second.begin());
            value_key_map[val].erase(key_elem->first);
            key_elem->second.erase(key_elem->second.begin());
            if (key_elem->second.empty())
                key_value_map.erase(key_elem);
            if (value_key_map[val].empty())
                value_key_map.erase(val);
            qsize--;

            // wstawienie nowej wartości
            // TODO: można szybciej, bo tak to wartość key jest wyszukiwana po raz drugi
            insert(key,value); // O(log(size))
        }
        catch (...) {
            throw;
        }
    }

    void merge(PriorityQueue<K, V>& queue)
    {
        try {
            while (!queue.empty()) { // O(queue.size)
                this->insert(queue.minKey(), queue.minValue()); // minKey, minValue - O(1), insert - O(log(size))
                queue.deleteMin(); // O(log(queue.size))
            }
            // Summary: O(queue.size * (O(log(size)) + O(loq(queue.size))))
        }
        catch (...) {
            throw;
        }
    }

    // TODO: 
    // ==, !=, <, >, <=, >=
};
