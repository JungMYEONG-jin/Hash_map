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

		// ���� �Ҹ���
		virtual ~hash_map() = default;



		// ��Ŷ ���� ���� ������ invalid_argument throw
		explicit hash_map(const KeyEqual& equal = KeyEqual(), size_t numBuckets = 101, const Hash& hash = Hash());


		// copy constructor
		hash_map(const hash_map<Key, T, KeyEqual, Hash>& src) = default;

		//move constructor noexcept�� �����ִ� ������ �̵������� ������ �����ϵ���
		// ������� �ʴ´ٸ� �����Ϸ��� ���� �������� �����
		hash_map(hash_map<Key, T, KeyEqual, Hash>&& src) noexcept = default;

		//���� ���� ����
		hash_map<Key, T, KeyEqual, Hash>& operator=(const hash_map<Key, T, KeyEqual, Hash>& rhs);

		//�̵� ���� ���� noexcept�� �̵��������� ������ ä���ϵ��� hint�� �ִ°�
		hash_map<Key, T, KeyEqual, Hash>&operator=(hash_map < Key, T, KeyEqual, Hash>&& src) noexcept;

		void insert(const value_type& x);

		void erase(const key_type& k);

		void clear() noexcept;

		// Ű�� k�� ���ҿ� ���� �����͸� ��ȯ��
		//���Ұ� ���ٸ� nullptr return
		value_type* find(const key_type& k);
		const value_type* find(const key_type& k) const;

		// Ű�� k�� ���Ҹ� ã�ų�
		// �� Ű�� ���Ұ� ���ٸ� ���� �߰�
		// �׸��� ���� �ش� Ű�� ���� ���� ���۷����� ����
		T& operator[] (const key_type& k);

		// move_semantic 
		void swap(hash_map<Key, T, KeyEqual, Hash>& otehr) noexcept;



	private:
		using ListType = std::list<value_type> ;

		//�־��� Ű�� ���� ���Ҹ� ����Ű�� �ݺ��ڿ� �� ������ ��Ŷ �ε����� ������ ���� ����
		std::pair<typename ListType::iterator, size_t> findElement(const key_type& k);
		std::vector<ListType> mBuckets;
		size_t mSize = 0;
		KeyEqual mEqual;
		Hash mHash;

	};






}

#include "hash_map.inl"