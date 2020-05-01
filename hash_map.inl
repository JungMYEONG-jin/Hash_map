#include <stdexcept>
#include <algorithm>
#include <utility>


namespace Proc {



	template<typename T>
	size_t hash<T>::operator()(const T& key) const
	{
		size_t bytes = sizeof(key);
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


	template<typename HashMap>
	const_hash_map_iterator<HashMap>::const_hash_map_iterator(size_t bucket, list_iterator_type listit, const HashMap* hashmap)
		:mBucketindex(bucket), mListiterator(listit), mHashmap(hashmap)
	{

	}

	template<typename HashMap>
	const typename const_hash_map_iterator<HashMap>::value_type&
		const_hash_map_iterator<HashMap>::operator*() const
	{
		return *mListiterator;
	}


	template<typename HashMap>
	const typename const_hash_map_iterator<HashMap>::value_type*
		const_hash_map_iterator<HashMap>::operator->() const
	{
		return &(*mListiterator);
	}


	template<typename HashMap>
	const_hash_map_iterator<HashMap>& const_hash_map_iterator<HashMap>::operator++()
	{
		increment();
		return *this;
	}

	template<typename HashMap>
	const_hash_map_iterator<HashMap> const_hash_map_iterator<HashMap>::operator++(int)
	{
		auto old = *this;
		increment();
		return old;
	}



	template<typename HashMap>
	const_hash_map_iterator<HashMap>& const_hash_map_iterator<HashMap>::operator--()
	{
		decrement();
		return *this;
	}

	template<typename HashMap>
	const_hash_map_iterator<HashMap> const_hash_map_iterator<HashMap>::operator--(int)
	{
		auto old = *this;
		decrement();
		return old;
	}


	template<typename HashMap>
	bool const_hash_map_iterator<HashMap>::operator==(const const_hash_map_iterator<HashMap>& rhs) const
	{
		return (mHashmap == rhs.mHashmap && mBucketindex == rhs.mBucketindex && mListiterator == rhs.mListiterator);
	}


	template<typename HashMap>
	bool const_hash_map_iterator<HashMap>::operator!=(const const_hash_map_iterator<HashMap>& rhs) const
	{
		return !(*this == rhs);
	}



	template<typename HashMap>
	void const_hash_map_iterator<HashMap>::increment()
	{
		++mListiterator;

		auto& buckets = mHashmap->mBuckets;
		if (mListiterator == end(buckets[mBucketindex])) {
			for (size_t i = mBucketindex + 1; i < buckets.size(); i++) {
				if (!buckets[i].empty()) {
					mListiterator = begin(buckets[i]);
					mBucketindex = i;
					return;
				}
			}
			mBucketindex = buckets.size() - 1;
			mListiterator = end(buckets[mBucketindex]);
		}
	}



	template<typename HashMap>
	void const_hash_map_iterator<HashMap>::decrement()
	{

		// mlistiter는 한 버킷에 대한 반복자다
		// 현재 버킷의 시작 지점을 가리키고 있다면 감소시키지 않고
		// 현재 지점보다 앞에 있는 버킷 중에서 비어 있지 않느 것을 찾는다.

		auto& buckets = mHashmap->mBuckets;
		if (mListiterator == begin(buckets[mBucketindex])) {
			for (int i = mBucketindex - 1; i >= 0; --i) {
				if (!buckets[i].empty()) {
					mListiterator = --end(buckets[i]);
					mBucketindex = i;
					return;
				}
			}
			mBucketindex = buckets.size() - 1;
			mListiterator = end(buckets[mBucketindex]);
		}
		else {
			// 버킷의 시작 지점에 아직 도달하지 않았으므로 한 칸 앞으로 이동한다.
			--mListiterator;
		}

	}



	template<typename HashMap>
	hash_map_iterator<HashMap>::hash_map_iterator(size_t bucket, list_iterator_type listit, HashMap* hashmap)
		: const_hash_map_iterator<HashMap>(bucket, listit, hashmap)
	{
	}


	template<typename HashMap>
	typename hash_map_iterator<HashMap>::value_type&
		hash_map_iterator<HashMap>::operator*()
	{
		return const_cast<value_type&>(*this->mListiterator);
	}
	//실제 원소에 대한 포인터를 return 그래서 컴파일러는 원하는 필드에 접근할 때 ->사용
	template<typename HashMap>
	typename hash_map_iterator<HashMap>::value_type*
		hash_map_iterator<HashMap>::operator->()
	{
		return const_cast<value_type*>(&(*this->mListiterator));
	}

	template<typename HashMap>
	hash_map_iterator<HashMap>& hash_map_iterator<HashMap>::operator++()
	{
		this->increment();
		return *this;
	}

	template<typename HashMap>
	hash_map_iterator<HashMap> hash_map_iterator<HashMap>::operator++(int)
	{
		auto old = *this;
		this->increment();
		return old;
	}


	template<typename HashMap>
	hash_map_iterator<HashMap>& hash_map_iterator<HashMap>::operator--()
	{
		this->decrement();
		return *this;
	}
	template<typename HashMap>
	hash_map_iterator<HashMap> hash_map_iterator<HashMap>::operator--(int)
	{
		auto old = *this;
		this->decrement();
		return old;
	}



	template<typename Key, typename T, typename KeyEqual, typename Hash>
	void swap(hash_map<Key, T, KeyEqual, Hash>& first, hash_map<Key, T, KeyEqual, Hash>& second) noexcept
	{
		first.swap(second);
	}


	template <typename Key, typename T, typename KeyEqual, typename Hash>
	hash_map<Key, T, KeyEqual, Hash>::hash_map(const KeyEqual& equal, size_type numBuckets, const Hash& hash)
		:mBuckets(numBuckets), mEqual(equal), mHash(hash)
	{
		if (numBuckets == 0) {
			throw std::invalid_argument("Number must be positive!");
		}
	}

	//원소를 실제로 추가하도록 insert 호출
	template <typename Key, typename T, typename KeyEqual, typename Hash>
	template<typename Inputiterator>
	hash_map<Key, T, KeyEqual, Hash>::hash_map(Inputiterator first, Inputiterator last, const KeyEqual& equal, size_type numBuckets, const Hash& hash)
		:hash_map(equal, numBuckets, hash)
	{
		insert(first, last);
	}

	//이니셜라이저 리스트 생성자
	template <typename Key, typename T, typename KeyEqual, typename Hash>
	hash_map<Key, T, KeyEqual, Hash>::hash_map(std::initializer_list<value_type> il, const KeyEqual& equal, size_type numBuckets, const Hash& hash)
		:hash_map(equal, numBuckets, hash)
	{
		insert(std::begin(il), std::end(il));
	}

	
	template<typename Key, typename T, typename KeyEqual, typename Hash>
	hash_map<Key, T, KeyEqual, Hash>&
		hash_map<Key, T, KeyEqual, Hash>::operator=(const hash_map_type& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}


		auto copy = rhs;
		swap(copy);
		return *this;

	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	hash_map<Key, T, KeyEqual, Hash>&
		hash_map<Key, T, KeyEqual, Hash>::operator=(hash_map_type&& rhs) noexcept
	{
	}

	// 이니셜 라이저 리스트 대입 연산자

	template<typename Key, typename T, typename KeyEqual, typename Hash>
	hash_map<Key,T,KeyEqual,Hash>&
		hash_map<Key, T, KeyEqual, Hash>::operator=(std::initializer_list<value_type> il)
	{
		hash_map_type newHashMap(il, mEqual, mBuckets.size(), mHash);
		swap(newHashMap);
		return *this;
	}


	template<typename Key, typename T, typename KeyEqual, typename Hash>
	std::pair<typename hash_map<Key, T, KeyEqual, Hash>::ListType::iterator, size_t>
		hash_map<Key, T, KeyEqual, Hash>::findElement(const key_type& k)
	{
	
		size_t bucket = mHash(k) % mBuckets.size();
		auto iter = find_if(std::begin(mBuckets[bucket]), std::end(mBuckets[bucket]),
			[this, &k](const auto& element) {return mEqual(element.first, k); });

		return std::make_pair(iter, bucket);
	}

	template<typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::iterator
		hash_map<Key, T, KeyEqual, Hash>::find(const key_type& k)
	{
		auto[it, bucket] = findElement(k);
		if (it == std::end(mBuckets[bucket])) {
			return end();
		}
		return hash_map_iterator<hash_map_type>(bucket, it, this);
	}



	template<typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::const_iterator
		hash_map<Key, T, KeyEqual, Hash>::find(const key_type& k) const
	{
		return const_cast<hash_map_type*>(this)->find(k);
	}

	template<typename Key, typename T, typename KeyEqual, typename Hash>
	std::pair<typename hash_map<Key, T, KeyEqual, Hash>::iterator,
		typename hash_map<Key ,T ,KeyEqual, Hash>::iterator>
		hash_map<Key, T, KeyEqual, Hash>::equal_range(const key_type& k)
	{
		auto it = find(k);
		return std::make_pair(it, it);

	}


	template<typename Key, typename T, typename KeyEqual, typename Hash>
	std::pair<typename hash_map<Key, T, KeyEqual, Hash>::const_iterator,
		typename hash_map<Key, T, KeyEqual, Hash>::const_iterator>
		hash_map<Key, T, KeyEqual, Hash>::equal_range(const key_type& k) const
	{
		auto it = find(k);
		return std::make_pair(it, it);

	}


	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::size_type
		hash_map<Key, T, KeyEqual, Hash>::count(const key_type& k) const
	{


		if (find(k) == end())
		{
			return 0;
		}
		else {
			return 1;
		}

	}


	template <typename Key, typename T, typename KeyEqual, typename Hash>
	T& hash_map<Key, T, KeyEqual, Hash>::operator[] (const key_type& k)
	{
		return ((insert(std::make_pair(k, T()))).first)->second;
	}



	template <typename Key, typename T, typename KeyEqual, typename Hash>
	std::pair<typename hash_map<Key, T, KeyEqual, Hash>::iterator, bool>
		hash_map<Key, T, KeyEqual, Hash>::insert(const value_type& x)
	{
		auto[it, bucket] = findElement(x.first);
		bool inserted = false;
		if (it == std::end(mBuckets[bucket])) {
			it = mBuckets[bucket].insert(it, x);
			inserted = true;
			mSize++;
		}
		return std::make_pair(hash_map_iterator<hash_map_type>(bucket, it, this), inserted);
	}



	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key , T, KeyEqual, Hash>::iterator
		hash_map<Key, T, KeyEqual, Hash>::insert(const_iterator, const value_type& x)
	{
		return insert(x).first;
	}



	template <typename Key, typename T, typename KeyEqual, typename Hash>
	template <typename InputIterator>
	void hash_map<Key, T, KeyEqual, Hash>::insert(InputIterator first, InputIterator last)
	{
		// insert_iterator 어댑터를 이용하여 주어진 범위에 있는 원소를 모두 복제한다.
		// 주어진 begin()의 위치는 큰 의미가 없다. insert 메서드는 어차피 이 값을 무시한다.
		std::insert_iterator<hash_map_type> inserter(*this, begin());
		std::copy(first, last, inserter);
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	void hash_map<Key, T, KeyEqual, Hash>::insert(std::initializer_list<value_type> il)
	{
		insert(std::begin(il), std::end(il));
	}



	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::size_type
		hash_map<Key, T, KeyEqual, Hash>::erase(const key_type& k)
	{
		auto[it, bucket] = findElement(k);
		if (it != std::end(mBuckets[bucket])) {
			mBuckets[bucket].erase(it);
			mSize--;
			return 1;
		}
		else {
			return 0;
		}
	}



	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::iterator
		hash_map<Key, T, KeyEqual, Hash>::erase(iterator position)
	{
		iterator next = position;
		++next;
		mBuckets[position.mBucketindex].erase(position.mListiterator);
		mSize--;
		return next;
	}
	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::iterator
		hash_map<Key, T, KeyEqual, Hash>::erase(iterator first, iterator last)
	{
		//범위 원소 삭제
		for (iterator next = first; next != last;)
		{
			next = erase(next);
		}
		return last;
	}


	template <typename Key, typename T, typename KeyEqual, typename Hash>
	void hash_map<Key, T, KeyEqual, Hash>::clear() noexcept
	{
		for (auto& bucket : mBuckets) {
			bucket.clear();
		}
		mSize = 0;
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	bool hash_map<Key, T, KeyEqual, Hash>::empty() const
	{
		return mSize == 0;
	}


	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::size_type
		hash_map<Key, T, KeyEqual, Hash>::size() const
	{
		return mSize;
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::size_type
		hash_map<Key, T, KeyEqual, Hash>::max_size() const
	{
		return mBuckets[0].max_size();
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	void hash_map<Key, T, KeyEqual, Hash>::swap(hash_map_type& other) noexcept
	{
		using std::swap;

		swap(mBuckets, other.mBuckets);
		swap(mSize, other.mSize);
		swap(mEqual, other.mEqual);
		swap(mHash, other.mHash);
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::iterator
		hash_map<Key, T, KeyEqual, Hash>::begin()
	{
		if (mSize == 0)
		{
			return end();
		}

		for (size_t i = 0; i < mBuckets.size(); ++i) {
			if (!mBuckets[i].empty())
			{
				return hash_map_iterator<hash_map_type>(i, std::begin(mBuckets[i]), this);
			}
		}
		// 올 일은 없지만 혹시 모르니
		return end();

	}
	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::const_iterator
		hash_map<Key, T, KeyEqual, Hash>::begin() const
	{
		return const_cast<hash_map_type*>(this)->begin();
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::const_iterator
		hash_map<Key, T, KeyEqual, Hash>::cbegin() const
	{
		return begin();
	}


	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::iterator
		hash_map<Key, T, KeyEqual, Hash>::end()
	{
		size_t bucket = mBuckets.size() - 1;
		return hash_map_iterator<hash_map_type>(bucket, std::end(mBuckets[bucket]), this);
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::const_iterator
		hash_map<Key, T, KeyEqual, Hash>::end() const
	{
		return const_cast<hash_map_type*>(this)->end();
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::const_iterator
		hash_map<Key, T, KeyEqual, Hash>::cend() const
	{
		return end();
	}
	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::key_equal
		hash_map<Key, T, KeyEqual, Hash>::key_eq() const
	{
		return mEqual;
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::hasher
		hash_map<Key, T, KeyEqual, Hash>::hash_function() const
	{
		return mHash;
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::size_type
		hash_map<Key, T, KeyEqual, Hash>::bucket_count() const
	{
		return mBuckets.size();
	}
	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::size_type
		hash_map<Key, T, KeyEqual, Hash>::max_bucket_count() const
	{
		return mBuckets.max_size();
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::size_type
		hash_map<Key, T, KeyEqual, Hash>::bucket(const Key& k) const
	{
		return const_cast<hash_map_type*>(this)->findElement(k).second;
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::size_type
		hash_map<Key, T, KeyEqual, Hash>::bucket_size(size_type n) const
	{
		return mBuckets[n].size();
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::local_iterator
		hash_map<Key, T, KeyEqual, Hash>::begin(size_type n)
	{
		return mBuckets[n].begin();
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::const_local_iterator
		hash_map<Key, T, KeyEqual, Hash>::begin(size_type n) const
	{
		return mBuckets[n].begin();
	}
	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::const_local_iterator
		hash_map<Key, T, KeyEqual, Hash>::cbegin(size_type n) const
	{
		return mBuckets[n].cbegin();
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::local_iterator
		hash_map<Key, T, KeyEqual, Hash>::end(size_type n)
	{
		return mBuckets[n].end();
	}


	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::const_local_iterator
		hash_map<Key, T, KeyEqual, Hash>::end(size_type n) const
	{
		return mBuckets[n].end();
	}

	template <typename Key, typename T, typename KeyEqual, typename Hash>
	typename hash_map<Key, T, KeyEqual, Hash>::const_local_iterator
		hash_map<Key, T, KeyEqual, Hash>::cend(size_type n) const
	{
		return mBuckets[n].cend();
	}


}