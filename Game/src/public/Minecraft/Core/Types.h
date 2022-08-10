#pragma once


namespace Minecraft {

	template<typename Vector>
	struct VectorIterator
	{
	public:
		using ValueType = Vector;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

	public:
		VectorIterator(PointerType ptr) : m_Ptr(ptr) {}

		VectorIterator& operator++()
		{
			m_Ptr++;
			return *this;
		}

		VectorIterator operator++(int)
		{
			VectorIterator tmp = *this;
			++(*this);
			return tmp;
		}

		VectorIterator& operator--()
		{
			m_Ptr--;
			return *this;
		}

		VectorIterator operator--(int)
		{
			VectorIterator tmp = *this;
			--(*this);
			return tmp;
		}

		ReferenceType operator[](int index)
		{
			return *(m_Ptr + index);
		}

		PointerType operator->()
		{
			return m_Ptr;
		}

		ReferenceType operator*()
		{
			return *m_Ptr;
		}

		bool operator==(const VectorIterator& other) const
		{
			return m_Ptr == other.m_Ptr;
		};

		bool operator!=(const VectorIterator& other) const
		{
			return !(*this == other);
		}

		bool operator>=(const VectorIterator& other) const
		{
			return m_Ptr >= other.m_Ptr;
		};

		bool operator<=(const VectorIterator& other) const
		{
			return m_Ptr >= other.m_Ptr;
		};

		bool operator>(const VectorIterator& other) const
		{
			return m_Ptr > other.m_Ptr;
		};

		bool operator<(const VectorIterator& other) const
		{
			return m_Ptr > other.m_Ptr;
		};

	private:
		PointerType m_Ptr;

	};

	struct Int2
	{
		int x, y;

		Int2() : x(0), y(0) {}
		Int2(int x, int y) : x(x), y(y) {}

		using Iterator = VectorIterator<Int2>;

		bool operator==(const Int2& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Int2& other) const { return !(*this == other); }

		Int2 operator+(const Int2& other) const { return { x + other.x, y + other.y }; }
		Int2 operator-(const Int2& other) const { return { x - other.x, y - other.y }; }

		Int2 operator+(int value) const { return { x + value, y + value }; }
		Int2 operator-(int value) const { return { x - value, y - value }; }
	};

	struct Int3
	{
		int x, y, z;

		Int3() : x(0), y(0), z(0) {}
		Int3(int x, int y, int z) : x(x), y(y), z(z) {}

		using Iterator = VectorIterator<Int3>;

		bool operator==(const Int3& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator!=(const Int3& other) const { return !(*this == other); }

		Int3 operator+(const Int3& other) const { return { x + other.x, y + other.y, z + other.z }; }
		Int3 operator-(const Int3& other) const { return { x - other.x, y - other.y, z - other.z }; }

		Int3 operator+(int value) const { return { x + value, y + value, z + value }; }
		Int3 operator-(int value) const { return { x - value, y - value, z - value }; }
	};

}
