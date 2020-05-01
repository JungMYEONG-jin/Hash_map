#pragma once

#include <cstddef>
#include <vector>
#include <list>
#include <string>
#include <iterator>
#include <initializer_list>
#include <functional>

namespace Proc {
	template<typename T>
	class hash
	{
	public:
		size_t operator()(const T& key) const;
	};


	template<>
	class hash<std::string>
	{
	public:
		size_t operator()(const std::string& key) const;
	};



	//const hash map iterator 정의
	template<typename HashMap>
	class const_hash_map_iterator
	{
		friend HashMap;
	public:
		using value_type = typename HashMap::value_type;
		using difference_type = ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;
		using pointer = value_type* ;
		using reference = value_type & ;
		using list_iterator_type = typename HashMap::ListType::const_iterator;


		const_hash_map_iterator() = default;


		const_hash_map_iterator(size_t bucket, list_iterator_type listit, const HashMap* hashmap);

		// 복제생성자나 대입연산 정의 필요 x
		// 소멸자 역시 정의필요 x 기본 동작만으로 충분


		const value_type& operator*() const;

		//pair<const Key, T> 포인터 return
		const value_type* operator->() const;

		const_hash_map_iterator<HashMap>& operator++();
		const_hash_map_iterator<HashMap> operator++(int);

		const_hash_map_iterator<HashMap>& operator--();
		const_hash_map_iterator<HashMap> operator--(int);

		bool operator==(const const_hash_map_iterator<HashMap>& rhs) const;
		bool operator!=(const const_hash_map_iterator<HashMap>& rhs) const;


	protected:
		size_t mBucketindex = 0;
		list_iterator_type mListiterator;
		const HashMap* mHashmap = nullptr;

		void increment();
		void decrement();


	};



	template<typename HashMap>
	class hash_map_iterator : public const_hash_map_iterator<HashMap>
	{



		friend HashMap;

	public:

		using value_type = typename const_hash_map_iterator<HashMap>::value_type;
		using difference_type = ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;
		using pointer = value_type* ;
		using reference = value_type & ;
		using list_iterator_type = typename HashMap::ListType::iterator;

		hash_map_iterator() = default;

		hash_map_iterator(size_t bucket, list_iterator_type listit, HashMap* hashmap);

		value_type& operator*();
		value_type* operator->();

		hash_map_iterator<HashMap>& operator++();
		hash_map_iterator<HashMap> operator++(int);

		hash_map_iterator<HashMap>& operator--();
		hash_map_iterator<HashMap> operator--(int);

	};








	template<typename Key, typename T, typename KeyEqual=std::equal_to<>,typename Hash=hash<Key>>
	class hash_map
	{
	public:
		using key_type = Key;
		using mapped_type = T;
		using value_type = std::pair<const Key, T>;
		using hasher = Hash;
		using key_equal = KeyEqual;
		using reference = value_type& ;
		using const_reference = const value_type&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using hash_map_type = hash_map<Key, T, KeyEqual, Hash>;
		using iterator = hash_map_iterator<hash_map_type>;
		using const_iterator = const_hash_map_iterator<hash_map_type>;



	private:
		using ListType = std::list<value_type>;

	public:
		using local_iterator = typename ListType::iterator;
		using const_local_iterator = typename ListType::const_iterator;

		//iterator 클래스는 hash_map의 모든 원소에 접근할 수 있어야함
		friend class hash_map_iterator<hash_map_type>;
		friend class const_hash_map_iterator<hash_map_type>;




		// 가상 소멸자
		virtual ~hash_map() = default;



		// 버킷 수가 맞지 않으면 invalid_argument throw
		explicit hash_map(const KeyEqual& equal = KeyEqual(), size_t numBuckets = 101, const Hash& hash = Hash());





		template<typename Inputiterator>
		hash_map(Inputiterator first, Inputiterator last, const KeyEqual& equal = KeyEqual(), size_type numBuckets = 101, const Hash& hash = Hash());

		explicit hash_map(std::initializer_list<value_type> il, const KeyEqual& equal = KeyEqual(), size_type numBuckets = 101, const Hash& hash = Hash());


		hash_map(const hash_map_type& src) = default;

		hash_map(hash_map_type&& src) noexcept = default;
		

		hash_map_type& operator=(const hash_map_type& rhs);
		hash_map_type& operator=(hash_map_type&& rhs) noexcept;

		hash_map_type& operator=(std::initializer_list<value_type> il);

		//반복자 메소드
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		const_iterator cbegin() const;
		const_iterator cend() const;

		//크기 관련 메소드
		bool empty() const;
		size_type size() const;
		size_type max_size() const;

		// 원소 추가 메소드
		T& operator[] (const key_type& k);
		std::pair<iterator, bool> insert(const value_type& x);
		iterator insert(const_iterator hint, const value_type& x);
		template <typename Inputiterator>
		void insert(Inputiterator first, Inputiterator last);
		void insert(std::initializer_list<value_type> il);

		//원소 삭제 메소드
		size_type erase(const key_type& k);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

		//기타 수정 유틸
		void swap(hash_map_type& other) noexcept;
		void clear() noexcept;


		// 표준 라이버르리 조건에 맞추기 위한 접근 메서드
		key_equal key_eq() const;
		hasher hash_function() const;

		//룩업 메서드
		iterator find(const key_type& k);
		const_iterator find(const key_type& k) const;
		std::pair<iterator, iterator> equal_range(const key_type& k);
		std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;

		size_type count(const key_type& k) const;

		size_type bucket_count() const;
		size_type max_bucket_count() const;
		size_type bucket(const Key& k) const;
		size_type bucket_size(size_type n) const;
		local_iterator begin(size_type n);
		const_local_iterator begin(size_type n) const;
		const_local_iterator cbegin(size_type n) const;
		local_iterator end(size_type n);
		const_local_iterator end(size_type n) const;
		const_local_iterator cend(size_type n) const;



		

	



	private:

		//주어진 키에 대한 원소를 가리키는 반복자와 그 원소의 버킷 인덱스로 구성된 쌍을 리턴
		std::pair<typename ListType::iterator, size_t> findElement(const key_type& k);
		std::vector<ListType> mBuckets;
		size_t mSize = 0;
		KeyEqual mEqual;
		Hash mHash;

	};






}

#include "hash_map.inl"