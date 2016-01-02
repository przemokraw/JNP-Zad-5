#include <map>
#include <exception>

/*
 * Co jest? Czego nie ma?
 *
 * - Trzeba pomyslec nad operator= (to chyba najbardziej trikowa czesc zadania)
 * PATRZ - http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
 *
 * - Insert pewnie mam zle.
 * - Generalnie przejrzyj ten kod dobrze bo ja nie jestem pewny moich umiejetnosci C++
 * - No i oczywiscie jak jest tylko deklaracja gdzies to tez tego jeszcze nie ma.
 *
 * Generalnie to opieram sie na gwarancjach stla, ze jesli wystapi wyjatek
 * to pozostawia obiekt bez zmiany, a wyjatki propaguje na zewnatrz za pomoca
 * catch (...) { throw; }
 *
 *
 */


template <typename K, typename V>
class PriorityQueue {

private:
    std::map<K, V> mData;

public:
    using size_type = decltype(mData.size());
    using key_type = K;
    using value_type = V;

    PriorityQueue()
    {
        try {
            // Map ctor has strong guarantee
            mData = std::map<K, V>();
        }
        catch (...) {
            throw;
        }
    }

    PriorityQueue(const PriorityQueue<K, V>& queue)
    {
        try {
            mData = std::map<K, V>(queue.mData);
        }
        catch (...) {
            throw;
        }
    }

    PriorityQueue(PriorityQueue<K, V>&& queue)
    {
        try {
            mData = std::map<K, V>( std::move(queue) );
        }
        catch (...) {
            throw;
        }
    }

    PriorityQueue<K, V>& operator=(PriorityQueue<K, V> queue)
    {
        try {
            std::map<K, V> temp = std::map<K, V>( queue.mData );
        }
        catch (...) {
            throw;
        }
    }

    bool empty() const { return mData.size() == 0; }

    size_type size() const { return mData.size(); }

    void insert(const K& key, const V& value)
    {
        try {
            // TODO: WATCHOUT for K, V throwing
            mData.insert( std::pair<K, V>(key, value) );
        }
        catch (...) {
            throw;
        }
    }

    const V& minValue() const
    {
        return std::map<K, V>::begin()->second;
    }

    const V& maxValue() const
    {
        return std::map<K, V>::rbegin()->second;
    }

    const K& minKey() const
    {
        return std::map<K, V>::begin()->first;
    }
    const K& maxKey() const
    {
        return std::map<K, V>::rbegin()->second;
    }

    void deleteMin();
    void deleteMax();
    void changeValue(const K& key, const V& value);
    void merge(PriorityQueue<K, V>& queue);
    void swap(PriorityQueue<K, V>& queue);

    // TODO: Global operations
    // swap, ==, !=, <, >, <=, >=
};
