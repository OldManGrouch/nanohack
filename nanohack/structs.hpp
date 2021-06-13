#include <array>

class ListDictionary
{
public:
	char pad_0000[ 0x20 ];
	class BufferList* keys;
	class BufferList* vals;
};
class BufferList
{
public:
	char pad_0000[ 0x10 ];
	int32_t size;
	char pad_0014[ 0x4 ];
	void* buffer;
};
class Transform;
bool LineOfSight(Vector3, Vector3);
class Bone {
public:
	Vector3 position;
	bool visible;
	Transform* transform;

	Bone( ) {
		this->position = Vector3::Zero( );
		this->visible = false;
	}
	Bone(Vector3 position, bool visible) {
		this->position = position;
		this->visible = visible;
		this->transform = nullptr;
	}
	Bone(Vector3 position, bool visible, Transform* traa) {
		this->position = position;
		this->visible = visible;
		this->transform = traa;
	}
	bool visible_(Vector3 from) {
		return LineOfSight(this->position, from);
	}
};
struct Ray {
public:
	Vector3 m_Origin;
	Vector3 m_Direction;

	Ray( ) {
		this->m_Origin = Vector3(0, 0, 0);
		this->m_Direction = Vector3(0, 0, 0);
	}

	Ray(Vector3 origin, Vector3 direction) {
		this->m_Origin = origin;
		this->m_Direction = direction;
	}

	Vector3 ClosestPoint(Vector3 pos) {
		return this->m_Origin + this->m_Direction * (pos - this->m_Origin).dot_product(this->m_Direction);
	}
};
class TickInterpolator {
private:
	struct Segment {
		Vector3 point;
		float length;
		Segment(Vector3 a, Vector3 b) {
			this->point = b;
			this->length = a.distance(b);
		}
	};
public:
	std::vector<Segment> points = std::vector<Segment>( );
	int index;
	float Length;
	Vector3 CurrentPoint;
	Vector3 StartPoint;
	Vector3 EndPoint;

	void Reset( ) {
		this->index = 0;
		this->CurrentPoint = this->StartPoint;
	}
	void Reset(Vector3 point) {
		this->points.clear( );
		this->index = 0;
		this->Length = 0.f;
		this->EndPoint = point;
		this->StartPoint = point;
		this->CurrentPoint = point;
	}
	void AddPoint(Vector3 point) {
		Segment segment = Segment(this->EndPoint, point);
		this->points.push_back(segment);
		this->Length += segment.length;
		this->EndPoint = segment.point;
	}
	bool MoveNext(float distance) {
		float num = 0.f;
		while (num < distance && this->index < this->points.size( )) {
			Segment segment = this->points[ this->index ];
			this->CurrentPoint = segment.point;
			num += segment.length;
			this->index++;
		}
		return num > 0.f;
	}
	bool HasNext( ) {
		return this->index < this->points.size( );
	}
};
template<typename T = void*>
struct List {
public:
	char pad_0000[ 0x10 ];
	void* buffer;
	uint32_t size;

	T* get(uint32_t idx) {
		if (!this) return nullptr;

		if (idx > this->size) return nullptr;

		void* items = this->buffer;

		if (!items) return nullptr;

		return *reinterpret_cast<T**>((uint64_t)items + (0x20 + (idx * 0x8)));
	}
};
class CBounds {
public:
	Vector3 center;
	Vector3 extents;
};
struct box_bounds {
	float left, right, top, bottom;

	bool empty( ) {
		if (this->left == 0 && this->right == 0 && this->top == 0 && this->bottom == 0)
			return true;

		if (this->left == FLT_MAX || this->right == FLT_MIN || this->top == FLT_MAX || this->bottom == FLT_MIN)
			return true;

		return false;
	}
	static box_bounds null( ) {
		return { 0, 0, 0, 0 };
	}
};
struct Color {
public:
	float r, g, b, a;

	Color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};
template<typename T = void*>
class Array {
public:
	uint32_t size( ) {
		if (!this) return 0;
		return *reinterpret_cast<uint32_t*>(this + 0x18);
	}
	T get(int idx) {
		if (!this) return T{};
		return *reinterpret_cast<T*>(this + (0x20 + (idx * 0x8)));
	}
	T gett(int idx) {
		if (!this) return T{};
		return *reinterpret_cast<T*>(this + 0x20 + idx * 0x8);
	}
};
struct Line {
	Vector3 point0;
	Vector3 point1;

	Line(Vector3 point0, Vector3 point1) {
		this->point0 = point0;
		this->point1 = point1;
	}

	Line(Vector3 origin, Vector3 direction, float length) {
		this->point0 = origin;
		this->point1 = origin + direction * length;
	}
	Vector3 ClosestPoint(Vector3 pos) {
		Vector3 a = this->point1 - this->point0;
		float magnitude = a.magnitude( );
		Vector3 vector = a / magnitude;
		return this->point0 + (vector * std::clamp(Vector3(pos - this->point0).dot_product(vector), 0.f, magnitude));
	}
	float Distance(Vector3 pos) {
		return (pos - this->ClosestPoint(pos)).magnitude( );
	}
};