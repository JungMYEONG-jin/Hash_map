#pragma once

#include <cstddef>
#include <vector>
#include <list>
#include <string>
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



	template<typename Key, typename T, typename KeyEqual=std::equal_to<>,typename Hash=hash<Key>>
	class hash_map
	{
	public:
		using key_type = Key;
		using mapped_type = T;
		using value_type = std::pair<const Key, T>;

		// 가상 소멸자
		virtual ~hash_map() = default;



		// 버킷 수가 맞지 않으면 invalid_argument throw
		explicit hash_map(const KeyEqual& equal = KeyEqual(), size_t numBuckets = 101, const Hash& hash = Hash());


		// copy constructor
		hash_map(const hash_map<Key, T, KeyEqual, Hash>& src) = default;

		//move constructor noexcept를 적어주는 이유는 이동생성의 장점을 선택하도록
		// 명시하지 않는다면 컴파일러는 복사 생성으로 취급함
		hash_map(hash_map<Key, T, KeyEqual, Hash>&& src) noexcept = default;

		//복제 대입 연산
		hash_map<Key, T, KeyEqual, Hash>& operator=(const hash_map<Key, T, KeyEqual, Hash>& rhs);

		//이동 대입 연산 noexcept는 이동생성자의 장점을 채택하도록 hint를 주는것
		hash_map<Key, T, KeyEqual, Hash>&operator=(hash_map < Key, T, KeyEqual, Hash>&& src) noexcept;

		void insert(const value_type& x);

		void erase(const key_type& k);

		void clear() noexcept;

		// 키가 k인 원소에 대한 포인터를 반환함
		//원소가 없다면 nullptr return
		value_type* find(const key_type& k);
		const value_type* find(const key_type& k) const;

		// 키가 k인 원소를 찾거나
		// 그 키의 원소가 없다면 새로 추가
		// 그리고 나서 해당 키의 값에 대한 레퍼런스를 리턴
		T& operator[] (const key_type& k);

		// move_semantic 
		void swap(hash_map<Key, T, KeyEqual, Hash>& otehr) noexcept;



	private:
		using ListType = std::list<value_type> ;

		//주어진 키에 대한 원소를 가리키는 반복자와 그 원소의 버킷 인덱스로 구성된 쌍을 리턴
		std::pair<typename ListType::iterator, size_t> findElement(const key_type& k);
		std::vector<ListType> mBuckets;
		size_t mSize = 0;
		KeyEqual mEqual;
		Hash mHash;

	};






}

#include "hash_map.inl"