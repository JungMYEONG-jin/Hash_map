#include <stdexcept>
#include <algorithm>
#include <utility>


namespace Proc {



	template<typename T>
	size_t hash<T>::operator()(const T& key) const
	{
		const size_t bytes = sizeof(key);
		size_t sum = 0;
		for (size_t i = 0; i < bytes; ++i) {
			unsigned char b = *(reinterpret_cast<const unsigned char*>(&key) + i);
			sum += b;
		}
		return sum;
	}

	size_t hash<std::string>::operator()(const std::string& key) const
	{
		size_t sum = 0;
		for (auto c : key) {
			sum += static_cast<unsigned char>(c);
		}
		return sum;
	}





	template<typename Key, typename T ,typename KeyEqual, typename Hash>
	void swap(hash_map<Key, T, KeyEqual, Hash>& first, hash_map<Key, T, KeyEqual, Hash>& second) noexcept
	{
		first.swap(second);
	}

	template<typename Key, typename T, typename KeyEqual, typename Hash>
	hash_map<Key, T, KeyEqual, Hash>::hash_map(const KeyEqual& equal, size_t numBuckets, const Hash& hash)
		:mBuckets(numBuckets), mEqual(equal), mHash(hash)
	{
		if (numBuckets == 0) {
			throw std::invalid_argument("Number of buckets must be positive");
		}
	}


	template<typename Key, typename T, typename KeyEqual, typename Hash>
	hash_map<Key, T, KeyEqual, Hash>& hash_map<Key, T, KeyEqual, Hash>::operator=(const hash_map<Key, T, KeyEqual, Hash>& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}
		auto copy = rhs;
		swap(copy); //rhs랑 swap후
		return *this; //swpa해서 바뀐상태
	}

		

	template<typename Key, typename T, typename KeyEqual, typename Hash>
	hash_map<Key, T, KeyEqual, Hash>& hash_map<Key, T, KeyEqual, Hash>::operator=(hash_map<Key, T, KeyEqual, Hash>&& rhs) noexcept
	{

		swap(rhs);
		return *this;
	}


	template<typename Key, typename T, typename KeyEqual, typename Hash>
	std::pair<typename hash_map<Key ,T, KeyEqual, Hash>::ListType::iterator, size_t>
		hash_map<Key, T, KeyEqual, Hash>::findElement(const key_type& k)
	{
		size_t bucket = mHash(k) % mBuckets.size();

		auto iter = find_if(begin(mBuckets[bucket]), end(mBuckets[bucket]),
			[this, &k](const auto& element) {return mEqual(element.first, k); });

		return std::make_pair(iter, bucket);

	}

	template<typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::value_type* hash_map<Key, T, KeyEqual, Hash>::find(const key_type& k)
	{
		auto[it, bucket] = findElement(k);
		if (it == end(mBcukets[bucket])) {
			return nullptr;

		}
		// 원소 포인터 return 
		return &(*it);
	}


	template<typename Key, typename T, typename KeyEqual, typename Hash>
	const typename hash_map<Key, T, KeyEqual, Hash>::value_type* hash_map<Key, T, KeyEqual, Hash>::find(const key_type& k) const
	{
		return const_cast<hash_map<Key, T, KeyEqual, Hash>*>(this)->find(k);
	}




	template<typename Key, typename T, typename KeyEqual, typename Hash>
	T& hash_map<Key, T, KeyEqual, Hash>::operator[](const key_type& k)
	{
		auto[it, bucket] = findElement(k);
		if (it == end(mBuckets[bucket]))
		{
			mSzie++;
			mBuckets[bucket].push_back(std::make_pair(l, T()));
			return mBuckets[bucket].back().second;
		}
		else {
			return it->second;
		}
	}


	template<typename Key, typename T, typename KeyEqual, typename Hash>
	void hash_map<Key, T, KeyEqual, Hash>::insert(const value_type& x)
	{
		auto[it, bucket] = findElement(x.first);

		if (it != end(mBuckets[bucket])) {
			return; // 이미 존재하는 경우
		}
		else {
			mSize++;
			mBuckets[bucket].push_back(x);
		}


	}


	template<typename Key, typename T, typename KeyEqual, typename Hash>
	void hash_map<Key, T, KeyEqual, Hash>::erase(const key_type& k)
	{
		auto[it, bucket] = findElement(k);

		if (it != end(mBuckets[bucket]))
		{
			mBuckets[bucket].erase(it);
			mSize--;
		}
	}

	template<typename Key, typename T, typename KeyEqual, typename Hash>
	void hash_map<Key, T, KeyEqual, Hash>::clear() noexcept
	{
		for (auto & bucket : mBuckets) {
			bucket.clear();
		}
		mSize = 0;
	}

	template<typename Key, typename T, typename KeyEqual, typename Hash>
	void hash_map<Key, T, KeyEqual, Hash>::swap(hash_map<Key, T, KeyEqual, Hash>& other) noexcept
	{
		using std::swap;
		swap(mBuckets, other.mBuckets);
		swap(mSize, other.mSize);
		swap(mEqual, other.mEqual);
		swap(mHash, other.mHash);

	}





}